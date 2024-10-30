#pragma once
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace  AL
{
constexpr char OneCharToken[]{
  '\0',
  '(',
  ')',
  '{',
  '}',
  ':',
  ';',
  ',',
  '+',
  '-',
  '*',
  '<',
  '>',
  '!'
};

enum class TokenType : char {
  Unknown = -128,

  // one and more of it mean different tokens
  Slash,
  SlashSlash,
  Equal,
  EqualEqual,
  Ampersand,
  AmpersandAmpersand,
  Pipe,
  PipePipe,

  // numbers and identifiers
  Identifier,
  Number,

  // reserved keywords
  Keyword_Function,
  Keyword_Number,
  Keyword_Void,
  Keyword_If,
  Keyword_Else,
  Keyword_While,
  Keyword_Return,
  Keyword_const,

  // one character tokens
  EOL = OneCharToken[0],
  LeftParenthesis = OneCharToken[1],
  RightParenthesis = OneCharToken[2],
  LeftBrace = OneCharToken[3],
  RightBrace = OneCharToken[4],
  Colon = OneCharToken[5],
  Semicolon = OneCharToken[6],
  Comma = OneCharToken[7],
  Plus = OneCharToken[8],
  Minus = OneCharToken[9],
  Asterisk = OneCharToken[10],
  LowerThan = OneCharToken[11],
  GreaterThan = OneCharToken[12],
  Exclamation = OneCharToken[13]
};

const std::unordered_map<std::string_view, TokenType> Keywords = {
    {"func", TokenType::Keyword_Function}, {"num", TokenType::Keyword_Number},
    {"void", TokenType::Keyword_Void},     {"if", TokenType::Keyword_If},
    {"else", TokenType::Keyword_Else},     {"while", TokenType::Keyword_While},
    {"return", TokenType::Keyword_Return}, {"const", TokenType::Keyword_const}};

struct SourceLocation {
  std::string_view filepath;
  int line;
  int col;
};

struct SourceFile {
  std::string_view path;
  std::string buffer;
};

struct Token {
  std::optional<std::string> value = std::nullopt; // 40B
  SourceLocation loc;                              // 24B
  TokenType type;                                  // 1B
};

class Lexer {
private:
  std::unique_ptr<SourceFile> source_file;

  // counters
  unsigned int line = 1;
  unsigned int column = 0;
  unsigned int idx = 0;

  // funcions
  char GetNextChar() const { return source_file->buffer[idx]; };
  char PopNextChar() {
    std::string &buf = source_file->buffer;
    if (idx >= buf.size()) {
      throw std::runtime_error("Index has passed the source buffer EOF");
    }
    ++column;
    if(buf[idx] == '\n') {
      ++line;
      column = 0;
    }
    ++idx;
    return buf[idx];
  }

public:
  Lexer(const SourceFile &source)
      : source_file(std::make_unique<SourceFile>(source)) {}
  Token GetNextToken();
};

} // namespace  AL
