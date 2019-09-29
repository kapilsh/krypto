// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class OrderReplaceRequest extends Table {
  public static OrderReplaceRequest getRootAsOrderReplaceRequest(ByteBuffer _bb) { return getRootAsOrderReplaceRequest(_bb, new OrderReplaceRequest()); }
  public static OrderReplaceRequest getRootAsOrderReplaceRequest(ByteBuffer _bb, OrderReplaceRequest obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public OrderReplaceRequest __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public String orderId() { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer orderIdAsByteBuffer() { return __vector_as_bytebuffer(6, 1); }
  public ByteBuffer orderIdInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 6, 1); }
  public long price() { int o = __offset(8); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long quantity() { int o = __offset(10); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public byte side() { int o = __offset(12); return o != 0 ? bb.get(o + bb_pos) : 0; }

  public static int createOrderReplaceRequest(FlatBufferBuilder builder,
      long timestamp,
      int order_idOffset,
      long price,
      long quantity,
      byte side) {
    builder.startObject(5);
    OrderReplaceRequest.addQuantity(builder, quantity);
    OrderReplaceRequest.addPrice(builder, price);
    OrderReplaceRequest.addTimestamp(builder, timestamp);
    OrderReplaceRequest.addOrderId(builder, order_idOffset);
    OrderReplaceRequest.addSide(builder, side);
    return OrderReplaceRequest.endOrderReplaceRequest(builder);
  }

  public static void startOrderReplaceRequest(FlatBufferBuilder builder) { builder.startObject(5); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addOrderId(FlatBufferBuilder builder, int orderIdOffset) { builder.addOffset(1, orderIdOffset, 0); }
  public static void addPrice(FlatBufferBuilder builder, long price) { builder.addLong(2, price, 0L); }
  public static void addQuantity(FlatBufferBuilder builder, long quantity) { builder.addLong(3, quantity, 0L); }
  public static void addSide(FlatBufferBuilder builder, byte side) { builder.addByte(4, side, 0); }
  public static int endOrderReplaceRequest(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

