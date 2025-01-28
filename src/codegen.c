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
}

// Gera código intermediário a partir da AST
void generate_intermediate_code(ASTNode* root, CodeGenerator* cg) {
    if (!root) return;

    char temp[16];
    switch (root->type) {
        case NODE_PROGRAM:
            generate_intermediate_code(root->left, cg);
            generate_intermediate_code(root->right, cg);
            break;
        case NODE_ASSIGNMENT:
            generate_intermediate_code(root->right, cg);
            add_instruction(cg, "=", root->right->temp, NULL, root->left->value);
            break;
        case NODE_EXPRESSION:
            generate_intermediate_code(root->left, cg);
            generate_intermediate_code(root->right, cg);
            sprintf(temp, "t%d", cg->count);
            root->temp = strdup(temp);
            add_instruction(cg, root->value, root->left->temp, root->right->temp, root->temp);
            break;
        case NODE_NUMBER:
        case NODE_IDENTIFIER:
            sprintf(temp, "t%d", cg->count);
            root->temp = strdup(temp);
            add_instruction(cg, "=", root->value, NULL, root->temp);
            break;
        default:
            break;
    }
}

// Imprime o código intermediário
void print_intermediate_code(CodeGenerator* cg) {
    for (int i = 0; i < cg->count; i++) {
        IntermediateCode* instr = &cg->instructions[i];
        printf("%s = %s", instr->result, instr->arg1);
        if (instr->op) printf(" %s %s", instr->op, instr->arg2);
        printf("\n");
    }
}

// Gera o código final (em C, por exemplo)
void generate_target_code(CodeGenerator* cg, const char* output_file) {
    FILE* out = fopen(output_file, "w");
    if (!out) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }

    fprintf(out, "#include <stdio.h>\n\nint main() {\n");

    for (int i = 0; i < cg->count; i++) {
        IntermediateCode* instr = &cg->instructions[i];

        if (instr->arg2) { // Operação binária
            fprintf(out, "    int %s = %s %s %s;\n", instr->result, instr->arg1, instr->op, instr->arg2);
        } else { // Atribuição simples
            fprintf(out, "    int %s = %s;\n", instr->result, instr->arg1);
        }
    }

    fprintf(out, "    return 0;\n}\n");
    fclose(out);

    printf("Código final gerado em: %s\n", output_file);
}
