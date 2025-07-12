#include <cstddef>
#include <cstring>
#include <format>
#include <print>
#include <stdexcept>
#include <utility>
#include <iostream>

class my_string {
private:
  char* data;
  std::size_t length;
  std::size_t capacity;

  void reallocate(const std::size_t new_capacity) {
    char* new_data = new char[new_capacity + 1]; // +1 for null terminator

    // Copy existing string
    if (data) {
      std::strcpy(new_data, data);
    }

    delete [] data;
    data = new_data;
    capacity = new_capacity;
  }

  static std::size_t calculate_length(const char* str) {
    return str ? std::strlen(str) : 0;
  }

public:
  // Default constructor
  my_string() : data{nullptr}, length{0}, capacity{0} {
    std::println("Default constructor called");
  }

  // Construct from C-string
  my_string(const char* str) {
    std::println("C-string constructor called");

    length = calculate_length(str);
    capacity = length;

    if (length > 0) {
      data = new char[capacity + 1];
      std::strcpy(data, str);
    } else {
      data = nullptr;
    }
  }

  // Construct with capacity
  explicit my_string(std::size_t initial_capacity)
    : data{new char[initial_capacity + 1]}
    , length{0}
    , capacity{initial_capacity}
  {
    std::println("Constructor with capacity {} called", initial_capacity);

    // Empty string
    data[0] = '\0';
  }

  // 1. Destructor
  ~my_string() {
    std::println("Destructor called");
    delete [] data;
  }

  // Helper swap function for copy-and-swap idiom
  void swap(my_string& other) noexcept {
    std::swap(data, other.data);
    std::swap(length, other.length);
    std::swap(capacity, other.capacity);
  }

  // 2. Copy constructor
  my_string(const my_string& other) {
    std::println("Copy constructor called");

    length = other.length;
    capacity = other.capacity;

    if (other.data) {
      data = new char[capacity + 1];
      std::strcpy(data, other.data);
    } else {
      data = nullptr;
    }
  }

  // 3. Copy assignment operator (copy-and-swap idiom)
  my_string& operator=(const my_string& other) {
    std::println("Copy assignment operator called (copy-and-swap)");
    my_string {other}.swap(*this);
    return *this;
  }

  // 4. Move constructor
  my_string(my_string&& other) noexcept
    : data{std::exchange(other.data, nullptr)}
    , length{std::exchange(other.length, 0)}
    , capacity{std::exchange(other.capacity, 0)}
  {
    std::println("Move constructor called");
  }

  // 5. Move assignment operator
  my_string& operator=(my_string&& other) noexcept {
    std::println("Move assignment operator called");
    my_string{std::move(other)}.swap(*this);
    return *this;
  }

  // string operations
  my_string& operator+=(const my_string& other) {
    if (other.length > 0) {
      if (length + other.length > capacity) {
        reallocate(length + other.length);
      }
      std::strcat(data, other.data);
      length += other.length;
    }
    return *this;
  }

  my_string& operator+=(const char* str) {
    if (str && *str) {
      std::size_t str_len = std::strlen(str);
      if (length + str_len > capacity) {
        reallocate(length + str_len);
      }

      if (!data) {
        data = new char[capacity + 1];
        data[0] = '\0';
      }

      std::strcat(data, str);
      length += str_len;
    }

    return *this;
  }

  my_string& operator+=(char ch) {
    if (length + 1 > capacity) {
      reallocate(std::max(capacity + 2, length + 1));
    }

    if (!data) {
      data = new char[capacity + 1];
      data[0] = '\0';
    }

    data[length] = ch;
    data[length + 1] = '\0';
    ++length;

    return *this;
  }

  // Concatenation operators
  my_string operator+(const my_string& other) const {
    my_string result{*this};
    result += other;
    return result;
  }

  my_string operator+(const char* str) const {
    my_string result{*this};
    result += str;
    return result;
  }

  // Access operators
  char& operator[](const std::size_t index) {
    if (index >= length) {
      throw std::out_of_range("Index out of bounds");
    }
    return data[index];
  }

  const char& operator[](const std::size_t index) const {
    if (index >= length) {
      throw std::out_of_range("Index out of bounds");
    }
    return data[index];
  }

  // Comparison operators
  bool operator==(const my_string& other) const {
    if (length != other.length) return false;
    if (!data && !other.data) return true;
    if (!data || !other.data) return false;
    return std::strcmp(data, other.data) == 0;
  }

