#ifndef JSONREADER_H
#define JSONREADER_H
#include <string>
using namespace std;
#include "lexer.h"
#include "parser.h"

class JsonReader {
public:
  JsonReader();
  ~JsonReader();

  JsonValue readJson(const string& filePath);

  bool isObject() const { return m_json.isObject(); }
  string getString(const string& key);
private:
  JsonValue m_json;
  vector<Token> tokenPool;

  string readall(const string& filePath);
  string normalize_json(const string& input);
  
  TokenType strtotoken(const string& str);
  string tokentostr(TokenType token);
};

#endif // JSONREADER_H