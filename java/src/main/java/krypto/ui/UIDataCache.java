package krypto.ui;

import krypto.instruments.InstrumentsClient;
import krypto.mktdata.Subscriber;
import krypto.serialization.*;

import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

public class UIDataCache implements Subscriber.Listener {

    private final InstrumentsClient instrumentsClient;
    private final Map<Long, Quote> quotes;
    private final SortedMap<Long, Instrument> instruments;

    public UIDataCache(final InstrumentsClient instrumentsClient) {
        this.instrumentsClient = instrumentsClient;
        this.instruments = new TreeMap<>();
        this.quotes = new ConcurrentHashMap<>();
    }

    @Override
    public void onHeartbeat(final Heartbeat hb) {

    }

    @Override
    public void onQuote(final Quote quote) {
        this.quotes.put(quote.securityId(), quote);
    }

    @Override
    public void onTrade(final Trade quote) {

    }

    @Override
    public void onOrderUpdate(final OrderUpdate ou) {

    }

    public SortedMap<Long, Instrument> getInstruments(final boolean reloadCache) {
        if (reloadCache || this.instruments.isEmpty()) {
            this.instruments.clear();
            instrumentsClient.getInstruments().forEach(this.instruments::put);
        }
        return instruments;
    }

    public Map<Long, Quote> getQuotes() {
        return quotes;
    }
}
