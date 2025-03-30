#include "memory/pmr_deep_dive.hpp"

#include <array>
#include <cstdint>
#include <cstdlib>
#include <memory_resource>
#include <string>
#include <vector>
#include <iostream>

void freed_resources() {
  // 16 byte for allocation
  std::array<std::uint8_t, 16> buffer{};
  // Create a memory resource that allocates from 'buffer'
  std::pmr::monotonic_buffer_resource memory_resource(buffer.data(), buffer.size());

  print_buffer("initial", buffer, "");
  // Use this resource to allocate a std::pmr::vector
  std::pmr::vector<std::uint8_t> vec1({1,2,3,4}, &memory_resource);
  print_buffer("vec1", buffer, vec1);

  {
    std::pmr::vector<std::uint8_t> vec2({6,7,8}, &memory_resource);
    print_buffer("vec1 + vec2", buffer, vec2);
    // Notice that memory vec2 not reclaim from the buffer
  }

  std::pmr::vector<std::uint8_t> vec3({10,11,12}, &memory_resource);
  print_buffer("vec1 + vec2 + vec3", buffer, vec3);
}

/**
 * \brief Notice that memory grows without freeing
 */
void growing_resources() {
  // 16 byte for allocation
  std::array<std::uint8_t, 16> buffer{};
  // Create a memory resource that allocates from the 'buffer'
  std::pmr::monotonic_buffer_resource memory_resource(buffer.data(), buffer.size());
  std::pmr::vector<std::uint8_t> vec1(&memory_resource);
  vec1.push_back(1);
  print_buffer("1", buffer, vec1);
  vec1.push_back(2);
  print_buffer("2", buffer, vec1);
  vec1.push_back(3);
  print_buffer("3", buffer, vec1);
  vec1.push_back(4);
  print_buffer("4", buffer, vec1);
  vec1.push_back(5);
  print_buffer("5", buffer, vec1);
}

/**
 * \brief Short string, non pmr
 *        Notice how the two short strings fit nicely into the memory for
 *        the fisrt example 2 short strings. sizeof string 32, our string
 *        length is 13. This is done with SSO (Short String Optimization)
 *        However, for the 2 short and 1 long strings, the long string is
 *        on the heap since it cannot fit 32 bytes
 */
void nested_string() {
  fmt::print("Size of `std::string`: {}\n", sizeof(std::string));
  std::array<std::uint8_t, 128> buffer1{};
  std::pmr::monotonic_buffer_resource memory_resource(buffer1.data(), buffer1.size());
  print_buffer("initial", buffer1, "");
  std::pmr::vector<std::string> vec1({"Hello World 1", "Hello World 2"}, &memory_resource);
  print_buffer("2short strings", buffer1, vec1);

  std::array<std::uint8_t, 128> buffer2{};
  std::pmr::monotonic_buffer_resource memory_resource2(buffer2.data(), buffer2.size());
  std::pmr::vector<std::string> vec2({"Hello World 1", "Hello World 2", "This is a long stringggg"}, &memory_resource2);
  print_buffer("2short and 1long strings", buffer2, vec2);
}

/**
 * \brief Short string, std::pmr::string
 *        Notice that 128 bytes it not enough for all three strings,
 *        hence, increased to 256.
 *        the fisrt example 2 short strings. sizeof string 32, our string
 *        length is 13. This is done with SSO (Short String Optimization)
 *        However, for the 2 short and 1 long strings, the long string is
 *        on the heap since it cannot fit 32 bytes
 */
void nested_string_pmr() {
  fmt::print("Size of `std::pmr::string`: {}\n", sizeof(std::pmr::string));
  std::array<std::uint8_t, 256> buffer{};
  std::pmr::monotonic_buffer_resource memory_resource(buffer.data(), buffer.size());
  print_buffer("initial", buffer, "");
  std::pmr::vector<std::pmr::string> vec1({"Hello World 1", "Hello World 2", "This is a long stringggg"}, &memory_resource);
  print_buffer("2short and 1long pmr strings", buffer, vec1);
}

/**
 * \brief example for 
 */
struct S
{
  /// Member variable
  std::pmr::string str = "default_value";

  /// This is required for the class to be allocator aware
  using allocator_type = std::pmr::polymorphic_allocator<>;

  /// Default constructor, delegate to allocator aware constructor
  S() = default;
  // S() : S(allocator_type{}) {}

  explicit S(allocator_type alloc) : str("allocator_constructor", alloc) {}

  /// Copy constructor
  S(const S &other, allocator_type alloc = {})
    : str(other.str, alloc)
    {
      ;
    }

  /// Move constructor
  S(S &&) = default;

  /// Move constructor, with allocator
  S(S &&other, allocator_type alloc)
    : str(std::move(other.str), alloc)
  {}

  /// Copy assignment
  S &operator=(const S &rhs) = default;

  /// Move assignment
  S &operator=(S &&rhs) = default;

  ~S() = default;

  allocator_type get_allocator() const {
    return str.get_allocator();
  }
};
/**
 * \brief Allocator aware class!
 *        If you would like to use pmr data in your class, these are the
 *        things that you would have to pay attention to.
 *        Notice that there may be memory fragmentation happening ref to below
 *        vid: https://www.youtube.com/watch?v=2LAsqp7UrNs&t=873s
 *
 *        Or look at the output of the code, after updating vec1[0].str, the
 *        address is pointed to somewhere else in the memory down below.
 */
