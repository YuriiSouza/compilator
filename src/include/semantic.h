#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"

typedef enum {
  TYPE_INT,
  TYPE_UNKNOWN
} VariableType;

//Estrutura para representa simbolos na tabela de simbolos
typedef struct {
  char *name;
  VariableType type;
  int is_declared;
} Symbol;

// Tamanho maximo da tabela de simbolos
#define SYMBOL_TABLE_SIZE 100

// Estrutura da tabela de simbolos
typedef struct {
  Symbol *symbols[SYMBOL_TABLE_SIZE];
} SymbolTable;

//Funcoes para a tabela de simbolos
SymbolTable *create_symbol_table();
Symbol *lookup(SymbolTable *table, const char *name);
void insert(SymbolTable *table, const char *name, VariableType type);
void check_declaration(SymbolTable *table, const char *name);
void check_type(SymbolTable *table, const char *name, VariableType expect_type);

//Funcoes para o analisador semantico
void semantic_analysis(ASTNode *root, SymbolTable *table);

#endif