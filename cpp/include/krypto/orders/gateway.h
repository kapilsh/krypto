#pragma once


#include <atomic>
#include <string>
#include <queue>
#include <unordered_map>

#include <zmq.hpp>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/network/helper.h>
#include <krypto/utils/types.h>

namespace krypto::orders {

    template<bool Verbose = false>
    class OrderGateway final {
    private:
        std::string frontend_address_;
        std::string backend_address_;
        std::unique_ptr<zmq::socket_t> frontend_;
        std::unique_ptr<zmq::socket_t> backend_;
        std::atomic_bool running_;
        std::unordered_map<std::string, std::string> workers_;

        bool bind();

    public:
        OrderGateway(zmq::context_t &, const krypto::Config &);

        void start();

        void stop();
    };


    template<bool Verbose>
    OrderGateway<Verbose>::OrderGateway(zmq::context_t &context, const krypto::Config &config) :
            frontend_address_{config.at<std::string>("/services/order_gateway/frontend/server")},
            backend_address_{config.at<std::string>("/services/order_gateway/backend/server")},
            frontend_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
            backend_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
            running_{false} {}

    template<bool Verbose>
    bool OrderGateway<Verbose>::bind() {
        bool bound = false;
        KRYP_LOG(info, "Binding frontend to {}", frontend_address_);
        try {
            frontend_->bind(frontend_address_);
            bound = true;
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with {}", e.what());
        }
        KRYP_LOG(info, "Binding backend to {}", backend_address_);
        try {
            backend_->bind(backend_address_);
            bound = true;
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with {}", e.what());
        }
        if (!bound) {
            KRYP_LOG(warn, "Failed starting proxy - binding problems");
        }
        return bound;
    }

    template<bool Verbose>
    void OrderGateway<Verbose>::start() {
        running_ = bind();

        if (running_) {
            KRYP_LOG(info, "Started order gateway");
        }

        while (running_) {
            zmq::pollitem_t items[] = {
                    {*backend_,  0, ZMQ_POLLIN, 0},
                    {*frontend_, 0, ZMQ_POLLIN, 0}
            };

            zmq::poll(&items[0], 2, 0);

            if (items[0].revents && ZMQ_POLLIN) {
                auto address = krypto::network::recv_string(*backend_);
                krypto::network::recv_empty_frame(*backend_);

                auto status = krypto::network::recv_status(*backend_);
                auto exchange = krypto::network::recv_string(*backend_);

                if (status == krypto::network::SocketStatus::READY) {
                    KRYP_LOG(info, "{} :: ready @ address {}", exchange, address);
                    workers_[exchange] = address;
                } else if (status == krypto::network::SocketStatus::DISCONNECT) {
                    KRYP_LOG(info, "{} :: disconnected", exchange);
                    workers_.erase(exchange);
                } else if (status == krypto::network::SocketStatus::REPLY) {
                    auto client_addr = krypto::network::recv_string(*backend_);
                    auto msg_type = krypto::network::recv_msg_type(*backend_);
                    krypto::network::send_string(*frontend_, client_addr, ZMQ_SNDMORE);
                    krypto::network::send_empty_frame(*frontend_, ZMQ_SNDMORE);
                    krypto::network::send_string(*frontend_, exchange, ZMQ_SNDMORE);

                    bool no_more_flag = msg_type == krypto::utils::MsgType::NO_PAYLOAD ||
                                        msg_type == krypto::utils::MsgType::UNDEFINED;
                    krypto::network::send_msg_type(*frontend_, msg_type, no_more_flag ? ZMQ_NULL : ZMQ_SNDMORE);
                    if (no_more_flag) {
                        continue;
                    }

                    zmq::message_t payload;
                    backend_->recv(&payload);
                    if constexpr (Verbose) {
                        KRYP_LOG(info, "{} :: received reply payload of size {}",
                                 exchange, payload.size());
                    }
                    frontend_->send(payload);
                }
            }

            if (items[1].revents && ZMQ_POLLIN) {

                auto client_addr = krypto::network::recv_string(*frontend_);
                krypto::network::recv_empty_frame(*frontend_);
                auto exchange = krypto::network::recv_string(*frontend_);
                auto msg_type = krypto::network::recv_msg_type(*frontend_);

                zmq::message_t request_payload;
                frontend_->recv(&request_payload);

                if (workers_.find(exchange) != std::end(workers_)) {
                    auto worker_addr = workers_.at(exchange);
                    krypto::network::send_string(*backend_, worker_addr, ZMQ_SNDMORE);
                    krypto::network::send_empty_frame(*backend_, ZMQ_SNDMORE);
                    krypto::network::send_string(*backend_, client_addr, ZMQ_SNDMORE);
                    if constexpr (Verbose) {
                        KRYP_LOG(info, "{} sending request to {}@{} with payload size {}",
                                 client_addr, exchange, worker_addr, request_payload.size());
                    }
                    krypto::network::send_msg_type(*backend_, msg_type, ZMQ_SNDMORE);
                    backend_->send(request_payload);
                } else {
                    KRYP_LOG(info, "Service not available");
                    krypto::network::send_string(*frontend_, client_addr, ZMQ_SNDMORE);
                    krypto::network::send_empty_frame(*frontend_, ZMQ_SNDMORE);
                    krypto::network::send_string(*frontend_, exchange, ZMQ_SNDMORE);
                    krypto::network::send_msg_type(*frontend_, krypto::utils::MsgType::UNDEFINED);
                }
            }
        }
    }

    template<bool Verbose>
    void OrderGateway<Verbose>::stop() {
        running_ = false;
    }
}
