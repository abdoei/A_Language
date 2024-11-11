#pragma once
#include "utils.hpp"
#include <memory>

namespace AL {
// user-defined and built-in types
struct Type
{
  enum class eType : char
  {
    Num,
    Void,
    Custom
  };

  eType enum_type;
  std::string type_name;

  // setters (builders)
  static Type BuildVoid() { return {eType::Void, "void"}; }
  static Type BuildNum() { return {eType::Num, "num"}; }
  static Type Custom(std::string name) { return {eType::Custom, std::move(name)}; }
  

 private:
  Type(eType t, std::string name)
  : enum_type(t), type_name(name)
  {}
};

struct Declaration {
  SourceLocation location;
  std::string identifier;

  Declaration(SourceLocation loc, std ::string id)
    : location(std::move(loc)), identifier(std::move(id)) 
  {}

  virtual ~Declaration() = default;

  virtual void dump(size_t level = 0) const = 0;
};

struct Block {
  SourceLocation location;

  Block(SourceLocation loc) : location(loc) {}

  void dump(size_t level = 0) const;
};

struct FunctionDeclaration : public Declaration {
  Type type;
  std::unique_ptr<Block> body;

  FunctionDeclaration(SourceLocation loc, std::string id, Type t,
                      std::unique_ptr<Block> bdy)
      : Declaration(loc, id), type(std::move(t)), body(std::move(bdy)) {}

  void dump(size_t level = 0) const override;
};
}
