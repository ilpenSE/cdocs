#ifndef PARSER_H
#define PARSER_H
using namespace std;
#include <string>
#include <variant>
#include <vector>
#include <map>
#include "lexer.h"

struct JsonValue {
  using Object = map<string, JsonValue>;
  using Array = vector<JsonValue>;

  variant<nullptr_t, bool, double, float, int, string, Object, Array> value;

  bool isObject() const { return holds_alternative<Object>(value); }
  bool isArray() const { return holds_alternative<Array>(value); }
  bool isString() const { return holds_alternative<string>(value); }
  bool isNumber() const { return holds_alternative<double>(value); }
  bool isBool() const { return holds_alternative<bool>(value); }
  bool isNull() const { return holds_alternative<nullptr_t>(value); }
  bool isInt() const { return holds_alternative<int>(value); }
  bool isFloat() const { return holds_alternative<float>(value); }

  string toString() const {
    if (isString()) return get<string>(value);
    throw runtime_error("Not a string");
  }

  int toInt() const {
    if (isInt()) return get<int>(value);
    throw runtime_error("Not an integer");
  }

  double toDouble() const {
    if (isNumber()) return get<double>(value);
    throw runtime_error("Not a number");
  }

  float toFloat() const {
    if (isNumber()) return get<float>(value);
    throw runtime_error("Not a float");
  }

  bool toBool() const {
    if (isBool()) return get<bool>(value);
    throw runtime_error("Not a boolean");
  }

  Object toObject() const {
    if (isObject()) return get<Object>(value);
    throw runtime_error("Not an object");
  }

  Array toArray() const {
    if (isArray()) return get<Array>(value);
    throw runtime_error("Not an array");
  }
};

class Parser {
public:
  Parser(const vector<Token>& tokens) : tokens(tokens), pos(0) {}

  JsonValue parse() {
      return parseValue();
  }

private:
  const std::vector<Token>& tokens;
  size_t pos;

  static const size_t MAX_DEPTH = 1000;
  size_t depth = 0;

  const Token& peek() const {
    if (pos >= tokens.size()) throw std::runtime_error("Unexpected end of input");
    return tokens[pos];
  }

  const Token& consume() {
    return tokens[pos++];
  }

  JsonValue parseValue() {
    if (++depth > MAX_DEPTH) {
      throw runtime_error("Maximum nesting depth exceeded");
    }

    auto result = parseValueImpl();
    --depth;
    return result;
  }

  JsonValue parseValueImpl();
  JsonValue parseObject();
  JsonValue parseArray();
};

#endif // PARSER_H