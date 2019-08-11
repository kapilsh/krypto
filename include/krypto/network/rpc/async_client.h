#pragma once

#include <utility>
#include <bitset>
#include <memory>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include <krypto/network/helper.h>
#include <krypto/logger.h>
#include <krypto/config.h>

namespace krypto::network::rpc {

    template<typename Derived, bool Verbose = false>
    class AsyncClientBase {
    private:
        zmq::context_t context_;
        std::string broker_;
        std::unique_ptr<zmq::socket_t> socket_;

        template<typename... Args>
        bool send_impl(const std::string &, Args...);

        template<typename ReceiveType>
        const ReceiveType * receive_impl(const std::string &);

    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        explicit AsyncClientBase(const krypto::Config& config);

        void connect();

        template<typename ReceiveType, typename... Args>
        const ReceiveType * send(std::string service, Args... args);
    };

    template<typename Derived, bool Verbose>
    AsyncClientBase<Derived, Verbose>::AsyncClientBase(
            const krypto::Config& config) :
            context_{1},
            broker_{config.at<std::string>("/services/rpc/broker/frontend/client")} {

        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_REQ);

        connect();
    }

    template<typename Derived, bool Verbose>
    void AsyncClientBase<Derived, Verbose>::connect() {
        if constexpr  (Verbose) {
            KRYP_LOG(info, "Connecting to broker @ {}", broker_);
        }

        int linger = 0;
        socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
        auto identity = "client-" + generate_id();
        socket_->setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
        socket_->connect(broker_.c_str());
    }

    template<typename Derived, bool Verbose>
    template<typename ReceiveType, typename... Args>
    const ReceiveType * AsyncClientBase<Derived, Verbose>::send(std::string service_name, Args... args) {

        send_impl(service_name, args...);

        if constexpr  (Verbose) {
            KRYP_LOG(debug, "Sent message to {}", service_name);
        }

        return receive_impl<ReceiveType>(service_name);
    }

    template<typename Derived, bool Verbose>
    template<typename... Args>
    bool AsyncClientBase<Derived, Verbose>::send_impl(const std::string &service_name, Args... args) {
        std::bitset<2> status;

        zmq::message_t service_name_msg(service_name.size());
        std::memcpy(service_name_msg.data(), service_name.data(), service_name.size());
        status.set(0, socket_->send(service_name_msg, ZMQ_SNDMORE));

        auto &derived = static_cast<Derived &>(*this);
        derived.serialize(args...);

        zmq::message_t payload_msg(fb_builder_.GetSize());
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        status.set(1, socket_->send(payload_msg));

        return status.all();
    }

    template<typename Derived, bool Verbose>
    template<typename ReceiveType>
    const ReceiveType * AsyncClientBase<Derived, Verbose>::receive_impl(const std::string &service_name) {

        zmq::message_t service_msg;

        if (!socket_->recv(&service_msg, 0)) {
            return nullptr;
        }

        auto s_name = std::string(static_cast<char *>(service_msg.data()), service_msg.size());

        if (s_name != service_name) {
            KRYP_LOG(warn, "Service name received [{}] != expected [{}]", s_name, service_name);
            return std::nullptr_t {};
        }

        if (!service_msg.more()) {
            KRYP_LOG(error, "No data received");
            return nullptr;
        }

        zmq::message_t payload_msg;
        socket_->recv(&payload_msg);

        auto payload = flatbuffers::GetRoot<ReceiveType>(payload_msg.data());

        if (payload_msg.more()) {
            KRYP_LOG(error, "Received more than 2 frames");
        }

        return payload;
    }
}