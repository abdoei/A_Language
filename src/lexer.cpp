#include "lexer.hpp"

namespace AL {
inline bool IsSpace(char c) {
  return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\r' ||
          c == '\n');
}
inline bool IsAlpha(char c) {
  return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}
inline bool IsNum(char c) { return ('0' <= c && c <= '9'); }
inline bool IsAlphaNum(char c) { return IsAlpha(c) or IsNum(c); }
Token Lexer::GetNextToken() { return {}; }

} // namespace AL
