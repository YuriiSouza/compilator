#ifndef LEXER_H
#define LEXER_H

//Definindo o tipo TokenType
typedef enum {
  TOKEN_KEYWORD, //Palavras-chave
  TOKEN_OPERATOR, //Operadores
  TOKEN_IDENTIFIER, //Identificadores
  TOKEN_NUMBER, //Números
  TOKEN_SYMBOL, //Símbolos
  TOKEN_EOF //Fim de arquivo
} TokenType;

typedef struct {
  TokenType type;
  char* value;
} Token;

Token* lexer(const char* source_code);

#endif // LEXER_H