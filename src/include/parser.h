#ifndef PARSER_H
#define PARSER_H

#include "lexer.h" //icluindo o arquivo lexer.h

//definindo os tipos de nos de arvore sintatica abstrata (AST)
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

typedef struct ASTNode {
  NodeType type;
  struct ASTNode* left;
  struct ASTNode* right;
  char* value;
} ASTNode;

ASTNode* parse_program();
ASTNode* parse_statement();
ASTNode* parse_declaration();
ASTNode* parse_assignment();
ASTNode* parse_return();
ASTNode* parse_expression();
ASTNode* parse_term();
ASTNode* parse_factor();

#endif