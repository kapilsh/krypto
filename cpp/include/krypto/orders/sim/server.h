#pragma once

#include <krypto/orders/protocol.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/network/publisher.h>

#include <tbb/concurrent_hash_map.h>
#include <krypto/utils/types.h>
#include <krypto/utils/date_time.h>
#include <krypto/utils/message_types.h>

namespace krypto::orders::sim {
    template<krypto::serialization::Exchange Exchange, bool Verbose = false>
    class OrderServer final {
    private:

        std::unique_ptr<zmq::socket_t> receiver_;
        std::unique_ptr<zmq::socket_t> mktdata_subscriber_;
        std::string order_gateway_endpoint_;
        std::string mktdata_gateway_endpoint_;
        std::atomic_bool running_;
        std::unordered_map<std::string, krypto::utils::MsgType> msg_type_ref_;
        flatbuffers::FlatBufferBuilder fb_builder_;
        std::unordered_map<int64_t, krypto::utils::Quote> quotes_;
        std::unordered_map<std::string, int64_t> order_id_to_security_id_map_;
        std::unordered_map<int64_t, std::unordered_map<
                std::string,
                krypto::utils::OrderRequest>> day_orders_;
        std::unordered_map<int64_t, uint64_t> last_hb_;

        void serialize(const krypto::utils::OrderUpdate &);

        void fill_price(int64_t security_id, const krypto::serialization::Side &, int64_t price);

        void send(const krypto::utils::OrderUpdate &order_update);

        void send_reject(krypto::serialization::OrderStatus, const std::string &order_id);

        void send_ready(const std::string &identity);

        void send_disconnect(const std::string &identity);

    public:
        OrderServer(zmq::context_t &, const krypto::Config &);

        void process(const krypto::serialization::OrderRequest *);

        void process(const krypto::serialization::OrderCancelRequest *);

        void process(const krypto::serialization::OrderReplaceRequest *);

        void process(const krypto::serialization::Quote *quote);

        void process(const krypto::serialization::Trade *trade);

        void process(const krypto::serialization::Heartbeat *hb);

        void start();

