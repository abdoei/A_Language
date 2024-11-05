#include <iostream>
#include <filesystem>
#include "lexer.hpp"
#include <sstream>
#include <fstream>
#include "utils.hpp"


int main(int argc, char *argv[]) {
  std::filesystem::path path{argv[1]};
  AL::SourceFile src{path.c_str(), ""};
  AL::Lexer L1(src);
  
  AL::Token current_token;
  while (AL::TokenType::_EOF != (current_token = L1.GetNextToken()).type){
    std::cout << to_string(current_token.type) << "\t\t";
    if (current_token.value != std::nullopt) {
      std::cout << current_token.value.value() << '\t';
    }
    std::cout << std::endl;
  }
  return 0;
}
