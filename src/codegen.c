#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/codegen.h"

// Cria um gerador de código
CodeGenerator* create_codegen() {
    CodeGenerator* cg = (CodeGenerator*)malloc(sizeof(CodeGenerator));
    cg->instructions = (IntermediateCode*)malloc(10 * sizeof(IntermediateCode));
    cg->count = 0;
    cg->capacity = 10;
    return cg;
}

// Imprime os detalhes do nó CodeGenerator
void print_codegen_node(CodeGenerator* cg) {
    if (!cg) {
        printf("CodeGenerator node is NULL\n");
        return;
    }

    printf("CodeGenerator Details:\n");
    printf("Count: %d\n", cg->count);
    printf("Capacity: %d\n", cg->capacity);
    printf("Instructions:\n");
    
    for (int i = 0; i < cg->count; i++) {
        printf("Instruction %d:\n", i);
        printf("  Operation: %s\n", cg->instructions[i].op ? cg->instructions[i].op : "NULL");
        printf("  Arg1: %s\n", cg->instructions[i].arg1 ? cg->instructions[i].arg1 : "NULL");
        printf("  Arg2: %s\n", cg->instructions[i].arg2 ? cg->instructions[i].arg2 : "NULL");
        printf("  Result: %s\n", cg->instructions[i].result ? cg->instructions[i].result : "NULL");
        printf("-------------------\n");
    }
}


// Adiciona uma instrução ao gerador de código
void add_instruction(CodeGenerator* cg, const char* op, const char* arg1, const char* arg2, const char* result) {
    if (cg->count == cg->capacity) {
        cg->capacity *= 2;
        cg->instructions = (IntermediateCode*)realloc(cg->instructions, cg->capacity * sizeof(IntermediateCode));
    }

    IntermediateCode* instr = &cg->instructions[cg->count++];
    instr->op = strdup(op);
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->result = result ? strdup(result) : NULL;

    printf("Added instruction:\n");
    printf("  Operation: %s\n", op);
    printf("  Arg1: %s\n", arg1 ? arg1 : "NULL");
    printf("  Arg2: %s\n", arg2 ? arg2 : "NULL");
    printf("  Result: %s\n", result ? result : "NULL");
    printf("-------------------\n");
}
// Gera código intermediário a partir da AST
void generate_intermediate_code(ASTNode* root, CodeGenerator* cg) {
    if (!root) return;

    printf("DEBUG: Processing node type %d\n", root->type);  // Adicione essa linha para verificar o tipo do nó

    char temp[16];
    switch (root->type) {
        case NODE_PROGRAM:
            generate_intermediate_code(root->right, cg);
            generate_intermediate_code(root->left, cg);
            break;

        case NODE_DECLARATION:
            // Para declarações, apenas reservamos espaço para a variável
            sprintf(temp, "t%d", cg->count++);
            add_instruction(cg, "DECL", root->value, NULL, temp);
            root->temp = strdup(temp);
            break;

        case NODE_ASSIGNMENT:
            if (root->right == NULL) {
                printf("DEBUG: root->right é NULL\n");
            } else {
                printf("DEBUG: root->right está sendo processado\n");
            }
            generate_intermediate_code(root->right, cg);
            add_instruction(cg, "MOV", root->right->temp, NULL, root->left->value);
            break;

        case NODE_RETURN:
            generate_intermediate_code(root->left, cg);
            add_instruction(cg, "RET", root->left->temp, NULL, NULL);
            break;

        case NODE_EXPRESSION:
            generate_intermediate_code(root->left, cg);
            generate_intermediate_code(root->right, cg);
            sprintf(temp, "t%d", cg->count++);
            root->temp = strdup(temp);
            add_instruction(cg, root->value, root->left->temp, root->right->temp, root->temp);
            break;

        case NODE_TERM:
            generate_intermediate_code(root->left, cg);
            generate_intermediate_code(root->right, cg);
            sprintf(temp, "t%d", cg->count++);
            root->temp = strdup(temp);
            add_instruction(cg, root->value, root->left->temp, root->right->temp, root->temp);
            break;

        case NODE_FACTOR:
            generate_intermediate_code(root->left, cg);
            sprintf(temp, "t%d", cg->count++);
            root->temp = strdup(temp);
            add_instruction(cg, root->value, root->left->temp, NULL, root->temp);
            break;

        case NODE_IDENTIFIER:
            sprintf(temp, "t%d", cg->count++);
            root->temp = strdup(temp);
            add_instruction(cg, "LOAD", root->value, NULL, temp);
            break;

        case NODE_NUMBER:
            sprintf(temp, "t%d", cg->count++);
            root->temp = strdup(temp);
            add_instruction(cg, "LOADI", root->value, NULL, temp);
            break;

        default:
            break;
    }
}

