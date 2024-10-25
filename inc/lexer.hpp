#pragma once

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
}  // namespace  AL
