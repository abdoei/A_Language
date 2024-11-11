#pragma once
#include "utils.hpp"

namespace AL {
using std::move;
// user-defined and built-in types
struct alignas(64) Type {
  enum class eType : char { Num, Void, Custom };

  eType enum_type;
  std::string type_name;

  // setters (builders)
  static auto BuildVoid() -> Type { return {eType::Void, "void"}; }
  static auto BuildNum() -> Type { return {eType::Num, "num"}; }
  static auto Custom(std::string name) -> Type {
    return {eType::Custom, move(name)};
  }

private:
  Type(eType typ, std::string name) : enum_type(typ), type_name(move(name)) {}
};

struct alignas(64) Declaration {
  const SourceLocation location;
  const std::string identifier;

  Declaration(SourceLocation loc, std ::string id)
      : location(loc), identifier(move(id)) {}

  virtual ~Declaration() = default;

  virtual void dump(size_t level = 0) const = 0;
};

struct alignas(64) Block {
  const SourceLocation location;

  Block(SourceLocation loc) : location(loc) {}

  void dump(size_t level = 0) const;
};

struct FunctionDeclaration : public Declaration {
  Type type;
  std::unique_ptr<Block> body;

  FunctionDeclaration(SourceLocation loc, std::string id, Type t,
                      std::unique_ptr<Block> bdy)
      : Declaration(loc, id), type(move(t)), body(move(bdy)) {}

  void dump(size_t level = 0) const override;
};
} // namespace AL