void allocator_aware_class() {
  fmt::print("Size of `S()`: {}\n", sizeof(S));

  std::array<std::uint8_t, 304> buffer{};
  std::pmr::monotonic_buffer_resource memory_resource(buffer.data(), buffer.size());

  fmt::print("memory resource address: {}\n", static_cast<const void *>(&memory_resource));
  print_buffer("initial", buffer, "");

  std::pmr::vector<S> vec1(&memory_resource);
  vec1.push_back(S());
  print_buffer("1 S Object", buffer, vec1);

  vec1.emplace_back();
  print_buffer("2 S Object", buffer, vec1);

  // A different long string
  vec1[0].str = "A different long string";
  print_buffer("Updated string", buffer, vec1);
}

/**
 * \brief A helpful wrapper for debugging where pmr allocator is really
 *        being used.
 */
class print_resource : public std::pmr::memory_resource {
private:
  void* do_allocate(std::size_t bytes, std::size_t alignment) override {
    std::cout << "Allocating " << bytes << '\n';
    return std::pmr::new_delete_resource()->allocate(bytes, alignment);
  }

  void do_deallocate(void *p, std::size_t bytes, std::size_t alignment) override {

    std::cout << "Deallocating " << bytes << ": '";
    for (auto i = 0uz; i < bytes; ++i) {
      std::cout << *(static_cast<char*>(p) + i);
    }
    std::cout << "'\n";

    return std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
  }

  bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override {
    return std::pmr::new_delete_resource()->is_equal(other);
  }
};

void initializer_list_and_pmr() {
  // Initializer list are broken (memory allocation happens and they are being
  // copied over when std::initializer_list<> is used)
  // vid: https://www.youtube.com/watch?v=sSlmmZMFsXQ

  // Set our memory resource wrapper as default
  print_resource default_memory_resource;
  std::pmr::set_default_resource(&default_memory_resource);

  std::array<std::uint8_t, 8> small_buffer{};
  std::pmr::monotonic_buffer_resource small_memory_resource{small_buffer.data(), small_buffer.size()};

  std::cout << "Small buffer pmr vector\n";
  std::pmr::vector<std::pmr::string> vec{{"Hello", "World"}, &small_memory_resource};

  std::array<std::uint8_t, 1024> big_buffer{};
  std::pmr::monotonic_buffer_resource big_memory_resource{big_buffer.data(), big_buffer.size()};

  std::cout << "Scope Starts!\n\n";
  {
    // Notice "Hello Long String" is allocated with our wrapper memory resource
    std::pmr::vector<std::pmr::string> vec2{{"Hello", "World", "Hello Long String"}, &big_memory_resource};
    std::cout << "vec2 created!!\n";

    // Notice "Another long string created!" is also allocated
    // with default memory resource, because for vector push back, it needs to
    // first create a pmr::string, which is on the using our wrapper memory resource
    // then it copies over, it cannot be moved since it is from a different memory resource!
    vec2.push_back("Another long string created!");

    // The one way for us is to use emplace_back!
    // Notice wrapper memory resource is not used
    vec2.emplace_back("weehee but please used reserve first!");
  }
  std::cout << "Scope Ends!\n\n";

  std::cout << "Exit function!\n";
}

// A better way is to write a helper function that emplace_back all values
template <typename Container, typename... Values>
auto create_contianer(auto *resource, Values&&... values) {
  Container result{resource};
  result.reserve(sizeof...(values));
  (result.emplace_back(std::forward<Values>(values)), ...);
  return result;
}
void no_initializer_list_pmr() {
  // Set our memory resource wrapper as default
  print_resource default_memory_resource;
  std::pmr::set_default_resource(&default_memory_resource);

  std::array<std::uint8_t, 1024> buffer{};
  std::pmr::monotonic_buffer_resource memory_resource{buffer.data(), buffer.size()};

  std::cout << "Start of function!\n";
  auto vec = create_contianer<std::pmr::vector<std::pmr::string>>(&memory_resource,
      "Hello", "World", "Hello Long String", "Another long string created!",
      "weehee but please used reserve first!");
  std::cout << "Exit of function!\n";
}

int main ([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  fmt::print("\n\n\n======={:^25}=======\n\n\n", "Freed Resources");
  freed_resources();

  fmt::print("\n\n\n======={:^25}=======\n\n\n", "Growing Resources");
  growing_resources();

  fmt::print("\n\n\n======={:^25}=======\n\n\n", "Short String Non-PMR");
  nested_string();

  fmt::print("\n\n\n======={:^25}=======\n\n\n", "Short String PMR");
  nested_string_pmr();

  fmt::print("\n\n\n======={:^25}=======\n\n\n", "Allocator Aware Class");
  allocator_aware_class();

  fmt::print("\n\n\n======={:^25}=======\n\n\n", "Initializer List & PMR");
  initializer_list_and_pmr();

  fmt::print("\n\n\n======={:^25}=======\n\n\n", "No Initializer List & PMR");
  no_initializer_list_pmr();

  return EXIT_SUCCESS;
}
