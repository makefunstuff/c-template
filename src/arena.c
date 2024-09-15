#include "arena.h"

// NOTE this function calculates alignment and is being called before allocating memory
// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/#memory-alignment stolen from here
private
uptr align_forward(uptr ptr, usize alignment)
{
  uptr p, a, modulo;

  ASSERT(IS_POWER_OF_TWO(alignment));

  p = ptr;
  a = (uptr) alignment;

  // NOTE fast modulo operation, have no clue how does it work
  // TODO spend some time to understand this, for now I consider this as black magic I am dummily copypasted
  //      from the abovementioned article.
  modulo = p & (a - 1);

  if (modulo != 0) {
    p += a - modulo;
  }

  return p;
}

private
void* arena_alloc_(arena_t* arena, usize size, usize alignment)
{
  uptr curr_ptr = (uptr)arena->buf + (uptr)arena->cur_offset;
  uptr offset = align_forward(curr_ptr, alignment);

  offset -= (uptr)arena->buf;

  if (offset + size <= arena->buf_len) {
    void* ptr = &arena->buf[offset];
    arena->prev_offset = offset;
    arena->cur_offset = offset + size;

    memset(ptr, 0, size);
    return ptr;
  }

  return NULL;
}

private inline
void* arena_resize_(arena_t* arena, void* old_memory, usize old_size, usize new_size, usize align)
{
  byte* old_mem = (byte*) old_memory;

  ASSERT(IS_POWER_OF_TWO(align));

  if (old_mem == NULL || old_size == 0) {
    return arena_alloc_(arena, new_size, align);
  } else if (arena->buf <= old_mem && old_mem < arena->buf+arena->buf_len) {
    if (arena->buf+arena->prev_offset == old_mem) {
      arena->cur_offset = arena->prev_offset + new_size;
      if (new_size > old_size) {
        memset(&arena->buf[arena->cur_offset], 0, new_size - old_size);
      }
      return old_memory;
    } else {
      void* new_memory = arena_alloc_(arena, new_size, align);
      usize copy_size = old_size < new_size ? old_size : new_size;
      memmove(new_memory, old_memory, copy_size);
      return new_memory;
    }
  } else {
    ASSERT(0 && "Memory is out of bounds of the buffer in this arena");
    return NULL;
  }
}

void arena_init(arena_t* arena, void* buffer, usize buf_len)
{
  arena->buf = buffer;
  arena->buf_len = buf_len;

  arena->cur_offset = 0;
  arena->prev_offset = 0;
}

void* arena_alloc(arena_t* arena, usize bytes)
{
  return arena_alloc_(arena, bytes, DEFAULT_ALIGNMENT);
}

void* arena_resize(arena_t* arena, void* old_mem, usize old_size, usize new_size)
{
  return arena_resize_(arena, old_mem, old_size, new_size, DEFAULT_ALIGNMENT);
}


void arena_reset(arena_t* arena)
{
  arena->cur_offset = 0;
}

void arena_destroy(arena_t* arena)
{
}
