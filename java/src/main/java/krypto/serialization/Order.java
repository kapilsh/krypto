// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Order extends Table {
  public static Order getRootAsOrder(ByteBuffer _bb) { return getRootAsOrder(_bb, new Order()); }
  public static Order getRootAsOrder(ByteBuffer _bb, Order obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public Order __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public OrderRequest request() { return request(new OrderRequest()); }
  public OrderRequest request(OrderRequest obj) { int o = __offset(4); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public OrderUpdate updates(int j) { return updates(new OrderUpdate(), j); }
  public OrderUpdate updates(OrderUpdate obj, int j) { int o = __offset(6); return o != 0 ? obj.__assign(__indirect(__vector(o) + j * 4), bb) : null; }
  public int updatesLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public double fees() { int o = __offset(8); return o != 0 ? bb.getDouble(o + bb_pos) : 0.0; }

  public static int createOrder(FlatBufferBuilder builder,
      int requestOffset,
      int updatesOffset,
      double fees) {
    builder.startObject(3);
    Order.addFees(builder, fees);
    Order.addUpdates(builder, updatesOffset);
    Order.addRequest(builder, requestOffset);
    return Order.endOrder(builder);
  }

  public static void startOrder(FlatBufferBuilder builder) { builder.startObject(3); }
  public static void addRequest(FlatBufferBuilder builder, int requestOffset) { builder.addOffset(0, requestOffset, 0); }
  public static void addUpdates(FlatBufferBuilder builder, int updatesOffset) { builder.addOffset(1, updatesOffset, 0); }
  public static int createUpdatesVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startUpdatesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addFees(FlatBufferBuilder builder, double fees) { builder.addDouble(2, fees, 0.0); }
  public static int endOrder(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

