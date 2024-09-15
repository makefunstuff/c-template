#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "base.h"

typedef struct {
  usize cur_offset;
  usize prev_offset;
  usize buf_len;
  byte* buf;
} arena_t;

void arena_init(arena_t* arena, void* buffer, usize buf_len);
void arena_destroy(arena_t* arena);
void arena_reset(arena_t* arena);
void* arena_alloc(arena_t* arena, usize bytes);

#endif // ARENA_H
