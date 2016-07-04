#ifndef _BASE_UTIL_H
#define _BASE_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structures/structures.h"
#include "config.h"

#define WARN_ONCE(...) _WARN_ONCE(warn_once_##__COUNTER__, __VA_ARGS__)
#define _WARN_ONCE(UNIQ, ...) do {\
    static bool UNIQ = false;\
    if (!UNIQ) {\
        UNIQ = true;\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
    }\
} while (0)

#ifdef _Static_assert
#define STATIC_ASSERT(cond,msg) CTA2(cond, msg)
#define CTA2(cond, msg) _Static_assert(cond,msg)
#else
#define CTASTR2(pre,post) pre ## post
#define CTASTR(pre,post) CTASTR2(pre,post)
#define STATIC_ASSERT(cond,msg) \
    typedef struct { int CTASTR(static_assertion_failed_,msg) : !!(cond); } \
        CTASTR(static_assertion_failed_,__COUNTER__)
#endif

/* Macro because it makes printf errors easier to detect at compile time */
/* Minor hack because c99 doesn't allow zero-length varargs for macros */
#define ERROR(...) do {\
    fprintf(stderr, "Error at %s:%s:%i: ", __FILE__, __func__, __LINE__);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(1);\
} while (0);

#define ARR_MALLOC(num_elem, elem_val) malloc(arr_malloc_size((num_elem), sizeof(elem_val)))

// Would be a function, but then So. Much. Casting.
#define RESIZE(ptr_to_arr, new_len) do {\
    if ((new_len) == 0) {\
        free(ptr_to_arr);\
        (ptr_to_arr) = NULL;\
    } else {\
        (ptr_to_arr) = realloc((ptr_to_arr), arr_malloc_size((new_len), sizeof((ptr_to_arr)[0])));\
        if ((ptr_to_arr) == NULL) {\
            ERROR("Malloc failure!");\
        }\
    }\
} while (0)

size_t arr_malloc_size(size_t num, size_t size);
char *strdup(const char *str);
int string_contains(char ch, struct String* string);
void printTree(struct Tree *tree);
struct Tree * duplicate_tree(struct Tree * a);
void copy_array(struct Value * a, struct Value_array * b);
void make_scope(struct Scopes * scopes);
void sub_vars(struct Value *v, struct Scopes *scopes, int max_depth);

#endif