  bool operator!=(const my_string& other) const {
    return !(*this == other);
  }

  bool operator<(const my_string& other) const {
    if (!data && !other.data) return false;
    if (!data) return true;
    if (!other.data) return false;
    return std::strcmp(data, other.data) < 0;
  }

  // Utility methods
  const char* c_str() const {
    return data ? data : "";
  }

  std::size_t size() const { return length; }
  std::size_t get_capacity() const { return capacity; }
  bool empty() const { return length == 0; }

  void clear() {
    if (data) {
      data[0] = '\0';
      length = 0;
    }
  }

  void reserve(std::size_t new_capacity) {
    if (new_capacity > capacity) {
      reallocate(new_capacity);
    }
  }

  // Find substring
  std::size_t find(const my_string& substr) const {
    if (!data || !substr.data) return npos;

    const char* pos = std::strstr(data, substr.data);
    if (pos) {
      return pos - data;
    }

    return npos;
  }

  // Substring
  my_string substr(std::size_t pos, std::size_t len = npos) {
    if (pos >= length) return my_string{};

    std::size_t actual_len = std::min(len, length - pos);
    my_string result{actual_len};
    std::strncpy(result.data, data + pos, actual_len);
    result.data[actual_len] = '\0';
    result.length = actual_len;

    return result;
  }

  // Print string info
  void print_info() const {
    std::cout << std::format("my_string \"{}\" (length: {}, capacity: {})\n", c_str(), length, capacity);
  }

  static const std::size_t npos = static_cast<std::size_t>(-1);
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const my_string& str) {
  os << str.c_str();
  return os;
}

// std::format support
template <>
struct std::formatter<my_string> {
  constexpr auto parse(std::format_parse_context& ctx) {
    return ctx.begin();
  }

  auto format(const my_string& str, std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{}", str.c_str());
  }
};

// Helper function to use std::print with my_string class
template<typename... Args>
void print_string(const my_string& fmt, Args&&... args) {
  if constexpr (sizeof...(args) == 0) {
    std::cout << fmt.c_str();
  } else {
    std::cout << std::vformat(fmt.c_str(), std::make_format_args(args...));
  }
}

int main ()
{
  std::print("=== Custom my_string Class - Rule of Five Demo ===\n\n");

  // Test std::format and print support
  std::print("\n0. Testing std::format support:\n");
  my_string greeting("Hello");
  my_string name("World");

  // Using std::format with our my_string class
  std::print("Formatted output: {} {}!\n", greeting, name);

  // Using our print_string helper
  print_string(my_string("Custom print: {} + {} = {}\n"), greeting, name, greeting + " " + name);

  // Default constructor
  std::print("1. Creating empty string:\n");
  my_string str1;
  str1.print_info();

  // C-string constructor
  std::print("\n2. Creating from C-string:\n");
  my_string str2("Hello");
  str2.print_info();

  // Copy constructor
  std::print("\n3. Copy constructor:\n");
  my_string str3(str2);
  str3.print_info();

  // Copy assignment
  std::print("\n4. Copy assignment:\n");
  my_string str4;
  str4 = str2;
  str4.print_info();

  // my_string operations
  std::print("\n5. my_string concatenation:\n");
  str2 += " World";
  str2.print_info();

  my_string str5 = str2 + "!";
  str5.print_info();

  // Move constructor
  std::print("\n6. Move constructor:\n");
  my_string str6(std::move(str2));
  str6.print_info();
  std::print("Original after move: ");
  str2.print_info();

  // Move assignment
  std::print("\n7. Move assignment:\n");
  my_string str7;
  str7 = std::move(str3);
  str7.print_info();
  std::print("Original after move: ");
  str3.print_info();

  // my_string operations demo
  std::print("\n8. my_string operations:\n");
  my_string demo("C++ Programming");
  demo.print_info();

  std::print("Character at index 4: '{}'\n", demo[4]);
  std::print("Substring from index 4: \"{}\"\n", demo.substr(4));
  std::print("Find 'Program': {}\n", demo.find(my_string("Program")));

  // Comparison
  std::print("\n9. my_string comparison:\n");
  my_string a("apple");
  my_string b("banana");
  std::print("\"{}\" == \"{}\": {}\n", a, b, (a == b));
  std::print("\"{}\" < \"{}\": {}\n", a, b, (a < b));

  std::print("\n=== End of demo ===\n");
  return 0;
}
