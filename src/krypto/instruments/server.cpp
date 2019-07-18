#include <utility>
#include <krypto/instruments/server.h>
#include <krypto/utils/enum.h>

namespace {
    const auto convert_inst_type = krypto::utils::convert_compatible_enum<
            krypto::common::InstrumentType, krypto::serialization::InstrumentType>;

    const auto convert_currency = krypto::utils::convert_compatible_enum<
            krypto::common::Currency , krypto::serialization::Currency>;
}

void krypto::instruments::Server::process(const krypto::serialization::InstrumentRequest *) {
    fb_builder_.Clear();
    std::vector<flatbuffers::Offset<krypto::serialization::Instrument>> instruments;
    for (auto&& inst : cache_) {
        auto inst_offset = krypto::serialization::CreateInstrument(fb_builder_,
                inst.id,
                ::convert_inst_type(inst.inst_type),
                fb_builder_.CreateString(inst.symbol),
                fb_builder_.CreateString(
                        krypto::common::ExchangeTypeEnum::enum_to_names.at(inst.exchange)),
                fb_builder_.CreateString(inst.exchange_symbol),
                inst.tick_size,
                inst.min_size,
                inst.max_size,
                ::convert_currency(inst.crypto_base),
                ::convert_currency(inst.crypto_quote));
        instruments.emplace_back(inst_offset);
    }
    auto instruments_offset = fb_builder_.CreateVector(instruments);
    krypto::serialization::InstrumentResponseBuilder builder{fb_builder_};
    builder.add_instruments(instruments_offset);
    auto response = builder.Finish();
    fb_builder_.Finish(response);
}

krypto::instruments::Server::Server(
        zmq::context_t &context,
        std::string broker,
        std::string service,
        bool verbose,
        const krypto::Config &config) :
        krypto::network::rpc::WorkerBase<Server,
                krypto::serialization::InstrumentRequest,
                krypto::serialization::InstrumentResponse>::WorkerBase{
                context, std::move(broker), std::move(service), verbose},
        store_{config} {
    cache_ = store_.load();
}