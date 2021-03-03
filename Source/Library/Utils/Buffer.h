#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "_Logger.h"
#include "_Queue.h"
#include "_String.h"
#include <iostream>
#include <malloc.h>
#include <memory>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define mallocError " *** Failure Allocating Memory ***"
#define cursorppOOB " *** Cursor Operator++ OOB ***"
#define cursormmOOB " *** Cursor Operator-- OOB ***"
#define cursorJumpForwardOOB " *** Cursor JumpForward OOB ***"
#define cursorJumpBackwardOOB " *** Cursor JumpBackward OOB ***"
#define cursorWriteByteFail " *** Cursor Write Byte Failure ***"

namespace Engine {
    class BufferCursor {
      private:
        std::string TAG = "BufferCursor";
        int8_t *ptrBuffer;
        int8_t *originPtr;
        size_t forwardLength;
        size_t backwardLength;

        void wrapBuffer() {
            ptrBuffer = originPtr;
            forwardLength = backwardLength;
            backwardLength = 0;
            SYSTEM(getTag(), getDesc(), stringbuilder() << "System Noticed warpBuffer() Call...");
        }

      public:
        const std::string &getTag() { return TAG; }
        std::string getDesc() { return ""; }
        BufferCursor() {}
        BufferCursor(int8_t *ptr, size_t cap) {
            ptrBuffer = ptr;
            originPtr = ptr;
            forwardLength = cap;
            backwardLength = 0;
        }
        int8_t *getCursorData() { return ptrBuffer; }
        size_t getForwardLength() { return forwardLength; }
        size_t getBackwardLength() { return backwardLength; }
        int8_t *operator++(int) {
            try {
                if (forwardLength - 1 > 0) {
                    ptrBuffer += 1;
                    forwardLength -= 1;
                    backwardLength += 1;
                    return ptrBuffer;
                } else {
                    wrapBuffer();
                    return ptrBuffer;
                }
            } catch (const std::exception &e) {
                ERROR(getTag(), getDesc(), e.what());
                throw cursorppOOB;
            }
        }
        int8_t *operator--(int) {
            try {
                if (backwardLength - 1 > 0) {
                    ptrBuffer -= 1;
                    forwardLength += 1;
                    backwardLength -= 1;
                    return ptrBuffer;
                } else {
                    wrapBuffer();
                    return ptrBuffer;
                }
            } catch (const std::exception &e) {
                ERROR(getTag(), getDesc(), e.what());
                throw cursormmOOB;
            }
        }
        int8_t *jumpForward(size_t jumpAmount) {
            try {
                if (forwardLength >= jumpAmount) {
                    ptrBuffer += jumpAmount;
                    forwardLength -= jumpAmount;
                    backwardLength += jumpAmount;
                    return ptrBuffer;
                } else {
                    wrapBuffer();
                    return ptrBuffer;
                }
            } catch (const std::exception &e) {
                ERROR(getTag(), getDesc(), e.what());
                throw cursormmOOB;
            }
        }
        int8_t *jumpBackward(size_t jumpAmount) {
            try {
                if (backwardLength >= jumpAmount) {
                    ptrBuffer += jumpAmount;
                    forwardLength += jumpAmount;
                    backwardLength -= jumpAmount;
                    return ptrBuffer;
                } else {
                    wrapBuffer();
                }
            } catch (const std::exception &e) {
                ERROR(getTag(), getDesc(), e.what());
                throw cursormmOOB;
            }
        }
        int8_t *jump(size_t jumpAmount) {
            if (jumpAmount > 0) {
                jumpForward(jumpAmount);
            } else if (jumpAmount < 0) {
                jumpBackward(jumpAmount * -1);
            }
            return ptrBuffer;
        }
        int8_t *getCurrentPosition() { return ptrBuffer; }
        int8_t *writeByte(const int8_t *ptr) {
            try {
                void *dest = memcpy(ptrBuffer, ptr, 1);
                if (dest != ptrBuffer) {
                    throw cursorWriteByteFail;
                }
                int8_t *curBuffer = ptrBuffer;
                ptrBuffer += 1;

                forwardLength -= 1;
                backwardLength += 1;

                if (forwardLength == 0) {
                    wrapBuffer();
                }
                return curBuffer;
            } catch (const std::exception &e) {
                ERROR(getTag(), getDesc(), e.what());
                throw cursorWriteByteFail;
            }
        }
        int8_t *writeBytes(const int8_t *ptr, size_t size) {
            try {
                int8_t *curBuffer = ptrBuffer;
                for (size_t i = 0; i < size; i++) {
                    writeByte(ptr);
                    ptr += 1;
                }
                return curBuffer;
            } catch (const std::exception &e) {
                ERROR(getTag(), getDesc(), e.what());
                throw cursorWriteByteFail;
            }
        }
    };

    class Buffer {
      private:
        std::string TAG = "MessageBuffer";

        int8_t *allocatedBuffer = NULL;
        int8_t *originalAllocatedBuffer = NULL;
        size_t capacity;
        size_t length;
        BufferCursor bufferCursor;

        bool alreadyAllocatedBuffer = false;

        void deallocate() {
            if (alreadyAllocatedBuffer) {
                int8_t *allocated = NULL;
                if (originalAllocatedBuffer) {
                    allocated = originalAllocatedBuffer;
                }
                if (allocated) {
                    free(allocated);
                    originalAllocatedBuffer = NULL;
                }
            }
            alreadyAllocatedBuffer = false;
        }
        const std::string &getTag() { return TAG; }
        std::string getDesc() { return ""; }
        size_t _getCapacity() { return malloc_usable_size(allocatedBuffer); }
        void iniMessageCursor() { bufferCursor = BufferCursor(allocatedBuffer, capacity); }

      public:
        int Allocate(size_t size) {
            if (!alreadyAllocatedBuffer) {
                try {
                    originalAllocatedBuffer = (int8_t *)malloc(size);
                    allocatedBuffer = originalAllocatedBuffer;
                    size_t realUsable = _getCapacity();
                    SYSTEM(getTag(), getDesc(), stringbuilder() << "Verifying MessageBuffer malloc: " << realUsable << " >= " << size);
                    if (!(realUsable >= size)) {
                        throw mallocError;
                    }
                    capacity = size;
                    length = size;
                    iniMessageCursor();
                    alreadyAllocatedBuffer = true;
                    return 0;
                } catch (const std::exception &e) {
                    ERROR(getTag(), getDesc(), e.what());
                    return 1;
                }
            }
            return 1;
        }
        size_t getCapacity() { return capacity; }
        size_t getLength() { return length; }
        BufferCursor getBufferCursor() { return bufferCursor; }
        ~Buffer() { deallocate(); }
        Buffer() {}
        // copy constructor
        Buffer(Buffer const &b) {
            Allocate(b.length);
            memcpy(allocatedBuffer, b.allocatedBuffer, b.length);
            alreadyAllocatedBuffer = b.alreadyAllocatedBuffer;
        }
        // copy assignment
        Buffer &operator=(Buffer const &b) {
            Allocate(b.length);
            memcpy(allocatedBuffer, b.allocatedBuffer, b.length);
            alreadyAllocatedBuffer = b.alreadyAllocatedBuffer;
            return *this;
        }
    };
} // namespace Engine
#endif