        void stop();
    };

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    OrderServer<Exchange, Verbose>::OrderServer(
            zmq::context_t &context, const krypto::Config &config) :
            receiver_{std::make_unique<zmq::socket_t>(context, ZMQ_DEALER)},
            mktdata_subscriber_{std::make_unique<zmq::socket_t>(context, ZMQ_SUB)},
            order_gateway_endpoint_{config.at<std::string>(
                    "/services/order_gateway/backend/client")},
            mktdata_gateway_endpoint_{config.at<std::string>(
                    "/services/order_gateway/backend/client")},
            running_{false},
            msg_type_ref_{krypto::utils::name_to_msg_type()} {
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::serialize(const krypto::utils::OrderUpdate &order_update) {
        fb_builder_.Clear();
        auto oid_offset = fb_builder_.CreateString(order_update.order_id);
        krypto::serialization::OrderUpdateBuilder ou_builder{fb_builder_};
        ou_builder.add_timestamp(order_update.timestamp);
        ou_builder.add_filled_quantity(order_update.filled_quantity);
        ou_builder.add_status(order_update.status);
        ou_builder.add_order_id(oid_offset);
        auto ou = ou_builder.Finish();
        fb_builder_.Finish(ou);
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::fill_price(
            int64_t security_id, const krypto::serialization::Side &side,
            int64_t price) {
        // TODO: Find all prices that the trade trades through
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::process(const krypto::serialization::Quote *quote) {
        quotes_[quote->security_id()] = krypto::utils::Quote{
                static_cast<int64_t >(quote->timestamp()),
                static_cast<int64_t >(quote->security_id()),
                quote->bid(),
                quote->ask(),
                quote->bid_quantity(),
                quote->ask_quantity(),
                quote->last(),
                quote->last_quantity()
        };
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::process(const krypto::serialization::OrderRequest *request) {
        KRYP_LOG(info, "Processing new order request : {}", request->order_id()->str());
        const krypto::utils::OrderUpdate in_flight{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_IN_FLIGHT,
                0};
        send(in_flight);

        const krypto::utils::OrderUpdate accept{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_ACCEPTED,
                0
        };
        send(accept);

        if (quotes_.find(request->security_id()) != std::end(quotes_)) {
            if ((request->side() == krypto::serialization::Side_BUY &&
                 request->price() >= quotes_.at(request->security_id()).ask) ||
                (request->side() == krypto::serialization::Side_SELL &&
                 request->price() <= quotes_.at(request->security_id()).bid)) {

                KRYP_LOG(info, "=> {} -- filled", request->order_id()->str());
                const krypto::utils::OrderUpdate fill{
                        krypto::utils::current_time_in_nanoseconds(),
                        request->order_id()->str(),
                        krypto::serialization::OrderStatus::OrderStatus_FILLED,
                        request->quantity()
                };
                send(fill);
            } else {
                if (request->tif() == krypto::serialization::TimeInForce::TimeInForce_IOC) {
                    const krypto::utils::OrderUpdate expired{
                            krypto::utils::current_time_in_nanoseconds(),
                            request->order_id()->str(),
                            krypto::serialization::OrderStatus::OrderStatus_EXPIRED,
                            0
                    };
                    send(expired);
                } else {
                    if (day_orders_.find(request->security_id()) == std::end(day_orders_)) {
                        day_orders_[request->security_id()] = {};
                    }

                    krypto::utils::OrderRequest r{
                            request->timestamp(),
                            request->security_id(),
                            request->price(),
                            request->quantity(),
                            request->side(),
                            request->order_id()->str(),
                            request->tif()
                    };

                    day_orders_.at(request->security_id())[request->order_id()->str()] = r;
                    order_id_to_security_id_map_[request->order_id()->str()] = request->security_id();

                    const krypto::utils::OrderUpdate new_order{
                            krypto::utils::current_time_in_nanoseconds(),
                            request->order_id()->str(),
                            krypto::serialization::OrderStatus::OrderStatus_NEW,
                            0
                    };
                    send(new_order);
                }
            }
        }

        send_reject(
                krypto::serialization::OrderStatus::OrderStatus_REJECTED,
                request->order_id()->str());
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::process(const krypto::serialization::OrderCancelRequest *request) {
        KRYP_LOG(info, "Processing cancel order request : {}", request->order_id()->str());
        const krypto::utils::OrderUpdate in_flight{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_CANCEL_IN_FLIGHT,
                0};
        send(in_flight);

        if (order_id_to_security_id_map_.find(request->order_id()->str()) ==
            std::cend(order_id_to_security_id_map_)) {
            KRYP_LOG(info, "Order id {} not found", request->order_id()->str());
            send_reject(krypto::serialization::OrderStatus::OrderStatus_CANCEL_REJECTED,
                        request->order_id()->str());
            return;
        }

        const int64_t security_id = order_id_to_security_id_map_.at(request->order_id()->str());

        day_orders_.at(security_id).erase(request->order_id()->str());
        const krypto::utils::OrderUpdate cancelled{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_CANCELLED,
                0};
        send(cancelled);
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::process(const krypto::serialization::OrderReplaceRequest *request) {
        KRYP_LOG(info, "Processing replace order request : {}", request->order_id()->str());
        const krypto::utils::OrderUpdate in_flight{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_REPLACE_IN_FLIGHT,
                0};
        send(in_flight);

        if (order_id_to_security_id_map_.find(request->order_id()->str()) ==
            std::cend(order_id_to_security_id_map_)) {
            KRYP_LOG(info, "Order id {} not found", request->order_id()->str());
            send_reject(krypto::serialization::OrderStatus::OrderStatus_REPLACE_REJECTED,
                        request->order_id()->str());
            return;
        }

        const int64_t security_id = order_id_to_security_id_map_.at(request->order_id()->str());

        day_orders_.at(security_id).at(request->order_id()->str()).price = request->price();
        day_orders_.at(security_id).at(request->order_id()->str()).quantity = request->quantity();
        day_orders_.at(security_id).at(request->order_id()->str()).side = request->side();

        const krypto::utils::OrderUpdate replaced{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_REPLACED,
                request->quantity()};
        send(replaced);
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::process(const krypto::serialization::Trade *trade) {
        fill_price(trade->security_id(), trade->side(), trade->price());
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::process(const krypto::serialization::Heartbeat *hb) {
        last_hb_[hb->security_id()] = hb->timestamp();
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::start() {
        auto identity = krypto::serialization::EnumNameExchange(Exchange);
        krypto::network::connect(*receiver_, order_gateway_endpoint_, identity);
        mktdata_subscriber_->connect(mktdata_gateway_endpoint_);
        mktdata_subscriber_->setsockopt(ZMQ_SUBSCRIBE, "");

        if constexpr (Verbose) {
            KRYP_LOG(info, "Connected to gateway backend @ {}", order_gateway_endpoint_);
        }

        send_ready(identity);

        if constexpr (Verbose) {
            KRYP_LOG(info, "Sent ready status to broker @ {}", order_gateway_endpoint_);
        }

        zmq::pollitem_t items[] = {
                {*mktdata_subscriber_, 0, ZMQ_POLLIN, 0},
                {*receiver_,           0, ZMQ_POLLIN, 0}
        };

        running_ = true;

        while (running_) {
            zmq::poll(&items[0], 2, 0);
            if (items[0].revents && ZMQ_POLLIN) {
                auto topic = krypto::network::recv_string(*mktdata_subscriber_);
                auto topic_prefix = topic.substr(0, 2);
                auto msg_type = msg_type_ref_.at(topic_prefix);

                zmq::message_t payload_msg;
                mktdata_subscriber_->recv(&payload_msg);

                switch (msg_type) {
                    case krypto::utils::MsgType::QUOTE: {
                        process(flatbuffers::GetRoot<krypto::serialization::Quote>(payload_msg.data()));
                    }
                    case krypto::utils::MsgType::TRADE: {
                        process(flatbuffers::GetRoot<krypto::serialization::Trade>(payload_msg.data()));
                    }
                    case krypto::utils::MsgType::HEARTBEAT: {
                        process(flatbuffers::GetRoot<krypto::serialization::Heartbeat>(payload_msg.data()));
                    }
                    default: {
                        KRYP_LOG(info, "Ignoring unknown message type = {}", topic);
                    }
                }
            }

            if (items[1].revents && ZMQ_POLLIN) {
                krypto::network::recv_empty_frame(*receiver_);
                auto address = krypto::network::recv_string(*receiver_);
                auto msg_type = krypto::network::recv_msg_type(*receiver_);
                zmq::message_t payload_msg;
                receiver_->recv(&payload_msg);

                if constexpr (Verbose) {
                    KRYP_LOG(info, "Received request with payload size {} from {}",
                             payload_msg.size(), address);
                }

                switch (msg_type) {
                    case krypto::utils::MsgType::ORDER_REQUEST: {
                        process(flatbuffers::GetRoot<krypto::serialization::OrderRequest>(payload_msg.data()));
                    }
                    case krypto::utils::MsgType::ORDER_CANCEL_REQUEST: {
                        process(flatbuffers::GetRoot<krypto::serialization::OrderCancelRequest>(payload_msg.data()));
                    }
                    case krypto::utils::MsgType::ORDER_REPLACE_REQUEST: {
                        process(flatbuffers::GetRoot<krypto::serialization::OrderReplaceRequest>(payload_msg.data()));
                    }
                    default: {
                        KRYP_LOG(info, "Ignoring unknown message type. ");
                    }
                }

            }
        }

        send_disconnect(identity);
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::stop() {
        running_ = false;
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::send_reject(
            const krypto::serialization::OrderStatus order_status,
            const std::string &order_id) {
        send({
                     krypto::utils::current_time_in_nanoseconds(),
                     order_id,
                     order_status,
                     0
             });
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::send(const krypto::utils::OrderUpdate &order_update) {
        serialize(order_update);
        krypto::network::send_fb_buffer(*receiver_, fb_builder_);
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::send_ready(const std::string &identity) {
        if constexpr (Verbose) {
            KRYP_LOG(info, "Sending ready status");
        }

        krypto::network::send_empty_frame(*receiver_, ZMQ_SNDMORE);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::READY, ZMQ_SNDMORE);
        krypto::network::send_string(*receiver_, identity);
    }

    template<krypto::serialization::Exchange Exchange, bool Verbose>
    void OrderServer<Exchange, Verbose>::send_disconnect(const std::string &identity) {
        if constexpr (Verbose) {
            KRYP_LOG(info, "Sending disconnect status");
        }

        krypto::network::send_empty_frame(*receiver_, ZMQ_SNDMORE);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::DISCONNECT, ZMQ_SNDMORE);
        krypto::network::send_string(*receiver_, identity);
    }
}