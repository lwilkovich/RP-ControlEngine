#include "../../../Source/Library/Utils/MessageBuffer.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace Engine;

TEST(MessageBuffer, allocateMemory) {
    size_t size = 500;
    MessageBuffer buffer;
    buffer.Allocate(size); 
    ASSERT_EQ(buffer.getCapacity(), size);
}

TEST(MessageBuffer, cursorForwardLengthSet) {
    size_t size = 500;
    MessageBuffer buffer;
    buffer.Allocate(size); 

    MessageCursor c = buffer.getMessageCursor();

    ASSERT_EQ(buffer.getLength(), c.getForwardLength());
}

TEST(MessageBuffer, cursorBackwardLengthSet) {
    size_t size = 500;
    MessageBuffer buffer;
    buffer.Allocate(size); 

    MessageCursor c = buffer.getMessageCursor();

    ASSERT_EQ(0, c.getBackwardLength());
}

TEST(MessageBuffer, cursorppOperator) {
    size_t size = 500;
    MessageBuffer buffer;
    buffer.Allocate(size); 

    MessageCursor c = buffer.getMessageCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    c++;

    ASSERT_EQ(pLen-=1, c.getForwardLength());
    ASSERT_EQ(nLen+=1, c.getBackwardLength());
}

TEST(MessageBuffer, cursormmOperator) {
    size_t size = 500;
    MessageBuffer buffer;
    buffer.Allocate(size); 

    MessageCursor c = buffer.getMessageCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    c--;

    ASSERT_EQ(pLen+=1, c.getForwardLength());
    ASSERT_EQ(nLen-=1, c.getBackwardLength());
}

TEST(MessageBuffer, cursorJumpForward) {
    size_t size = 500;
    MessageBuffer buffer;
    buffer.Allocate(size); 

    MessageCursor c = buffer.getMessageCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    size_t jumpAmount = 5;
    c.jumpForward(jumpAmount);

    ASSERT_EQ(pLen-=jumpAmount, c.getForwardLength());
    ASSERT_EQ(nLen+=jumpAmount, c.getBackwardLength());
}

TEST(MessageBuffer, cursorJumpBackward) {
    size_t size = 500;
    MessageBuffer buffer;
    buffer.Allocate(size); 

    MessageCursor c = buffer.getMessageCursor();
    size_t pLen = c.getForwardLength();
    size_t nLen = c.getBackwardLength();
    size_t jumpAmount = 5;
    size_t jumpAmountPartTwo = 2;
    c.jumpForward(jumpAmount);
    c.jumpBackward(jumpAmountPartTwo);

    ASSERT_EQ(pLen-=(jumpAmount-jumpAmountPartTwo), c.getForwardLength());
    ASSERT_EQ(nLen+=(jumpAmount-jumpAmountPartTwo), c.getBackwardLength());
}