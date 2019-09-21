#pragma once

#include <krypto/orders/protocol.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/network/publisher.h>

namespace krypto::orders::sim {
    class OrderUpdatePublisher final : public krypto::network::PublisherBase<OrderUpdatePublisher> {
    public:
        using krypto::network::PublisherBase<OrderUpdatePublisher>::PublisherBase;
        using krypto::network::PublisherBase<OrderUpdatePublisher>::send;
        using krypto::network::PublisherBase<OrderUpdatePublisher>::start;

        void serialize(const krypto::orders::OrderUpdate &order_update);
    };

    struct MktdataSubscriber;

    class OrderServer final : public krypto::network::rpc::WorkerBase<
            OrderServer,
            ServerParser,
            ServerParser::receive_variant_t,
            true> {
    private:
        OrderUpdatePublisher publisher_;
        std::unordered_map<uint64_t, krypto::mktdata::Quote> quotes_;
        std::unordered_map<uint64_t, uint64_t> last_hb_;

        void fill_price(int64_t security_id, const krypto::serialization::Side &side, int64_t price);

        friend MktdataSubscriber;

    public:
        OrderServer(const krypto::Config &config, std::string&& service);

        krypto::utils::MsgType process(const krypto::serialization::OrderRequest *);

        krypto::utils::MsgType process(const krypto::serialization::OrderCancelRequest *);

        krypto::utils::MsgType process(const krypto::serialization::OrderReplaceRequest *);
    };

    struct MktdataSubscriber final : public krypto::network::Subscriber<
            MktdataSubscriber, krypto::mktdata::Parser, false> {

        OrderServer &parent_;

        MktdataSubscriber(const krypto::Config &config, OrderServer &parent) :
                Subscriber(config.at<std::string>("/services/publisher/mktdata/proxy/backend/client")),
                parent_(parent) {}

        void process(const krypto::serialization::Quote *quote) {
            KRYP_LOG(info, "Got a quote");
            parent_.quotes_[quote->security_id()] = krypto::mktdata::Quote{
                    static_cast<uint64_t >(quote->timestamp()),
                    static_cast<uint64_t >(quote->security_id()),
                    quote->bid(),
                    quote->ask(),
                    quote->bid_quantity(),
                    quote->ask_quantity(),
                    quote->last(),
                    quote->last_quantity()
            };
        }

        void process(const krypto::serialization::Trade *trade) {
            parent_.fill_price(trade->security_id(), trade->side(), trade->price());
        }

        void process(const krypto::serialization::Heartbeat *hb) {
            parent_.last_hb_[hb->security_id()] = hb->timestamp();
        }
    };
}