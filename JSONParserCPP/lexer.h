#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>
using namespace std;

enum class TokenType {
  TK_OBJ_BEGIN,     // {
  TK_OBJ_END,       // }
  TK_ARR_BEGIN,     // [
  TK_ARR_END,       // ]
  TK_COLON,       // :
  TK_COMMA,       // ,
  TK_STR,         // "..."
  TK_NUM,         // 123, -123, 12.34, etc.
  TK_TRUE,        // true
  TK_FALSE,       // false
  TK_NULL,        // null
  TK_FEND,        // file end
  UNKNOWN
};

struct Token {
  TokenType type;
  string value;
};

class Lexer {
public:
  explicit Lexer(const string& src) : text(src), pos(0) {}

  TokenType strtotoken(const string& str);
  string tokentostr(TokenType token);

  vector<Token> tokenize();

private:
  string text;
  size_t pos;

  char peek() const { 
      if (pos >= text.size()) return '\0';
      return text[pos]; 
  }
  char consume() { return text[pos++]; }

  void skipWhitespace() {
    while (pos < text.size() && isspace(static_cast<unsigned char>(text[pos]))) {
      pos++;
    }
  }

  Token makeToken(TokenType type, char c) {
    return {type, string(1, c)};
  }

  Token readString();
  Token readNumber();
  Token readKeyword();
};
#endif // LEXER_H