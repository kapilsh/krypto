// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class OrderCancelRequest extends Table {
  public static OrderCancelRequest getRootAsOrderCancelRequest(ByteBuffer _bb) { return getRootAsOrderCancelRequest(_bb, new OrderCancelRequest()); }
  public static OrderCancelRequest getRootAsOrderCancelRequest(ByteBuffer _bb, OrderCancelRequest obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public OrderCancelRequest __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public String orderId() { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer orderIdAsByteBuffer() { return __vector_as_bytebuffer(6, 1); }
  public ByteBuffer orderIdInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 6, 1); }

  public static int createOrderCancelRequest(FlatBufferBuilder builder,
      long timestamp,
      int order_idOffset) {
    builder.startObject(2);
    OrderCancelRequest.addTimestamp(builder, timestamp);
    OrderCancelRequest.addOrderId(builder, order_idOffset);
    return OrderCancelRequest.endOrderCancelRequest(builder);
  }

  public static void startOrderCancelRequest(FlatBufferBuilder builder) { builder.startObject(2); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addOrderId(FlatBufferBuilder builder, int orderIdOffset) { builder.addOffset(1, orderIdOffset, 0); }
  public static int endOrderCancelRequest(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}
