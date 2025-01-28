#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/optimizer.h"

// Elimina código morto (instruções que não são usadas)
void eliminate_dead_code(CodeGenerator *cg) {
    int *used = calloc(cg->count, sizeof(int)); // Marcações para instruções usadas

    // Identifica quais variáveis temporárias são usadas
    for (int i = 0; i < cg->count; i++) {
        IntermediateCode *instr = &cg->instructions[i];
        if (instr->arg1 && instr->arg1[0] == 't') {
            for (int j = 0; j < cg->count; j++) {
                if (cg->instructions[j].result && strcmp(cg->instructions[j].result, instr->arg1) == 0) {
                    used[j] = 1;
                }
            }
        }
        if (instr->arg2 && instr->arg2[0] == 't') {
            for (int j = 0; j < cg->count; j++) {
                if (cg->instructions[j].result && strcmp(cg->instructions[j].result, instr->arg2) == 0) {
                    used[j] = 1;
                }
            }
        }
    }

    // Remove as instruções que não são usadas
    int new_count = 0;
    for (int i = 0; i < cg->count; i++) {
        if (used[i] || (cg->instructions[i].result && cg->instructions[i].result[0] != 't')) {
            cg->instructions[new_count++] = cg->instructions[i];
        }
    }
    cg->count = new_count;
    free(used);
}

// Simplifica expressões constantes (e.g., 2 + 3 -> 5)
void constant_folding(CodeGenerator *cg) {
    for (int i = 0; i < cg->count; i++) {
        IntermediateCode *instr = &cg->instructions[i];

        if (instr->arg1 && instr->arg2) {
            int val1, val2;

            // Verifica se ambos os argumentos são números
            if (sscanf(instr->arg1, "%d", &val1) == 1 && sscanf(instr->arg2, "%d", &val2) == 1) {
                int result = 0;

                // Realiza a operação e substitui pelo valor constante
                if (strcmp(instr->op, "+") == 0) result = val1 + val2;
                else if (strcmp(instr->op, "-") == 0) result = val1 - val2;
                else if (strcmp(instr->op, "*") == 0) result = val1 * val2;
                else if (strcmp(instr->op, "/") == 0 && val2 != 0) result = val1 / val2;

                char *new_result = malloc(16);
                sprintf(new_result, "%d", result);

                // Substitui a instrução pela constante
                instr->op = NULL;
                instr->arg1 = NULL;
                instr->arg2 = NULL;
                instr->result = new_result;
            }
        }
    }
}

// Otimiza o código intermediário
void optimize_code(CodeGenerator *cg) {
    printf("Otimizando o código intermediário...\n");
    constant_folding(cg); // Simplificação de constantes
    eliminate_dead_code(cg); // Eliminação de código morto
}
