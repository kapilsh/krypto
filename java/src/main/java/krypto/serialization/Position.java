// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Position extends Struct {
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; }
  public Position __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long securityId() { return bb.getLong(bb_pos + 0); }
  public long position() { return bb.getLong(bb_pos + 8); }

  public static int createPosition(FlatBufferBuilder builder, long securityId, long position) {
    builder.prep(8, 16);
    builder.putLong(position);
    builder.putLong(securityId);
    return builder.offset();
  }
}

