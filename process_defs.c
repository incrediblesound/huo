#include <stdlib.h>
#include "base_util.h"

struct Map * make_args_map(struct Tree * ast, struct Tree_map * defined, int idx){
    struct Map * arguments = malloc(sizeof(struct Map));
    for(int i = 0; i < ast->size; i++){
        struct Keyval * store = malloc(sizeof(struct Keyval));
        struct Value * key = malloc(sizeof(struct Value));
        struct Value * val = malloc(sizeof(struct Value));
        copy_value(key, &defined->trees[idx]->children[i+1]->content);
        copy_value(val, &ast->children[i]->content);
        arguments->members[i] = store;
        arguments->members[i]->key = key;
        arguments->members[i]->val = val;
        arguments->size++;
    }
    return arguments;
}

struct Tree * populate_args(struct Map * arguments, struct Tree * ast){
    if(ast->type == 'k'){
        for(int i = 0; i < arguments->size; i++){
            if(string_matches(arguments->members[i]->key->data.str, ast->content.data.str)){
                copy_value(&ast->content, arguments->members[i]->val);
            }
        }
    } else {
        for(int i = 0; i < ast->size; i++){
            populate_args(arguments, ast->children[i]);
        }
    }
    return ast;
}

struct Tree * get_defined_body(struct Tree * function){
    int check = 1;
    int index = 1;
    while(check){
        if(!function->children[index]->size){
            index++;
        } else {
            check = 0;
            function = function->children[index];
        }
    }
    return function;
}

int is_defined_func(struct Tree_map * defined, struct String key){
    for(int i = 0; i < defined->size; i++){
        if(string_matches(*defined->names[i], key)){
            return i;
        }
    }
    return -1;
}