#include "ast.hpp"
#include <iostream>

namespace AL {
void Block::dump(size_t level) const {
  std::cerr << std::string(2 * level, '=') << "Block\n";
}

void FunctionDeclaration::dump(size_t level) const {
  std::cerr << std::string(level*2, '=') << "FunctionDecl: " << identifier << ':'
            << type.type_name << '\n';

  body->dump(level + 1);
}
} // namespace AL
