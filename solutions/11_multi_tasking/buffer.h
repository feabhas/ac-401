// buffer.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef BUFFER_H
#define BUFFER_H
#include <stddef.h>

#define BUFFER_CAPACITY 8

typedef struct buffer_class Buffer;

typedef int buffer_t;

typedef enum buffer_status {
    BUFFER_OK, BUFFER_FULL, BUFFER_EMPTY
} BufferStatus;

void buffer_init(void);
Buffer* buffer_create(void);
BufferStatus buffer_put(Buffer *const buffer,
                        const buffer_t *const value);
BufferStatus buffer_get(Buffer *const buffer,
                        buffer_t *const value);
size_t buffer_size(Buffer *const buffer);

#endif
