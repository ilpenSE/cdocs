/**
 * my json parser all rights reserved to no one
*/
#include <variant>
#include <string>
#include <vector>
#include <unordered_map>

typedef struct {
    using array  = std::vector<JsonValue>;
    using object = std::unordered_map<std::string, JsonValue>;

    std::variant<
        std::nullptr_t,
        bool,
        double, // both ints and floats
        std::string,
        array,
        object
    > value;
} JsonValue;

class JsonParser {
  public:
    static bool read_file(std::string file_path);

  private:
    void tokenize();
}
