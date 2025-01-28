#ifndef PARSER_H
#define PARSER_H

#include "lexer.h" // Incluindo o arquivo lexer.h

// Definindo os tipos de nós da árvore sintática abstrata (AST)
typedef enum {
  NODE_PROGRAM,
  NODE_DECLARATION,
  NODE_ASSIGNMENT,
  NODE_RETURN,
  NODE_EXPRESSION,
  NODE_TERM,
  NODE_FACTOR,
  NODE_IDENTIFIER,
  NODE_NUMBER,
} NodeType;

// Estrutura de um nó da AST
typedef struct ASTNode {
  NodeType type;               // Tipo do nó
  struct ASTNode* left;        // Filho esquerdo
  struct ASTNode* right;       // Filho direito
  char* value;                 // Valor associado ao nó (e.g., identificadores, números, operadores)
  char* temp;                  // Identificador temporário para código intermediário
} ASTNode;

// Declarações de funções de parsing
ASTNode* parse_program();
ASTNode* parse_statement();
ASTNode* parse_declaration();
ASTNode* parse_assignment();
ASTNode* parse_return();
ASTNode* parse_expression();
ASTNode* parse_term();
ASTNode* parse_factor();

#endif
