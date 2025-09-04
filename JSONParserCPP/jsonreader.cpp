#include "jsonreader.h"
#include "lexer.h"
#include "parser.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>

#define MAX_FILE_SIZE (100 * 1024 * 1024) // 100 MB
using namespace std;

JsonReader::JsonReader() {}

JsonReader::~JsonReader() = default;

string JsonReader::normalize_json(const string& input) {
  string result;
  result.reserve(input.size());

  bool in_string = false;
  bool escape = false;

  for (char c : input) {
    if (in_string) {
      result.push_back(c);
      if (escape) {
        escape = false;
      } else if (c == '\\') {
        escape = true;
      } else if (c == '"') {
        in_string = false;
      }
    } else {
      if (c == '"') {
        in_string = true;
        result.push_back(c);
      }
      else if (isspace(static_cast<unsigned char>(c))) {
        // string dışında whitespace görürsek atla
        continue;
      }
      else {
        result.push_back(c);
      }
    }
  }

  return result;
}

JsonValue JsonReader::readJson(const string& filePath) {
  m_json = JsonValue{}; // Reset previous JSON value
  string content = readall(filePath);
  // NORMALIZING (PREPROCESSING)
  string normalized = normalize_json(content);

  // LEXING
  Lexer lexer(normalized);
  vector<Token> tokens = lexer.tokenize();

  // PARSING
  Parser parser(tokens);
  JsonValue json = parser.parse();

  m_json = json;
  return json;
}

JsonValue JsonReader::readJson(const string& filePath) {
  tokenPool.clear();
  tokenPool.reserve(1024); // Tahmin edilen token sayısı

  string content = readall(filePath);

  // NORMALIZE (PREPROCESS) PHASE
  string normalized = normalize_json(content);

  // LEXER PHASE
  Lexer lexer(normalized);
  tokenPool = lexer.tokenize();

  // PARSER PHASE
  Parser parser(tokenPool);
  return parser.parse();
}

string JsonReader::readall(const string& filePath) {
  ifstream file(filePath, ios::binary);
  if (!file.is_open()) {
    throw runtime_error("Could not open file: " + filePath);
  }
  
  // Dosya boyutunu kontrol et
  file.seekg(0, ios::end);
  auto fileSize = file.tellg();
  if (fileSize > MAX_FILE_SIZE) {
    throw runtime_error("File too large");
  }
  file.seekg(0, ios::beg);
  
  string content;
  content.reserve(static_cast<size_t>(fileSize));
  content.assign(istreambuf_iterator<char>(file), 
                  istreambuf_iterator<char>());
  
  return content;
}