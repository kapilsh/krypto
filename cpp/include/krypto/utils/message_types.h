#pragma once

#include <string>

#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/types.h>
#include <ostream>

namespace krypto::utils {
    struct InstrumentRequest {
        krypto::serialization::RequestType request_type;
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::INSTRUMENT_REQUEST;
    };

    struct InstrumentCacheRefreshRequest {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST;
    };

    struct OrderUpdate final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_UPDATE;
        int64_t timestamp;
        std::string order_id;
        std::string exchange_order_id;
        krypto::serialization::OrderStatus status;
        int64_t filled_quantity;
    };

    struct OrderRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_REQUEST;
        int64_t timestamp;
        int64_t security_id;
        int64_t price;
        int64_t quantity;
        krypto::serialization::Side side;
        std::string order_id;
        krypto::serialization::TimeInForce tif;
    };

    struct OrderCancelRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_CANCEL_REQUEST;
        int64_t timestamp;
        std::string order_id;
    };

    struct OrderReplaceRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_REPLACE_REQUEST;
        int64_t timestamp;
        std::string order_id;
        int64_t price;
        int64_t quantity;
        krypto::serialization::Side side;
    };

    struct TheoreticalSnapshot final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::THEO;
        int64_t timestamp;
        int64_t security_id;
        double_t price;
        double_t adjusted_price;
        double_t error;
        double_t mm_base_bid;
        double_t mm_base_ask;
        double_t mm_base_spread;
        double_t bid_liquidity;
        double_t ask_liquidity;
        bool valid;

        friend std::ostream &operator<<(std::ostream &os, const TheoreticalSnapshot &snapshot) {
            os << "security_id: " << snapshot.security_id << " price: " << snapshot.price << " adjusted_price: "
               << snapshot.adjusted_price << " error: " << snapshot.error << " mm_base_bid: " << snapshot.mm_base_bid
               << " mm_base_ask: " << snapshot.mm_base_ask << " mm_base_spread: " << snapshot.mm_base_spread
               << " bid_liquidity: " << snapshot.bid_liquidity << " ask_liquidity: " << snapshot.ask_liquidity
               << " valid: " << snapshot.valid;
            return os;
        }
    };


}