// Imprime o código intermediário
void print_intermediate_code(CodeGenerator* cg) {
    for (int i = 0; i < cg->count; i++) {
        IntermediateCode* instr = &cg->instructions[i];
        printf("%s = %s", instr->result, instr->arg1 ? instr->arg1 : "");
        if (instr->op) printf(" %s", instr->op);
        if (instr->arg2) printf(" %s", instr->arg2);
        printf("\n");
    }
}

// Gera código final em Assembly
void generate_assembly_code(CodeGenerator* cg, const char* output_file) {
    FILE* out = fopen(output_file, "w");
    if (!out) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }

    if (!cg || !cg->instructions) {
      printf("Erro: CodeGenerator ou instructions não inicializado.\n");
      return;
    }

    // fprintf(out, "section .data\n");
    // fprintf(out, "section .bss\n");
    // fprintf(out, "section .text\n");
    // fprintf(out, "global _start\n");

    fprintf(out, "_start:\n");
    for (int i = 0; i < cg->count; i++) {
        IntermediateCode* instr = &cg->instructions[i];

        if (instr->op) {
            if (strcmp(instr->op, "DECL") == 0 && instr->result) {
                fprintf(out, "    %s resd 1\n", instr->result);
            } else if (strcmp(instr->op, "MOV") == 0 && instr->result && instr->arg1) {
                fprintf(out, "    mov [%s], %s\n", instr->result, instr->arg1);
            } else if (strcmp(instr->op, "+") == 0 && instr->arg1 && instr->arg2 && instr->result) {
                fprintf(out, "    mov eax, [%s]\n", instr->arg1);
                fprintf(out, "    add eax, [%s]\n", instr->arg2);
                fprintf(out, "    mov [%s], eax\n", instr->result);
            } else if (strcmp(instr->op, "-") == 0 && instr->arg1 && instr->arg2 && instr->result) {
                fprintf(out, "    mov eax, [%s]\n", instr->arg1);
                fprintf(out, "    sub eax, [%s]\n", instr->arg2);
                fprintf(out, "    mov [%s], eax\n", instr->result);
            } else if (strcmp(instr->op, "*") == 0 && instr->arg1 && instr->arg2 && instr->result) {
                fprintf(out, "    mov eax, [%s]\n", instr->arg1);
                fprintf(out, "    imul eax, [%s]\n", instr->arg2);
                fprintf(out, "    mov [%s], eax\n", instr->result);
            } else if (strcmp(instr->op, "/") == 0 && instr->arg1 && instr->arg2 && instr->result) {
                fprintf(out, "    mov eax, [%s]\n", instr->arg1);
                fprintf(out, "    cdq\n");
                fprintf(out, "    idiv dword [%s]\n", instr->arg2);
                fprintf(out, "    mov [%s], eax\n", instr->result);
            } else if (strcmp(instr->op, "RET") == 0 && instr->arg1) {
                fprintf(out, "    mov eax, [%s]\n", instr->arg1);
                fprintf(out, "    mov ebx, 0\n");
                fprintf(out, "    int 0x80\n");
            } else if (strcmp(instr->op, "LOAD") == 0 && instr->result && instr->arg1) {
                fprintf(out, "    mov eax, [%s]\n", instr->arg1);
                fprintf(out, "    mov [%s], eax\n", instr->result);
            } else if (strcmp(instr->op, "LOADI") == 0 && instr->result && instr->arg1) {
                fprintf(out, "    mov eax, %s\n", instr->arg1);
                fprintf(out, "    mov [%s], eax\n", instr->result);
            }
        }
    }

    // fprintf(out, "    mov eax, 1\n");
    // fprintf(out, "    xor ebx, ebx\n");
    // fprintf(out, "    int 0x80\n");

    fclose(out);
    printf("Código Assembly gerado em: %s\n", output_file);
}
