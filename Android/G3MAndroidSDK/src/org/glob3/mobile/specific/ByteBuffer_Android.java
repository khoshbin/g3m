package org.glob3.mobile.specific;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import org.glob3.mobile.generated.IByteBuffer;

public class ByteBuffer_Android extends IByteBuffer {
   
   ByteBuffer _buffer;
   int _timestamp = 0;
   
   ByteBuffer_Android(byte[] data){
      _buffer = ByteBuffer.wrap(data);
      
//      _buffer = ByteBuffer.allocateDirect(data.length);
//      _buffer.put(data);
//      _buffer.rewind();
   }
   
   public ByteBuffer_Android(int size) {
      _buffer = ByteBuffer.allocateDirect(size);
   }

   @Override
   public int size() {
      return _buffer.capacity();
   }


   @Override
   public int timestamp() {
      return _timestamp;
   }


   @Override
   public byte get(int i) {
      return _buffer.get(i);
   }


   @Override
   public void put(int i,
                   byte value) {
      if (_buffer.get(i) != value) {
         _buffer.put(i, value);
         _timestamp++;
       }
   }
   
   public ByteBuffer getBuffer() {
      return _buffer;
   }

}
