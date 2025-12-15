#include <iostream>
using namespace std;
#include <variant>
#include <string>

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

  // JSONREADER
  cout << "JSONREADER OUTPUT:" << endl;
  start = clock();
  for (int i = 0; i < 100000; i++) {
    jreaderexample(filePath);
  }
  cout << "Time taken: " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

  return 0;
}
