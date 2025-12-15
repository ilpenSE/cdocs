#include <iostream>
using namespace std;
#include <variant>
#include <string>
#include "jsonreader.h"
#include "parser.h"

void jreaderexample(const string& filePath, bool shouldLog) {
  JsonReader* reader = new JsonReader();
  try {
    JsonValue result = reader->readJson(filePath);
    JsonValue::Object obj = result.toObject();

    if (shouldLog) {
      cout << obj.at("keystr").toString() << endl;
      cout << obj.at("keyint").toInt() << endl;
      cout << obj.at("keyfloat").toDouble() << endl;
      cout << obj.at("keybool").toBool() << endl;
    }

    JsonValue::Array arr = obj.at("keyarray").toArray();
    if (shouldLog) {
      for (const auto& item : arr) {
        cout << item.toInt() << " ";
      }
    }

    JsonValue::Object nestedObj = obj.at("keyobject").toObject();
    if (shouldLog) cout << endl << nestedObj.at("nestedKey").toString() << endl;
  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
  }

  delete reader;
}

int main() {
  cout << "Enter the file: " << endl;
  string filePath;
  cin >> filePath;

  // JSONREADER
  cout << "JSONREADER OUTPUT:" << endl;
  clock_t start = clock();
  for (int i = 0; i < 100000; i++) {
    jreaderexample(filePath, i == 0);
  }
  cout << "Time taken: " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

  return 0;
}
