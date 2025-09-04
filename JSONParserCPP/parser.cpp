#include "parser.h"

JsonValue Parser::parseValueImpl() {
  const Token& tok = peek();
  switch (tok.type) {
    case TokenType::TK_OBJ_BEGIN: return parseObject();
    case TokenType::TK_ARR_BEGIN: return parseArray();
    case TokenType::TK_STR: {
      string val = consume().value;
      return JsonValue{val};
    }
    case TokenType::TK_NUM: {
      double val = stod(consume().value);
      if (val == static_cast<int>(val)) {
        return JsonValue{static_cast<int>(val)};
      }
      return JsonValue{val};
    }
    case TokenType::TK_TRUE: consume(); return JsonValue{true};
    case TokenType::TK_FALSE: consume(); return JsonValue{false};
    case TokenType::TK_NULL: consume(); return JsonValue{nullptr};
    default:
      throw runtime_error("Unexpected token: " + tok.value);
  }
}

JsonValue Parser::parseObject() {
  consume(); // consume '{'
  JsonValue::Object obj;

  if (peek().type == TokenType::TK_OBJ_END) {
    consume(); // empty object
    return JsonValue{obj};
  }

  while (true) {
    const Token& keyTok = consume();
    if (keyTok.type != TokenType::TK_STR) throw runtime_error("Expected string key in object");

    if (consume().type != TokenType::TK_COLON) throw runtime_error("Expected ':' after key");

    obj[keyTok.value] = parseValue();

    if (peek().type == TokenType::TK_COMMA) {
      consume(); // continue
    } else if (peek().type == TokenType::TK_OBJ_END) {
      consume(); // end object
      break;
    } else {
      throw runtime_error("Expected ',' or '}' in object");
    }
  }

  return JsonValue{obj};
}

JsonValue Parser::parseArray() {
  consume(); // consume '['
  JsonValue::Array arr;

  if (peek().type == TokenType::TK_ARR_END) {
    consume(); // empty array
    return JsonValue{arr};
  }

  while (true) {
    arr.push_back(parseValue());

    if (peek().type == TokenType::TK_COMMA) {
      consume(); // continue
    } else if (peek().type == TokenType::TK_ARR_END) {
      consume(); // end array
      break;
    } else {
      throw runtime_error("Expected ',' or ']' in array");
    }
  }

  return JsonValue{arr};
}
