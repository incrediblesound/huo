#include "array.h"
#include "../config.h"
#include "../base_util.h"

void array_copy_to(struct Value_array *to, struct Value_array *from) {
    if (from->size == 0) {
        RESIZE(to->values, 0);
        to->size = 0;
    } else {
        if (to->size != from->size) {
            RESIZE(to->values, from->size);
            to->size = from->size;
        }
        for(int i = 0; i < from->size; i++){
            to->values[i] = value_copy_heap(from->values[i]);
        }
    }
}

struct Value_array array_copy_stack(struct Value_array * from){
    struct Value_array array;
    array.size = from->size;
    array.values = ARR_MALLOC(array.size, array.values[0]);
    for(int i = 0; i < from->size; i++){
        array.values[i] = value_copy_heap(from->values[i]);
    }
    return array;
}

struct Value_array *array_copy_heap(struct Value_array * from){
    struct Value_array * array = malloc_or_die(sizeof(struct Value_array));
    array->size = from->size;
    array->values = ARR_MALLOC(array->size, array->values[0]);
    for(int i = 0; i < from->size; i++){
        array->values[i] = value_copy_heap(from->values[i]);
    }
    return array;
}

struct Value_array *array_concat_heap(struct Value_array *a, struct Value_array *b) {
    struct Value_array *temp = array_copy_heap(a);
    array_concat_to(temp, b);
    return temp;
}

void array_concat_to(struct Value_array *to, struct Value_array *from) {
    int len = to->size;
    to->size += from->size;
    RESIZE(to->values, to->size);
    for(int l = 0; l < from->size; l++){
        to->values[l+len] = value_copy_heap(from->values[l]);
    }
}

bool array_matches(struct Value_array *base, struct Value_array *compare){
    if(base->size != compare->size){
        return false;
    }
    for (int i = 0; i < base->size; i++) {
        if (!value_equals_shallow(base->values[i], compare->values[i]))
            return false;
    }
    return true;
}