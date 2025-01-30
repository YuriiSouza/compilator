#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/semantic.h"
#include "include/optimizer.h"
#include "include/codegen.h"

int main() {
    // Código de entrada (substitua isso por leitura de arquivo, se necessário)
    char* source_code = "inteiro a;a = 5;";
    // 1. Lexer: Geração de tokens
    Token* tokens = lexer(source_code);

    // 2. Parser: Construção da árvore sintática abstrata (AST)
    ASTNode* ast = parse_program(tokens);

    // 3. Analisador Semântico: Verificação de erros semânticos
    SymbolTable* table = create_symbol_table();
    
    semantic_analysis(ast,table);

    // 4. Gerador de Código Intermediário
    CodeGenerator* cg = create_codegen();
    generate_intermediate_code(ast, cg);
    print_codegen_node(cg);

    // 5. Otimização de Código
    // optimize_code(cg);

    // 6. Geração de Código Final
    const char* output_file = "output.s";
    generate_assembly_code(cg, output_file);

    printf("\n\n");

    // // 7. Exibição do código intermediário (opcional)
    printf("Código Intermediário:\n");
    print_intermediate_code(cg);

    // printf("Código final gerado em: %s\n", output_file);

    // Liberação de memória (garbage collection básico)
    free(cg->instructions);
    free(cg);
    free(ast);
    free(tokens);
    free(table);

    return 0;
}