#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "include/semantic.h"

//Funcao para criar uma tabvelade simbolos vazia

SymbolTable *create_symbol_table() {
  SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
  for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
    table->symbols[i] = NULL;
  }
  return table;
}

int hash(const char *name) {
  int hash_value = 0;

  for (int i = 0; name[i] != '\0'; i++) {
    hash_value = (hash_value + name[i] % SYMBOL_TABLE_SIZE);
  }

  return hash_value;
}

// Função para buscar um símbolo na tabela
Symbol *lookup(SymbolTable *table, const char *name) {
    int index = hash(name);
    Symbol *symbol = table->symbols[index];
    return symbol && strcmp(symbol->name, name) == 0 ? symbol : NULL;
}

// Função para inserir um símbolo na tabela
void insert(SymbolTable *table, const char *name, VariableType type) {
    int index = hash(name);
    Symbol *symbol = malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->type = type;
    symbol->is_declared = 1;
    table->symbols[index] = symbol;
}

// Função para verificar se uma variável foi declarada
void check_declaration(SymbolTable *table, const char *name) {
    if (!lookup(table, name)) {
        fprintf(stderr, "Erro: A variável '%s' não foi declarada.\n", name);
        exit(1);
    }
}

// Função para verificar se o tipo de uma variável é compatível
void check_type(SymbolTable *table, const char *name, VariableType expected_type) {
    Symbol *symbol = lookup(table, name);
    if (!symbol) {
        fprintf(stderr, "Erro: A variável '%s' não foi declarada.\n", name);
        exit(1);
    }

    if (symbol->type != expected_type) {
        fprintf(stderr, "Erro: Tipo incompatível para a variável '%s'. Esperado %d, encontrado %d.\n",
                name, expected_type, symbol->type);
        exit(1);
    }
}
// Função principal de análise semântica
void semantic_analysis(ASTNode *node, SymbolTable *table) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            semantic_analysis(node->left, table);
            semantic_analysis(node->right, table);
            break;

        case NODE_DECLARATION:
            insert(table, node->value, TYPE_INT); // Supondo que o tipo seja sempre int
            break;

        case NODE_ASSIGNMENT:
            check_declaration(table, node->value);
            semantic_analysis(node->left, table);
            break;

        case NODE_RETURN:
            semantic_analysis(node->left, table);
            break;

        case NODE_EXPRESSION:
        case NODE_TERM:
        case NODE_FACTOR:
            semantic_analysis(node->left, table);
            semantic_analysis(node->right, table);
            break;

        case NODE_IDENTIFIER:
            check_declaration(table, node->value);
            break;

        case NODE_NUMBER:
            // Não é necessário verificar tipo para números literais
            break;

        default:
            fprintf(stderr, "Erro: Tipo de nó desconhecido.\n");
            exit(1);
    }
}