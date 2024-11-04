# A_Language is my simple language. [In Progress]

---

## Lexer
The lexer is the first step in the compilation process and it is the first part of the A_Language compiler to be implemented.

The lexer outlined in this document is a simple lexer that is capable of tokenizing the following tokens:
- Keywords: `func`, `num`, `void`, `if`, `else`, `while`, `return`, `const`, `string`, `var`, `int`, `float`

- Identifiers
- Numbers (integers and floats)
- Strings
- Operators: `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`
- Delimiters: `(`, `)`, `{`, `}`, `[`, `]`, `,`, `;`
- Comments: `//`
