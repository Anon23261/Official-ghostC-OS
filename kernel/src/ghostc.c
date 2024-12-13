#include "../include/ghostc.h"
#include "../include/ghost_terminal.h"
#include <string.h>

// Initialize GhostC Runtime
GhostCRuntime* ghostc_init(size_t stack_size, size_t heap_size) {
    GhostCRuntime* runtime = (GhostCRuntime*)kmalloc(sizeof(GhostCRuntime));
    if (!runtime) return NULL;

    runtime->stack = (GhostCValue*)kmalloc(stack_size * sizeof(GhostCValue));
    runtime->heap = kmalloc(heap_size);
    runtime->symbols = NULL;
    runtime->stack_size = stack_size;
    runtime->heap_size = heap_size;
    runtime->stack_ptr = 0;
    runtime->symbol_count = 0;

    return runtime;
}

// Cleanup GhostC Runtime
void ghostc_cleanup(GhostCRuntime* runtime) {
    if (!runtime) return;

    // Free stack
    if (runtime->stack) {
        for (size_t i = 0; i < runtime->stack_ptr; i++) {
            if (runtime->stack[i].type == GHOSTC_TYPE_STRING) {
                kfree(runtime->stack[i].value.string_val);
            }
        }
        kfree(runtime->stack);
    }

    // Free heap
    if (runtime->heap) {
        kfree(runtime->heap);
    }

    // Free symbols
    if (runtime->symbols) {
        for (size_t i = 0; i < runtime->symbol_count; i++) {
            kfree(runtime->symbols[i].name);
        }
        kfree(runtime->symbols);
    }

    kfree(runtime);
}

// Simple Compiler Implementation
int ghostc_compile(const char* source, GhostCCompiler* compiler) {
    if (!source || !compiler) return -1;

    compiler->source = (char*)source;
    compiler->length = strlen(source);
    compiler->position = 0;
    compiler->error = NULL;

    // Allocate bytecode buffer (simple 1:1 mapping for now)
    compiler->bytecode = (uint8_t*)kmalloc(compiler->length);
    compiler->bytecode_size = 0;

    // Simple tokenization and bytecode generation
    while (compiler->position < compiler->length) {
        char c = source[compiler->position++];
        
        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\n') continue;

        // Basic instruction encoding
        compiler->bytecode[compiler->bytecode_size++] = (uint8_t)c;
    }

    return 0;
}

// Simple Executor
int ghostc_execute(GhostCRuntime* runtime, uint8_t* bytecode, size_t size) {
    if (!runtime || !bytecode) return -1;

    for (size_t i = 0; i < size; i++) {
        uint8_t instruction = bytecode[i];
        
        // Execute instruction (placeholder)
        switch (instruction) {
            case 'p': // print
                ghostc_print(NULL, 0);
                break;
            case 'i': // input
                ghostc_input(NULL, 0);
                break;
            default:
                // Unknown instruction
                break;
        }
    }

    return 0;
}

// Built-in Functions
GhostCValue ghostc_print(GhostCValue* args, size_t count) {
    GhostCValue result = {.type = GHOSTC_TYPE_VOID};
    
    if (count > 0 && args) {
        for (size_t i = 0; i < count; i++) {
            switch (args[i].type) {
                case GHOSTC_TYPE_STRING:
                    terminal_writestring(args[i].value.string_val);
                    break;
                case GHOSTC_TYPE_INT:
                    // Convert int to string and print
                    char buf[32];
                    itoa(args[i].value.int_val, buf, 10);
                    terminal_writestring(buf);
                    break;
                default:
                    terminal_writestring("(unprintable value)");
                    break;
            }
        }
    }
    
    terminal_writestring("\n");
    return result;
}

GhostCValue ghostc_input(GhostCValue* args, size_t count) {
    GhostCValue result = {.type = GHOSTC_TYPE_STRING};
    
    // Print prompt if provided
    if (count > 0 && args && args[0].type == GHOSTC_TYPE_STRING) {
        terminal_writestring(args[0].value.string_val);
    }

    // Simple input buffer
    char buffer[256] = {0};
    size_t pos = 0;
    
    while (1) {
        char c = keyboard_getchar();
        if (c == '\n') break;
        if (pos < 255) buffer[pos++] = c;
    }
    
    buffer[pos] = '\0';
    result.value.string_val = strdup(buffer);
    return result;
}

// Error Handling
const char* ghostc_get_error(GhostCCompiler* compiler) {
    return compiler->error ? compiler->error : "No error";
}

void ghostc_clear_error(GhostCCompiler* compiler) {
    if (compiler->error) {
        kfree(compiler->error);
        compiler->error = NULL;
    }
}
