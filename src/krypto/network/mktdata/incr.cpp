#include <krypto/network/mktdata/incr.h>


namespace krypto::network::mktdata {

    void IncrementalPublisher::serialize(
            krypto::SecurityId id, krypto::Timestamp posix_time,
            const std::vector<std::tuple<krypto::Price, krypto::Quantity, krypto::serialization::OrderSide>> &updates) {
        std::vector<krypto::serialization::IncrementalPriceLevel> incrs;
        incrs.reserve(updates.size());

        for (auto&[price, qty, side] : updates) {
            incrs.emplace_back(krypto::serialization::IncrementalPriceLevel{price, qty, side});
        }

        auto updates_offset = fb_builder_.CreateVectorOfStructs<krypto::serialization::IncrementalPriceLevel>(incrs);

        krypto::serialization::IncrementalBuilder incr_builder(fb_builder_);
        incr_builder.add_security_id(id);
        incr_builder.add_timestamp(posix_time);
        incr_builder.add_updates(updates_offset);

        auto incr = incr_builder.Finish();

        fb_builder_.Finish(incr);
    }
}