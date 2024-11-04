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
  _EOF,

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
  Int_Number,
  Float_Number,

  // reserved keywords
  Keyword_Function,
  Keyword_Number,
  Keyword_Void,
  Keyword_If,
  Keyword_Else,
  Keyword_While,
  Keyword_Return,
  Keyword_const,
  Keyword_String,
  Keyword_Var,

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
    {"return", TokenType::Keyword_Return}, {"const", TokenType::Keyword_const},
    {"string", TokenType::Keyword_String}, {"var", TokenType::Keyword_Var},
    {"int", TokenType::Int_Number},        {"float", TokenType::Float_Number}};

struct SourceLocation {
  std::string_view filepath;
  unsigned int line;
  unsigned int col;
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
  char GetNextChar() const { return (idx >= source_file->buffer.size()) ? (char)TokenType::_EOF : source_file->buffer[idx]; };
  char PopNextChar() {
    std::string &buf = source_file->buffer;
    if (idx >= buf.size()) {
      return (char)TokenType::_EOF;
      // throw std::runtime_error("Index has passed the source buffer EOF");
    }
    ++column;
    if(buf[idx] == '\n') {
      ++line;
      column = 0;
    }
    return buf[idx++];
  }
  SourceLocation GetSourceLocation() const {
    return {source_file->path, line, column};
  }

public:
  Lexer(const SourceFile &source)
      : source_file(std::make_unique<SourceFile>(source)) {}
  Token GetNextToken();
};

} // namespace  AL
