#include "lexer.h"

vector<Token> Lexer::tokenize() {
  vector<Token> tokens;

  while (pos < text.size()) {
  skipWhitespace();

  if (pos >= text.size()) {
    tokens.push_back({TokenType::TK_FEND, ""});
    break;
  }

  char c = peek();

  switch (c) {
    case '{': tokens.push_back(makeToken(TokenType::TK_OBJ_BEGIN, consume())); break;
    case '}': tokens.push_back(makeToken(TokenType::TK_OBJ_END, consume())); break;
    case '[': tokens.push_back(makeToken(TokenType::TK_ARR_BEGIN, consume())); break;
    case ']': tokens.push_back(makeToken(TokenType::TK_ARR_END, consume())); break;
    case ':': tokens.push_back(makeToken(TokenType::TK_COLON, consume())); break;
    case ',': tokens.push_back(makeToken(TokenType::TK_COMMA, consume())); break;
    case '"': tokens.push_back(readString()); break;
    default:
    if (isdigit(c) || c == '-' || c == '+') {
    tokens.push_back(readNumber());
    } else if (isalpha(c)) {
    tokens.push_back(readKeyword());
    } else {
    throw runtime_error(string("Unexpected character: ") + c);
    }
  }
  }

  return tokens;
}

Token Lexer::readString() {
  consume(); // opening "
  size_t start = pos;

  // Önce hızlı tarama yap - escape karakteri var mı?
  size_t scan_pos = pos;
  bool has_escape = false;

  while (scan_pos < text.size() && text[scan_pos] != '"') {
    if (text[scan_pos] == '\\') {
      has_escape = true;
      break;
    }
    scan_pos++;
  }

  // Eğer escape yoksa, direkt substring al
  if (!has_escape && scan_pos < text.size()) {
    string val = text.substr(start, scan_pos - start);
    pos = scan_pos + 1; // closing " karakterini geç
    return {TokenType::TK_STR, std::move(val)};
  }

  // Escape varsa escape handling mantığını kullan
  string val;
  val.reserve(scan_pos - start + 10); // Tahmini boyut
  bool escape = false;

  while (pos < text.size()) {
    char c = consume();
    
    if (escape) {
      // Escape karakterini işle
      switch (c) {
        case '"': val.push_back('"'); break;
        case '\\': val.push_back('\\'); break;
        case '/': val.push_back('/'); break;
        case 'b': val.push_back('\b'); break;
        case 'f': val.push_back('\f'); break;
        case 'n': val.push_back('\n'); break;
        case 'r': val.push_back('\r'); break;
        case 't': val.push_back('\t'); break;
        case 'u': 
          // Unicode escape handling (basitleştirilmiş)
          // Gerçek implementasyon için 4 hex digit okuyup convert et
          val.push_back(c); // Şimdilik olduğu gibi ekle
          break;
        default:
          val.push_back(c); // Geçersiz escape, olduğu gibi ekle
      }
      escape = false;
    } else if (c == '\\') {
      escape = true;
    } else if (c == '"') {
      break; // String sonu
    } else {
      val.push_back(c);
    }
  }

  return {TokenType::TK_STR, std::move(val)};
}

Token Lexer::readNumber() {
  size_t start = pos;
  if (peek() == '-' || peek() == '+') pos++;
  
  bool hasDecimal = false;
  while (pos < text.size()) {
    char c = peek();
    if (isdigit(c)) {
      pos++;
    } else if (c == '.' && !hasDecimal) {
      hasDecimal = true;
      pos++;
    } else {
      break;
    }
  }
  
  string val = text.substr(start, pos - start);
  return {TokenType::TK_NUM, move(val)};
}

Token Lexer::readKeyword() {
  string val;
  while (pos < text.size() && isalpha(peek())) val.push_back(consume());
  TokenType type = strtotoken(val);
  if (type == TokenType::UNKNOWN)
  throw runtime_error("Invalid keyword: " + val);
  return {type, val};
}

TokenType Lexer::strtotoken(const string& str) {
  if (str == "{") return TokenType::TK_OBJ_BEGIN;
  if (str == "}") return TokenType::TK_OBJ_END;
  if (str == "[") return TokenType::TK_ARR_BEGIN;
  if (str == "]") return TokenType::TK_ARR_END;
  if (str == ":") return TokenType::TK_COLON;
  if (str == ",") return TokenType::TK_COMMA;
  if (str == "true") return TokenType::TK_TRUE;
  if (str == "false") return TokenType::TK_FALSE;
  if (str == "null") return TokenType::TK_NULL;
  // String ve Number için daha karmaşık kontroller gerekebilir
  // Burada basitçe kontrol ediyoruz
  if (!str.empty() && str.front() == '"' && str.back() == '"') return TokenType::TK_STR;
  // Basit sayı kontrolü
  bool is_number = !str.empty() && (isdigit(str[0]) || str[0] == '-' || str[0] == '+');
  for (size_t i = 1; i < str.size() && is_number; ++i) {
  if (!isdigit(str[i]) && str[i] != '.') {
  is_number = false;
  }
  }
  if (is_number) return TokenType::TK_NUM;
  
  return TokenType::UNKNOWN;
}

string Lexer::tokentostr(TokenType token) {
  switch (token) {
  case TokenType::TK_OBJ_BEGIN: return "{";
  case TokenType::TK_OBJ_END: return "}";
  case TokenType::TK_ARR_BEGIN: return "[";
  case TokenType::TK_ARR_END: return "]";
  case TokenType::TK_COLON: return ":";
  case TokenType::TK_COMMA: return ",";
  case TokenType::TK_STR: return "string";
  case TokenType::TK_NUM: return "number";
  case TokenType::TK_TRUE: return "true";
  case TokenType::TK_FALSE: return "false";
  case TokenType::TK_NULL: return "null";
  case TokenType::TK_FEND: return "EOF";
  default: return "unknown";
  }
}