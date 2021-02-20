#ifndef __MESSAGEBUFFER_H__
#define __MESSAGEBUFFER_H__

#include <queue>
#include <string>
#include <memory>
#include <malloc.h>
#include <stdlib.h>
#include <iostream>
#include "../Utils/_String.h"
#include "../Utils/TcpSocketClient.h"
#include "../Utils/_Queue.h"

#define mallocError " *** Failure Allocating Memory ***"
#define cursorppOOB " *** Cursor Operator++ OOB ***"
#define cursormmOOB " *** Cursor Operator-- OOB ***"

namespace Engine {
       class MessageCursor {
           private:
            std::string TAG = "MessageCursor";
            int8_t* ptrMessageBuffer;
           public:
            size_t forwardLength;
            size_t backwardLength;
            MessageCursor() {}
            MessageCursor(int8_t* ptr, size_t cap) {
                ptrMessageBuffer = ptr;
                forwardLength = cap;
                backwardLength = 0;
            }
            size_t getForwardLength() {
                return forwardLength;
            }
            size_t getBackwardLength() {
                return backwardLength;
            }
            void operator++(int) {
                if (forwardLength-1 > 0) {
                    ptrMessageBuffer += 1;
                    forwardLength -= 1;
                    backwardLength += 1;
                }
                else {
                    throw cursorppOOB;
                }
            }
            void operator--(int) {
                if (backwardLength-1 > 0) {
                    ptrMessageBuffer -= 1;
                    forwardLength += 1;
                    backwardLength -= 1;
                }
                else {
                    throw cursormmOOB;
                }
            }
            void jumpForward(size_t jumpAmount) {
                if (forwardLength >= jumpAmount) {
                    ptrMessageBuffer += jumpAmount;
                    forwardLength -= jumpAmount;
                    backwardLength += jumpAmount;
                }
                else {
                    throw cursorppOOB;
                }
            }
            void jumpBackward(size_t jumpAmount) {
                if (backwardLength >= jumpAmount) {
                    ptrMessageBuffer += jumpAmount;
                    forwardLength += jumpAmount;
                    backwardLength -= jumpAmount;
                }
                else {
                    throw cursorppOOB;
                }
            }
            void jump(size_t jumpAmount) {
                if (jumpAmount > 0) {
                    jumpForward(jumpAmount);
                }
                else if (jumpAmount < 0) {
                    jumpBackward(jumpAmount * -1);
                }
            }
       };

       class MessageBuffer {
           private:
            std::string TAG = "MessageBuffer";

            int8_t* allocatedBuffer;
            size_t capacity;
            size_t length;
            MessageCursor messageCursor;

            bool alreadyAllocatedBuffer = false;
           
            void deallocate(int8_t *ptr) {
                int8_t* allocated = NULL;
                if (ptr) {
                    allocated = ptr;
                }
                if (allocated) {
                    free(allocated);
                    *ptr = (int8_t)NULL;
                }
            }
            const std::string &getTag() { return TAG; }
            std::string getDesc() { return ""; }
            size_t _SYSTEM_getCapacity() {
                return malloc_usable_size(allocatedBuffer);
            }
            void iniMessageCursor() {
                messageCursor = MessageCursor(allocatedBuffer, capacity);
            }
           public:
            int Allocate(size_t size) {
                if (!alreadyAllocatedBuffer) {
                    try {
                        allocatedBuffer = (int8_t*)malloc(size);
                        size_t realUsable = _SYSTEM_getCapacity();
                        SYSTEM(getTag(), getDesc(), stringbuilder() << "Verifying MessageBuffer malloc: " << realUsable << " >= " << size);
                        if (!(realUsable >= size)) {
                            throw mallocError;
                        }
                        capacity = size;
                        length = size;
                        iniMessageCursor();
                        alreadyAllocatedBuffer = true;
                        return 0;
                    }
                    catch (const std::exception& e) {
                        ERROR(getTag(), getDesc(), e.what());
                        return 1;
                    }
                }
            }
            size_t getCapacity() {
                return capacity;
            }
            size_t getLength() {
                return length;
            }
            MessageCursor getMessageCursor() {
                return messageCursor;
            }
            ~MessageBuffer() {
                deallocate(allocatedBuffer);
            }
       };
}
#endif