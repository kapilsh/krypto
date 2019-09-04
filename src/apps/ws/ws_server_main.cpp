#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/utils/common.h>
//#include <krypto/mktdata/client.h>
#include <krypto/mktdata/ws_server.h>

namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}


std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char **argv) {
    if (argc < 2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    krypto::utils::Startup::init();
    const krypto::Config config(argv[1]);

    krypto::mktdata::WebsocketServer ws{config};

    auto mktdata_done = std::async(std::launch::async, [&ws]() {
        ws.subscribe(krypto::utils::MsgType::ALL);
        ws.start();
    });

    auto done = std::async(std::launch::async, [&ws]() {
        ws.process_messages();
    });

    ws.ioc_run();

    shutdown_handler = [&ws](int signal) {
        SIGNAL_STATUS = signal;
        ws.done();
        ws.stop();
    };

    std::signal(SIGINT, signal_handler);

    done.wait();
    mktdata_done.wait();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}


