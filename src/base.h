#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define ERROR_EXIT(...) fprintf(stderr, __VA_ARGS__); exit(1)
#define ERROR_RETURN(R, ...) fprintf(stderr, __VA_ARGS__); return R

#define global static
#define private static
#define local_persist static

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef unsigned char byte;

#define STRINGIFY_(S) #S
#define STRINGIFY(S) STRINGIFY_(S)

#define CONCAT_(A, B) A##B
#define CONCAT(A, B) CONCAT_(A, B)

#define ARRAY_LEN(a) (sizeof(a) / sizeof(*(a)))

#define STMT(S) do{ S }while(0)
#define ASSERT_BRK() (*(volatile int*)0 = 0)
#define ASSERT(c) STMT( if (!(c)){ ASSERT_BRK(); } )
#define STATIC_ASSERT(c,l) typedef u8 CONCAT(l,__LINE__) [(c)?1:-1]


// sizes
#define KB(x) ((u64)(x) << 10)  // KB = 2^10 bytes
#define MB(x) ((u64)(x) << 20)  // MB = 2^20 bytes
#define GB(x) ((u64)(x) << 30)  // GB = 2^30 bytes

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif

#define IS_POWER_OF_TWO(x) (((x) & ((x) - 1)) == 0)

#endif // BASE_H
