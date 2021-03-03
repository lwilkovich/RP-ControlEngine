#include "../../../Source/Library/Utils/Buffer.h"
#include "../../../Source/Library/Utils/_Queue.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace Engine;

TEST(Buffer, allocateMemory) {
    size_t size = 500;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size);
    ASSERT_EQ(buffer->getCapacity(), size);
}

TEST(Buffer, cursorForwardLengthSet) {
    size_t size = 500;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size);

    BufferCursor c = buffer->getBufferCursor();

    ASSERT_EQ(buffer->getLength(), c.getForwardLength());
}

TEST(Buffer, cursorBackwardLengthSet) {
    size_t size = 500;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();

    ASSERT_EQ(0, c.getBackwardLength());
}

TEST(Buffer, cursorppOperator) {
    size_t size = 500;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    c++;

    ASSERT_EQ(pLen-=1, c.getForwardLength());
    ASSERT_EQ(nLen+=1, c.getBackwardLength());
}

TEST(Buffer, cursormmOperator) {
    size_t size = 500;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size);

    BufferCursor c = buffer->getBufferCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    c--;

    ASSERT_EQ(pLen+=1, c.getForwardLength());
    ASSERT_EQ(nLen-=1, c.getBackwardLength());
}

TEST(Buffer, cursorJumpForward) {
    size_t size = 500;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    size_t jumpAmount = 5;
    c.jumpForward(jumpAmount);

    ASSERT_EQ(pLen-=jumpAmount, c.getForwardLength());
    ASSERT_EQ(nLen+=jumpAmount, c.getBackwardLength());
}

TEST(Buffer, cursorJumpBackward) {
    size_t size = 500;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    size_t jumpAmount = 5;
    size_t jumpAmountPartTwo = 2;
    c.jumpForward(jumpAmount);
    c.jumpBackward(jumpAmountPartTwo);

    ASSERT_EQ(pLen-=(jumpAmount-jumpAmountPartTwo), c.getForwardLength());
    ASSERT_EQ(nLen+=(jumpAmount-jumpAmountPartTwo), c.getBackwardLength());
}

TEST(Buffer, cursorWriteByte) {
    size_t size = 5;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();

    int8_t temp = 6;
    int8_t* curLocation = c.writeByte(&temp);
    // std::cout << static_cast<int16_t>(*curLocation) << std::endl;

    ASSERT_EQ(*curLocation, temp);
    ASSERT_EQ(pLen-=1, c.getForwardLength());
    ASSERT_EQ(nLen+=1, c.getBackwardLength());
}

TEST(Buffer, cursorWriteByte_WholeBuffer) {
    size_t size = 5;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();
    for (size_t i = 0; i < size-1; i++)
    {
        size_t pLen = c.getForwardLength();
        size_t nLen = c.getBackwardLength();

        int8_t temp = i;
        int8_t* curLocation = c.writeByte(&temp);
        // std::cout << static_cast<int16_t>(*curLocation) << std::endl;
        // std::cout << c.getForwardLength() << " : " << c.getBackwardLength() << std::endl << std::endl;

        ASSERT_EQ(*curLocation, temp);
        ASSERT_EQ(pLen-=1, c.getForwardLength());
        ASSERT_EQ(nLen+=1, c.getBackwardLength());
    }

    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();

    int8_t temp = size-1;
    int8_t* curLocation = c.writeByte(&temp);
    // std::cout << static_cast<int16_t>(*curLocation) << std::endl;
    // std::cout << static_cast<int16_t>(*c.getCurrentPosition()) << std::endl;

    ASSERT_EQ(*curLocation, temp);
    ASSERT_EQ(size, c.getForwardLength());
    ASSERT_EQ(0, c.getBackwardLength());

    for (size_t i = 0; i < size; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*c.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*c.getCurrentPosition(), i);
        c++;
    }
}

TEST(Buffer, cursorWriteBytes) {
    size_t size = 5;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();

    int8_t* tempBuf = (int8_t*)malloc(size);
    int8_t* tempOriginBuf = tempBuf;
    int8_t value = 0;
    for (size_t i = 0; i < size; i++)
    {
        memcpy(tempBuf, &value,1);
        tempBuf+=1;
        value += 1;
    }
    
    c.writeBytes(tempOriginBuf, 5);

    for (size_t i = 0; i < size; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*c.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*c.getCurrentPosition(), i);
        c++;
    }

}

TEST(Buffer, cursorWriteBytes_overFlowWrapping) {
    size_t size = 5;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();

    int8_t* tempBuf = (int8_t*)malloc(7);
    int8_t* tempOriginBuf = tempBuf;
    int8_t value = 0;
    for (size_t i = 0; i < 7; i++)
    {
        memcpy(tempBuf, &value,1);
        tempBuf+=1;
        value += 1;
    }
    
    c.writeBytes(tempOriginBuf, 7);

    c = buffer->getBufferCursor();
    for (size_t i = 5; i < 7; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*c.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*c.getCurrentPosition(), i);
        c++;
    }

    for (size_t i = 2; i < 5; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*c.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*c.getCurrentPosition(), i);
        c++;
    }
}

TEST(Buffer, bufferAssignment) {
    size_t size = 5;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();

    int8_t* tempBuf = (int8_t*)malloc(size);
    int8_t* tempOriginBuf = tempBuf;
    int8_t value = 0;
    for (size_t i = 0; i < size; i++)
    {
        memcpy(tempBuf, &value,1);
        tempBuf+=1;
        value += 1;
    }
    
    c.writeBytes(tempOriginBuf, 5);

    for (size_t i = 0; i < size; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*c.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*c.getCurrentPosition(), i);
        c++;
    }
    std::shared_ptr<Buffer> bufferTwo = buffer;

    auto bufferTwoCursor = bufferTwo->getBufferCursor();
    auto bufferCursor = buffer->getBufferCursor();

    for (size_t i = 0; i < size; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*bufferCursor.getCurrentPosition()) << " " << static_cast<int16_t>(*bufferTwoCursor.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*bufferCursor.getCurrentPosition(), *bufferTwoCursor.getCurrentPosition());
        bufferCursor++;
        bufferTwoCursor++;
    }
}

TEST(Buffer, bufferQueueAssignment) {
    size_t size = 5;
    std::shared_ptr<Buffer> buffer(new Buffer());
    buffer->Allocate(size); 

    BufferCursor c = buffer->getBufferCursor();

    int8_t* tempBuf = (int8_t*)malloc(size);
    int8_t* tempOriginBuf = tempBuf;
    int8_t value = 0;
    for (size_t i = 0; i < size; i++)
    {
        memcpy(tempBuf, &value,1);
        tempBuf+=1;
        value += 1;
    }
    
    c.writeBytes(tempOriginBuf, 5);

    for (size_t i = 0; i < size; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*c.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*c.getCurrentPosition(), i);
        c++;
    }

    _Queue::Queue<std::shared_ptr<Buffer>> qBuf;
    qBuf.push(buffer);
    std::shared_ptr<Buffer> bufferTwo = qBuf.pop();

    auto bufferTwoCursor = bufferTwo->getBufferCursor();
    auto bufferCursor = buffer->getBufferCursor();

    for (size_t i = 0; i < size; i++) {
        // std::cout << "i: " << i << " " << static_cast<int16_t>(*bufferCursor.getCurrentPosition()) << " " << static_cast<int16_t>(*bufferTwoCursor.getCurrentPosition()) << std::endl; 
        ASSERT_EQ(*bufferCursor.getCurrentPosition(), *bufferTwoCursor.getCurrentPosition());
        bufferCursor++;
        bufferTwoCursor++;
    }
}