#include <iostream>
#include "jsonreader.h"
using namespace std;
#include <variant>
#include <string>
#include "simdjson.h"

void simdjson_example(const string& filePath) {
  simdjson::dom::parser parser;
  simdjson::dom::element doc = parser.load(filePath);
  string s = doc["keystr"].get_string().value().data();
  cout << doc["keystr"].get_string().value().data() << endl;
  cout << doc["keyint"].get_int64().value().data() << endl;
  cout << doc["keyfloat"].get_double().value().data() << endl;
  cout << doc["keybool"].get_bool().value().data() << endl;

  cout << doc["keyarray"].get_string().value().data() << endl;

  cout << doc["keystr"].get_string().value().data() << endl;
  cout << doc["keystr"].get_string().value().data() << endl;
  cout << doc["keystr"].get_string().value().data() << endl;
}

void jreaderexample(const string& filePath) {
  JsonReader* reader = new JsonReader();
  try {
    JsonValue result = reader->readJson(filePath);
    JsonValue::Object obj = result.toObject();

    cout << obj.at("keystr").toString() << endl;
    cout << obj.at("keyint").toInt() << endl;
    cout << obj.at("keyfloat").toDouble() << endl;
    cout << obj.at("keybool").toBool() << endl;

    JsonValue::Array arr = obj.at("keyarray").toArray();
    for (const auto& item : arr) {
      cout << item.toInt() << " ";
    }

    JsonValue::Object nestedObj = obj.at("keyobject").toObject();
    cout << endl << nestedObj.at("nestedKey").toString() << endl;
  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
  }

  delete reader;
}

int main(int argc, char** argv) {
  cout << "Enter the file: " << endl;
  string filePath;
  cin >> filePath;

  // SIMDJSON
  cout << "SIMDJSON OUTPUT:" << endl;
  clock_t start = clock();
  for (int i = 0; i < 100000; i++) {
    simdjson_example(filePath);
  }
  cout << "Time taken: " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

  // JSONREADER
  cout << "JSONREADER OUTPUT:" << endl;
  start = clock();
  for (int i = 0; i < 100000; i++) {
    jreaderexample(filePath);
  }
  cout << "Time taken: " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

  return 0;
}