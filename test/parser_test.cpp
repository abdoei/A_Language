#include "gtest/gtest.h"
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp" // For FunctionDeclaration
#include <vector>
#include <memory> // For std::unique_ptr

TEST(ParserTest, HandlesSimpleFunction) {
    // 1. Prepare the source code
    AL::SourceFile source_file("test/sample_parser_0.AL", ""); // Let it load from file
    
    // 2. Lex the source code
    AL::Lexer lexer(source_file);
    
    // 3. Parse the tokens
    AL::Parser parser(lexer);
    auto [ast_nodes, incomplete_ast] = parser.ParseSourceFile();
    
    // 4. Assertions
    ASSERT_FALSE(incomplete_ast) << "Parsing failed or was incomplete.";
    
    ASSERT_FALSE(ast_nodes.empty()) << "No AST nodes were produced.";
    
    ASSERT_NE(ast_nodes[0], nullptr) << "First AST node is null.";
    
    // Check if it's a FunctionDeclaration (optional, but good)
    // Dynamic cast to check type, ensure ast.hpp is included
    AL::FunctionDeclaration* func_decl = dynamic_cast<AL::FunctionDeclaration*>(ast_nodes[0].get());
    ASSERT_NE(func_decl, nullptr) << "First AST node is not a FunctionDeclaration.";
    
    // Check function name
    EXPECT_EQ(func_decl->identifier, "main");
    EXPECT_EQ(func_decl->type.type_name, "void");
}
