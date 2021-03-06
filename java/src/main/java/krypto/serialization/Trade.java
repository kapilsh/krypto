// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Trade extends Table {
  public static Trade getRootAsTrade(ByteBuffer _bb) { return getRootAsTrade(_bb, new Trade()); }
  public static Trade getRootAsTrade(ByteBuffer _bb, Trade obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public Trade __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long securityId() { int o = __offset(6); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long price() { int o = __offset(8); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long quantity() { int o = __offset(10); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public byte side() { int o = __offset(12); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public String tradeId() { int o = __offset(14); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer tradeIdAsByteBuffer() { return __vector_as_bytebuffer(14, 1); }
  public ByteBuffer tradeIdInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 14, 1); }

  public static int createTrade(FlatBufferBuilder builder,
      long timestamp,
      long security_id,
      long price,
      long quantity,
      byte side,
      int trade_idOffset) {
    builder.startObject(6);
    Trade.addQuantity(builder, quantity);
    Trade.addPrice(builder, price);
    Trade.addSecurityId(builder, security_id);
    Trade.addTimestamp(builder, timestamp);
    Trade.addTradeId(builder, trade_idOffset);
    Trade.addSide(builder, side);
    return Trade.endTrade(builder);
  }

  public static void startTrade(FlatBufferBuilder builder) { builder.startObject(6); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addSecurityId(FlatBufferBuilder builder, long securityId) { builder.addLong(1, securityId, 0L); }
  public static void addPrice(FlatBufferBuilder builder, long price) { builder.addLong(2, price, 0L); }
  public static void addQuantity(FlatBufferBuilder builder, long quantity) { builder.addLong(3, quantity, 0L); }
  public static void addSide(FlatBufferBuilder builder, byte side) { builder.addByte(4, side, 0); }
  public static void addTradeId(FlatBufferBuilder builder, int tradeIdOffset) { builder.addOffset(5, tradeIdOffset, 0); }
  public static int endTrade(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

