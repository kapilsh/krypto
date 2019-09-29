// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class InstrumentResponse extends Table {
  public static InstrumentResponse getRootAsInstrumentResponse(ByteBuffer _bb) { return getRootAsInstrumentResponse(_bb, new InstrumentResponse()); }
  public static InstrumentResponse getRootAsInstrumentResponse(ByteBuffer _bb, InstrumentResponse obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public InstrumentResponse __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public Instrument instruments(int j) { return instruments(new Instrument(), j); }
  public Instrument instruments(Instrument obj, int j) { int o = __offset(4); return o != 0 ? obj.__assign(__indirect(__vector(o) + j * 4), bb) : null; }
  public int instrumentsLength() { int o = __offset(4); return o != 0 ? __vector_len(o) : 0; }

  public static int createInstrumentResponse(FlatBufferBuilder builder,
      int instrumentsOffset) {
    builder.startObject(1);
    InstrumentResponse.addInstruments(builder, instrumentsOffset);
    return InstrumentResponse.endInstrumentResponse(builder);
  }

  public static void startInstrumentResponse(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addInstruments(FlatBufferBuilder builder, int instrumentsOffset) { builder.addOffset(0, instrumentsOffset, 0); }
  public static int createInstrumentsVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startInstrumentsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endInstrumentResponse(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

