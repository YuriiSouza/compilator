#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./include/lexer.h"

// Definição dos globais
Token* tokens = NULL;             // Definição real do array de tokens
int current_token_index = 0;      // Definição real do índice atual

//Função para verificar se uma palavra é uma palavra-chave
int is_keyword(const char* word) {
  const char *keywords[] = {
    "inteiro", "flutuante", "vazio", "letra", "se", "senao", "enquanto", "por", "quando", "retorna"
  };

  for(int i = 0; i < 10; ++i) {
    if(strcmp(word, keywords[i]) == 0){
      return 1;
    }
  }

  return 0;
};

//Funcao principal lexer
Token* lexer(const char *source_code) {
  tokens = malloc(1024 * sizeof(Token));
  int token_count = 0;

  for (int i = 0; source_code[i] != '\0';) {
    char c = source_code[i];

    //Ignorar espacos em branco e comentarios
    if (isspace(c)) {
      i++;
      continue;
    }

    //Ignorar comentarios linha unica
    if ( c == '/' && source_code[i + 1] == '/') {
      while (source_code[i] != '\n' && source_code[i] != '\0') {
        i++;
      }
      continue;
    }

    //Ignorar comentarios multiplas linhas
    if (c == '/' && source_code[i + 1] == '*') {
      i += 2;
      while (source_code[i] != '*' && source_code[i + 1] != '/') {
        i++;
      }
      i += 2;
      continue;
    }

    //Palavras-chave ou identificadores
    if (isalpha(c)) {
      char buffer[64] = {0};
      int j = 0;
      while (isalnum(source_code[i]) || source_code[i] == '_') {
        buffer[j++] = source_code[i++];
      }
      buffer[j] = '\0';

      Token token;
      if (is_keyword(buffer)) {
        token.type = TOKEN_KEYWORD;
      } else {
        token.type = TOKEN_IDENTIFIER;
      }
      token.value = strdup(buffer);
      tokens[token_count++] = token;
      continue;
    }

    //numeros
    if (isdigit(c)) {
      char buffer[64] = {0};
      int j = 0;

      while (isdigit(source_code[i])) {
        buffer[j++] = source_code[i++];
      }
      buffer[j] = '\0';

      Token token;
      token.type = TOKEN_NUMBER;
      token.value = strdup(buffer);
      tokens[token_count++] = token;
      continue;
    }

    //operadores
    if (strchr("+-*/=", c)) {
      Token token;
      token.type = TOKEN_OPERATOR;
      token.value = strndup(&c, 1);
      tokens[token_count++] = token;
      i++;
      continue;
    }

    //Simbolos
    if (strchr(";(){}[],", c)) {
      Token token;
      token.type = TOKEN_SYMBOL;
      token.value = strndup(&c, 1);
      tokens[token_count++] = token;
      i++;
      continue;
    }


    // caso nao reconheca o caractere
    fprintf( stderr, "Erro lexico na linha %d: Caractere desconhecido '%c'\n", 1, c);
    exit(1);
  }

  //Adicionar token EOF ao final
  Token eof_token;
  eof_token.type = TOKEN_EOF;
  eof_token.value = NULL;
  tokens[token_count++] = eof_token;

  return tokens;
}
