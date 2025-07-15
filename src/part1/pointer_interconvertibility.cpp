#include <cstdio>
#include <print>
#include <string>
#include <tuple>
#include <utility>

struct A {
  std::string name;
  int age;
};

template <typename T>
struct DataType {
  using type = T;
  DataType(const std::string& _name, const T& _default_value)
    : name{_name}, default_value{_default_value}, value{_default_value}
  {}

  std::string name;
  T default_value;
  T value;

  std::string get_name() const { return name; }
  T get_value() const { return value; }
  T get_default_value() const { return default_value; }
  T *set_value() { return &value; }
};

auto getTuple () {
  return std::make_tuple(
      DataType<std::string>("name", "name"),
      DataType<int>("age", 20)
    );
}

template <typename S, typename T, std::size_t... index>
void print(S& s, T& t, std::index_sequence<index...>) {
  void* p = &s;

    ([&]() {
      auto* e = reinterpret_cast<std::tuple_element_t<index, T>::type*>(p);
      std::println("{}: {}",
          std::get<index>(t).get_name(),
          *e);
      p = reinterpret_cast<void*>(e + 1);
    }(), ...);
}

template <typename S, typename... Args>
void walk(S& s, std::tuple<Args...>&& t) {
  print(s, t, std::index_sequence_for<Args...>{});
}

int main ()
{
  A a;
  a.name = "Bruce";
  a.age = 31;
  walk(a, getTuple());
  return 0;
}
