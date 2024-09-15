#include "stack_allocator.h"
#include "base.h"
#include <stdint.h>

private
size_t calculate_padding_with_header(uintptr_t ptr, uintptr_t alignment, size_t header_size)
{
  uintptr_t p, a, modulo, padding, needed_space;

  ASSERT(IS_POWER_OF_TWO(alignment));

  p = ptr;
  a = alignment;
  modulo = p & (a - 1);

  padding = 0;
  needed_space = 0;

  if (modulo != 0) {
    padding = a - modulo;
  }

  needed_space = (uintptr_t) header_size;

  if (padding < needed_space) {

    needed_space -= padding;

    if ((needed_space & (a - 1)) != 0) {
      padding += a * (1 + (needed_space / a));
    } else {
      padding += a * (needed_space / a);
    }
  }

  return (size_t) padding;
}

private
void* stack_allocator_alloc_(stack_allocator_t* stack, size_t size, size_t alignment)
{
  uintptr_t cur_addr, next_addr;
  size_t padding;

  stack_allocation_header_t *header;

  ASSERT(IS_POWER_OF_TWO(alignment));

  if (alignment > 128) {
    alignment = 128;
  }

  cur_addr = (uintptr_t)stack->buf + (uintptr_t)stack->cur_offset;
  padding = calculate_padding_with_header(cur_addr, (uintptr_t)alignment, sizeof(stack_allocation_header_t));
  if (stack->cur_offset + padding + size > stack->buf_len) {
    return NULL;
  }

  stack->prev_offset = stack->cur_offset;
  stack->cur_offset += padding;

  next_addr = cur_addr + (uintptr_t) padding;
  header = (stack_allocation_header_t*) (next_addr - sizeof(stack_allocation_header_t));
  header->padding = (u8) padding;
  stack->cur_offset += size;

  return memset((void*)next_addr, 0, size);
}

private
void* stack_allocator_resize_(stack_allocator_t* stack, void* ptr, size_t old_size, size_t new_size, size_t alignment)
{
  if (ptr == NULL) {
    return stack_allocator_alloc_(stack, new_size, alignment);
  } else if (new_size == 0) {
    stack_allocator_free(stack, ptr);
  } else {
    uintptr_t start, end, cur_addr;
    size_t min_size = old_size < new_size ? old_size : new_size;
    void* new_ptr;

    start = (uintptr_t) stack->buf;
    end = start + (uintptr_t) stack->buf_len;
    cur_addr = (uintptr_t) ptr;

    if (!(start <= cur_addr && cur_addr < end)) {
      ASSERT(0 && "Address passed to allocator is out of bounds (resize)");
      return NULL;
    }

    if (cur_addr >= start + (uintptr_t)stack->cur_offset) {
      return NULL;
    }

    if (old_size == new_size) {
      return ptr;
    }

    new_ptr = stack_allocator_alloc_(stack, new_size, alignment);
    memmove(new_ptr, ptr, min_size);
    return new_ptr;
  }

  return NULL;
}


void* stack_allocator_alloc(stack_allocator_t* stack, size_t size) {
  return stack_allocator_alloc_(stack, size, DEFAULT_ALIGNMENT);
}

void*stack_allocator_resize(stack_allocator_t *stack, void *ptr, size_t old_size, size_t new_size)
{
  return stack_allocator_resize_(stack, ptr, old_size, new_size, DEFAULT_ALIGNMENT);
}

void stack_allocator_init(stack_allocator_t* stack, void* buffer, size_t buffer_size)
{
  stack->buf = (byte*) buffer;
  stack->buf_len = buffer_size;
  stack->prev_offset = 0;
  stack->cur_offset = 0;
}

void stack_allocator_free(stack_allocator_t* stack, void* ptr)
{
  if (ptr != NULL) {
    uintptr_t start, end, cur_addr;
    stack_allocation_header_t *header;
    size_t prev_offset;

    start = (uintptr_t) stack->buf;
    end = start + (uintptr_t) stack->buf_len;
    cur_addr = (uintptr_t) ptr;

    if (!(start <= cur_addr && cur_addr < end)) {
      ASSERT(0 && "Address passed to allocator is out of bounds (free)");
      return;
    }

    if (cur_addr >= start+(uintptr_t)stack->cur_offset) {
      return;
    }

    header = (stack_allocation_header_t*) (cur_addr - sizeof(stack_allocation_header_t));
    prev_offset = (size_t) (cur_addr - (uintptr_t)header->padding - start);

    if (prev_offset != header->prev_offset) {
      ASSERT(0 && "Free out of stack order");
      return;
    }


    stack->cur_offset = prev_offset;
    stack->prev_offset = header->prev_offset;
  }
}

void stack_allocator_free_all(stack_allocator_t* stack)
{
  stack->cur_offset = 0;
  stack->prev_offset = 0;
}
