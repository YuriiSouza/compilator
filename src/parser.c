#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/parser.h"

// Funções auxiliares
Token* get_current_token() {
    return &tokens[current_token_index];
}

void consume() {
    current_token_index++;
}

int match(TokenType type) {
    return get_current_token()->type == type;
}

// Função para criar um novo nó da árvore sintática (AST)
ASTNode* create_node(NodeType type, char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->value = value ? strdup(value) : NULL;
    return node;
}

// Função de parser para um programa
ASTNode* parse_program() {
    ASTNode *program = create_node(NODE_PROGRAM, NULL);
    ASTNode *last_node = NULL;
    
    while (get_current_token()->type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement();
        if (!last_node) {
            program->left = stmt;
        } else {
            last_node->right = stmt;
        }
        last_node = stmt;
    }

    return program;
}

ASTNode* parse_statement() {
    Token* current_token = get_current_token();

    if (current_token->type == TOKEN_KEYWORD) {
        if (strcmp(current_token->value, "inteiro") == 0 || 
            strcmp(current_token->value, "flutuante") == 0 || 
            strcmp(current_token->value, "letra") == 0) {
            // Declaração de variável
            return parse_declaration();
        } else if (strcmp(current_token->value, "return") == 0) {
            // Instrução de retorno
            return parse_return();
        }
    }

    if (current_token->type == TOKEN_IDENTIFIER) {
        // Atribuição
        return parse_assignment();
    }

    fprintf(stderr, "Erro: Declaração ou instrução desconhecida\n");
    exit(1);
}

ASTNode* parse_return() {
    Token* return_token = get_current_token();

    if (strcmp(return_token->value, "return") != 0) {
        fprintf(stderr, "Erro: Esperado 'return', encontrado '%s'\n", return_token->value);
        exit(1);
    }

    consume(); // Consumir o token 'return'

    // Analisar a expressão a ser retornada
    ASTNode* expr = parse_expression();

    // Garantir que a instrução termina com ';'
    if (match(TOKEN_SYMBOL) && strcmp(get_current_token()->value, ";") == 0) {
        consume(); // Consumir o ';'
    } else {
        fprintf(stderr, "Erro: Esperado ';' após 'return'\n");
        exit(1);
    }

    // Criar o nó da instrução de retorno
    ASTNode* return_node = create_node(NODE_RETURN, "return");
    return_node->left = expr;
    return return_node;
}


// Função de parser para uma declaração
ASTNode* parse_declaration() {
    consume();  // Consumir a palavra-chave 'int' ou 'float'
    
    Token *identifier_token = get_current_token();
    consume();  // Consumir o identificador

    if (match(TOKEN_SYMBOL) && strcmp(get_current_token()->value, ";") == 0) {
        consume();  // Consumir o símbolo ';'
    } else {
        fprintf(stderr, "Erro: Esperado ';' após declaração\n");
        exit(1);
    }

    return create_node(NODE_DECLARATION, identifier_token->value);
}

// Função de parser para uma atribuição
ASTNode* parse_assignment() {
    Token *identifier_token = get_current_token();
    consume();  // Consumir o identificador

    if (match(TOKEN_OPERATOR) && strcmp(get_current_token()->value, "=") == 0) {
        consume();  // Consumir o operador '='
    }

    ASTNode *expr = parse_expression();

    if (match(TOKEN_SYMBOL) && strcmp(get_current_token()->value, ";") == 0) {
        consume();  // Consumir o símbolo ';'
    } else {
        fprintf(stderr, "Erro: Esperado ';' após atribuição\n");
        exit(1);
    }

    ASTNode *assignment_node = create_node(NODE_ASSIGNMENT, identifier_token->value);
    assignment_node->left = expr;
    return assignment_node;
}

// Função de parser para uma expressão (contendo operadores aritméticos)
ASTNode* parse_expression() {
    ASTNode *term = parse_term();
    
    while (match(TOKEN_OPERATOR) && (strcmp(get_current_token()->value, "+") == 0 || strcmp(get_current_token()->value, "-") == 0)) {
        Token *operator_token = get_current_token();
        consume();  // Consumir o operador

        ASTNode *node = create_node(NODE_EXPRESSION, operator_token->value);
        node->left = term;
        node->right = parse_term();
        
        term = node;
    }

    return term;
}

// Função de parser para um termo (multiplicação e divisão)
ASTNode* parse_term() {
    ASTNode *factor = parse_factor();

    while (match(TOKEN_OPERATOR) && (strcmp(get_current_token()->value, "*") == 0 || strcmp(get_current_token()->value, "/") == 0)) {
        Token *operator_token = get_current_token();
        consume();  // Consumir o operador

        ASTNode *node = create_node(NODE_TERM, operator_token->value);
        node->left = factor;
        node->right = parse_factor();
        
        factor = node;
    }

    return factor;
}

// Função de parser para um fator (números ou identificadores)
ASTNode* parse_factor() {
    Token *current_token = get_current_token();
    
    if (match(TOKEN_NUMBER)) {
        ASTNode *node = create_node(NODE_NUMBER, current_token->value);
        consume();  // Consumir o número
        return node;
    }

    if (match(TOKEN_IDENTIFIER)) {
        ASTNode *node = create_node(NODE_IDENTIFIER, current_token->value);
        consume();  // Consumir o identificador
        return node;
    }

    fprintf(stderr, "Erro: Esperado número ou identificador\n");
    exit(1);
}

int main() {
    // Exemplo de código-fonte a ser analisado
    const char *source_code = "int x; x = 42; return x;";
    
    // Gerando tokens com o lexer
    Token *tokens = lexer(source_code);
    current_token_index = 0;

    // Analisando o programa com o parser
    ASTNode *program = parse_program();

    printf("Árvore sintática criada com sucesso!\n");

    return 0;
}
