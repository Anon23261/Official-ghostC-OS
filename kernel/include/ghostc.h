#ifndef __GHOSTC_H
#define __GHOSTC_H

#include <stdint.h>
#include <stddef.h>
#include "system.h"

// GhostC Language Types
typedef enum {
    GHOSTC_TYPE_VOID,
    GHOSTC_TYPE_INT,
    GHOSTC_TYPE_FLOAT,
    GHOSTC_TYPE_STRING,
    GHOSTC_TYPE_BOOL,
    GHOSTC_TYPE_ARRAY,
    GHOSTC_TYPE_STRUCT,
    GHOSTC_TYPE_FUNCTION
} GhostCType;

// GhostC Value Structure
typedef struct GhostCValue {
    GhostCType type;
    union {
        int64_t int_val;
        double float_val;
        char* string_val;
        uint8_t bool_val;
        struct {
            void* data;
            size_t length;
            GhostCType elem_type;
        } array;
        struct {
            char* name;
            void* data;
        } struct_val;
        struct {
            void* (*func_ptr)(struct GhostCValue*, size_t);
            size_t param_count;
        } function;
    } value;
} GhostCValue;

// GhostC Runtime Environment
typedef struct {
    GhostCValue* stack;
    size_t stack_size;
    size_t stack_ptr;
    void* heap;
    size_t heap_size;
    struct {
        char* name;
        GhostCValue value;
    }* symbols;
    size_t symbol_count;
} GhostCRuntime;

// GhostC Compiler Structure
typedef struct {
    char* source;
    size_t length;
    size_t position;
    uint8_t* bytecode;
    size_t bytecode_size;
    char* error;
} GhostCCompiler;

// Function Declarations
GhostCRuntime* ghostc_init(size_t stack_size, size_t heap_size);
void ghostc_cleanup(GhostCRuntime* runtime);
int ghostc_compile(const char* source, GhostCCompiler* compiler);
int ghostc_execute(GhostCRuntime* runtime, uint8_t* bytecode, size_t size);
GhostCValue ghostc_eval(GhostCRuntime* runtime, const char* expression);

// Built-in Functions
GhostCValue ghostc_print(GhostCValue* args, size_t count);
GhostCValue ghostc_input(GhostCValue* args, size_t count);
GhostCValue ghostc_array_new(GhostCValue* args, size_t count);
GhostCValue ghostc_array_get(GhostCValue* args, size_t count);
GhostCValue ghostc_array_set(GhostCValue* args, size_t count);

// Error Handling
const char* ghostc_get_error(GhostCCompiler* compiler);
void ghostc_clear_error(GhostCCompiler* compiler);

#endif
