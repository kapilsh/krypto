// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Snapshot extends Table {
  public static Snapshot getRootAsSnapshot(ByteBuffer _bb) { return getRootAsSnapshot(_bb, new Snapshot()); }
  public static Snapshot getRootAsSnapshot(ByteBuffer _bb, Snapshot obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public Snapshot __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long securityId() { int o = __offset(6); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public SnapshotPriceLevel bids(int j) { return bids(new SnapshotPriceLevel(), j); }
  public SnapshotPriceLevel bids(SnapshotPriceLevel obj, int j) { int o = __offset(8); return o != 0 ? obj.__assign(__vector(o) + j * 16, bb) : null; }
  public int bidsLength() { int o = __offset(8); return o != 0 ? __vector_len(o) : 0; }
  public SnapshotPriceLevel asks(int j) { return asks(new SnapshotPriceLevel(), j); }
  public SnapshotPriceLevel asks(SnapshotPriceLevel obj, int j) { int o = __offset(10); return o != 0 ? obj.__assign(__vector(o) + j * 16, bb) : null; }
  public int asksLength() { int o = __offset(10); return o != 0 ? __vector_len(o) : 0; }

  public static int createSnapshot(FlatBufferBuilder builder,
      long timestamp,
      long security_id,
      int bidsOffset,
      int asksOffset) {
    builder.startObject(4);
    Snapshot.addSecurityId(builder, security_id);
    Snapshot.addTimestamp(builder, timestamp);
    Snapshot.addAsks(builder, asksOffset);
    Snapshot.addBids(builder, bidsOffset);
    return Snapshot.endSnapshot(builder);
  }

  public static void startSnapshot(FlatBufferBuilder builder) { builder.startObject(4); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addSecurityId(FlatBufferBuilder builder, long securityId) { builder.addLong(1, securityId, 0L); }
  public static void addBids(FlatBufferBuilder builder, int bidsOffset) { builder.addOffset(2, bidsOffset, 0); }
  public static void startBidsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(16, numElems, 8); }
  public static void addAsks(FlatBufferBuilder builder, int asksOffset) { builder.addOffset(3, asksOffset, 0); }
  public static void startAsksVector(FlatBufferBuilder builder, int numElems) { builder.startVector(16, numElems, 8); }
  public static int endSnapshot(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}
