// buffer.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "buffer.h"

#define BUFFER_POOL_SIZE 2

typedef struct buffer_class {
    buffer_t items[BUFFER_CAPACITY];
    int read;
    int write;
    size_t size;
} Buffer;

static Buffer buffer_pool[BUFFER_POOL_SIZE];
static int next;

void buffer_init(void)
{
}

Buffer* buffer_create(void)
{
    if (next >= BUFFER_POOL_SIZE) {
        return NULL;
    }
    return &buffer_pool[next++];
}

BufferStatus buffer_put(Buffer *const buffer,
                        const buffer_t *const value)
{
    if (buffer->size == BUFFER_CAPACITY) {
        return BUFFER_FULL;
    }
    buffer->items[buffer->write] = *value;
    ++buffer->size;
    if (++buffer->write >= BUFFER_CAPACITY) {
        buffer->write = 0;
    }
    return BUFFER_OK;
}

BufferStatus buffer_get(Buffer *const buffer,
                        buffer_t *const value)
{
    if (buffer->size == 0) {
        return BUFFER_EMPTY;
    }
    *value = buffer->items[buffer->read];
    --buffer->size;
    if (++buffer->read >= BUFFER_CAPACITY) {
        buffer->read = 0;
    }
    return BUFFER_OK;
}

size_t buffer_size(Buffer *const buffer)
{
    return buffer->size;
}

