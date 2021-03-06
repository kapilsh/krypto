// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class RiskSummary extends Table {
  public static RiskSummary getRootAsRiskSummary(ByteBuffer _bb) { return getRootAsRiskSummary(_bb, new RiskSummary()); }
  public static RiskSummary getRootAsRiskSummary(ByteBuffer _bb, RiskSummary obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public RiskSummary __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public Position positions(int j) { return positions(new Position(), j); }
  public Position positions(Position obj, int j) { int o = __offset(6); return o != 0 ? obj.__assign(__vector(o) + j * 16, bb) : null; }
  public int positionsLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public double pnl() { int o = __offset(8); return o != 0 ? bb.getDouble(o + bb_pos) : 0.0; }

  public static int createRiskSummary(FlatBufferBuilder builder,
      long timestamp,
      int positionsOffset,
      double pnl) {
    builder.startObject(3);
    RiskSummary.addPnl(builder, pnl);
    RiskSummary.addTimestamp(builder, timestamp);
    RiskSummary.addPositions(builder, positionsOffset);
    return RiskSummary.endRiskSummary(builder);
  }

  public static void startRiskSummary(FlatBufferBuilder builder) { builder.startObject(3); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addPositions(FlatBufferBuilder builder, int positionsOffset) { builder.addOffset(1, positionsOffset, 0); }
  public static void startPositionsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(16, numElems, 8); }
  public static void addPnl(FlatBufferBuilder builder, double pnl) { builder.addDouble(2, pnl, 0.0); }
  public static int endRiskSummary(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

