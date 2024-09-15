#include "pool_allocator.h"

private
uintptr_t align_forward_uintptr(uintptr_t ptr, uintptr_t align) {
	uintptr_t a, p, modulo;

	ASSERT(IS_POWER_OF_TWO(align));

	a = align;
	p = ptr;
	modulo = p & (a-1);
	if (modulo != 0) {
		p += a - modulo;
	}
	return p;
}

private
size_t align_forward_size(size_t ptr, size_t align) {
	size_t a, p, modulo;

	ASSERT(IS_POWER_OF_TWO((uintptr_t)align));

	a = align;
	p = ptr;
	modulo = p & (a-1);
	if (modulo != 0) {
		p += a - modulo;
	}
	return p;
}

void pool_allocator_init(pool_allocator_t *pool,
                         void *buffer, size_t buffer_size,
                         size_t chunk_size, size_t chunk_alignment)
{
  uintptr_t initial_start = (uintptr_t) buffer;
  uintptr_t start = align_forward_uintptr(initial_start, chunk_alignment);
  buffer_size -= (size_t) (start - initial_start);

  chunk_size = align_forward_size(chunk_size, chunk_alignment);

  ASSERT(chunk_size >= sizeof(pool_free_node_t) && "chunk size too smal");

  ASSERT(buffer_size >= chunk_size && "buffer size too smal");

  pool->buf = (byte*) buffer;
  pool->buf_len = buffer_size;
  pool->chunk_size = chunk_size;
  pool->head = NULL;

  pool_allocator_free_all(pool);
}

void* pool_allocator_alloc(pool_allocator_t* pool)
{
  pool_free_node_t* node = pool->head;

  if (node == NULL) {
    ASSERT(0 && "Pool allocator has no free memory");
		return NULL;
  }

  pool->head = pool->head->next;

  return memset(node, 0, pool->chunk_size);
}

void pool_allocator_free(pool_allocator_t* pool)
{
  size_t chunk_count = pool->buf_len / pool->chunk_size;

  for (size_t i = 0; i < chunk_count; i++) {
    void* ptr = &pool->buf[i*pool->chunk_size];
    pool_free_node_t* node = (pool_free_node_t*) ptr;
    node->next = pool->head;
    pool->head = node;
  }
}
