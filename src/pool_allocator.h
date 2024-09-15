#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include "base.h"
#include <string.h>

typedef struct pool_free_node_t {
  struct pool_free_node_t *next;
} pool_free_node_t;

typedef struct {
  byte* buf;
  size_t buf_len;
  size_t chunk_size;

  pool_free_node_t* head;
} pool_allocator_t;

void pool_allocator_init(pool_allocator_t* pool, void* buffer, size_t buffer_size, size_t chunk_size, size_t chunk_alignment);
void pool_allocator_free_all(pool_allocator_t* pool);
void* pool_alloc(pool_allocator_t* pool);
void pool_free(pool_allocator_t* pool, void* ptr);

#endif // POOL_ALLOCATOR_H
