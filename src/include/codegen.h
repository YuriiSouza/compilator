#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

// Estrutura para representar instruções de código intermediário
typedef struct {
    char *op;        // Operação (e.g., "+", "-", "*", "=", "goto")
    char *arg1;      // Primeiro argumento
    char *arg2;      // Segundo argumento (se aplicável)
    char *result;    // Resultado
} IntermediateCode;

// Estrutura para armazenar o código intermediário
typedef struct {
    IntermediateCode *instructions;
    int count;
    int capacity;
} CodeGenerator;

// Inicializa o gerador de código
CodeGenerator* create_codegen();
void generate_intermediate_code(ASTNode* root, CodeGenerator* cg);
void print_codegen_node(CodeGenerator* cg);
void print_intermediate_code(CodeGenerator* cg);
void generate_assembly_code(CodeGenerator* cg, const char* output_file);

#endif
