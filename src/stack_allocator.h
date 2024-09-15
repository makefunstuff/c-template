#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include "base.h"
#include <string.h>

typedef struct {
  byte* buf;
  size_t buf_len;
  size_t prev_offset;
  size_t cur_offset;
} stack_allocator_t;

typedef struct {
  size_t prev_offset;
  size_t padding;
} stack_allocation_header_t;

void stack_allocator_init(stack_allocator_t* stack, void* buffer, size_t buffer_size);
void* stack_allocator_alloc(stack_allocator_t* stack, size_t size);
void* stack_allocator_resize(stack_allocator_t* stack, void* ptr, size_t old_size, size_t new_size);
void stack_allocator_free(stack_allocator_t* stack, void* ptr);
void stack_allocator_free_all(stack_allocator_t* stack);

#endif // STACK_ALLOCATOR_H
