# Grammar for the language *so far*
These are the rules for the language so far. The language is a simple one with only function declarations. The function declarations are of the form `func <Identifier> () : <Type> <Block>`. The language is statically typed and the type of the function is specified after the colon. The block of the function is empty for now.

BNF (Backus-Naur Form) is used to define the grammar of the language. The rules are defined in the following format:
```bnf
<SourceFile> 
  ::= <FunctionDeclaration>* _EOF
```

```bnf
<FunctionDeclaration> 
  ::= 'func' <Identifier>  '(' ')' ':' <Type> <Block>
```

```bnf
<Block>
    ::= '{' '}'
```

```bnf
<type>
    ::= 'void'
    |   <Identifier>
```