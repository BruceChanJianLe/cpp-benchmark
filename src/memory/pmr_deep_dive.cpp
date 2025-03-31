#include "memory/pmr_deep_dive.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <memory_resource>
#include <string>
#include <vector>
#include <iostream>

#include <spdlog/spdlog.h>

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

/**
 * \brief more detail version of print_resource
 */
class print_resource2 : public std::pmr::memory_resource {
public:
  print_resource2(std::string name, std::pmr::memory_resource* upstream)
    : name_(std::move(name))
    , upstream_(upstream)
    {}

private:
  std::string name_;
  std::pmr::memory_resource* upstream_;

  void* do_allocate(std::size_t bytes, std::size_t alignment) override {
    spdlog::trace("[{} (alloc)] Size: {} Alignment: {} ...", name_, bytes,
        alignment);
    auto result = upstream_->allocate(bytes, alignment);
    spdlog::trace("[{} (alloc)] ... Address: {}", name_, result);
    return result;
  }

  std::string format_destroyed_bytes(std::byte* p, const std::size_t size) {
    std::string result = "";
    bool in_str = false;

    auto format_char = [](bool& in_string, const char c, const char next) {
      auto format_byte = [](const char byte) {
        return fmt::format(" {:02x}", static_cast<unsigned char>(byte));
      };

      if (std::isprint(static_cast<int>(c))) {
        if (!in_string) {
          if (std::isprint(static_cast<int>(next))) {
            in_string = true;
            return fmt::format(" \"{}", c);
          } else {
            return format_byte(c);
          }
        } else {
          return std::string(1, c);
        }
      } else {
        if (in_string) {
          in_string = false;
          return '"' + format_byte(c);
        }
        return format_byte(c);
      }
    };

    std::size_t pos = 0;
    for (; pos < std::min(size - 1, static_cast<std::size_t>(32)); ++pos) {
      result += format_char(in_str, static_cast<char>(p[pos]),
          static_cast<char>(p[pos + 1]));
    }

    result += format_char(in_str, static_cast<char>(p[pos]), 0);
    if (in_str) {
      result += '"';
    }
    if (pos < (size - 1)) {
      result += " <truncated...>";
    }
    return result;
  }

  void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
    spdlog::trace(
        "[{} (dealloc)] Address: {} Dealloc Size: {} Alignment: {} Data: {}",
        name_, p, bytes, alignment, 
        format_destroyed_bytes(static_cast<std::byte*>(p), bytes));
    upstream_->deallocate(p, bytes, alignment);
  }

  bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
    return this == &other;
  }
};

/**
 * \brief Different mix and match of pmr
 *        Notice that we have set the default resource to pmr::null_memory_resource
 *        this resource will throw a std::bad_alloc, it is a good way to check whether
 *        you have over used the allocated buffer and re-consider to increase it
 *        or do something appropriate about it.
 *
 *        For the below example, we have the follow resource flow
 *        unsync_pool -> monotoic -> null_memory_resource
 *
 *        You may play with the buffer size, at the moment it is 32kb,
 *        if you reduced it to 500 bytes then it will go to the upstream for new
 *        resource which here is the null_memory_resource and will throw a
 *        std::bad_alloc.
 */
void mix_and_match_pmr() {
  spdlog::set_level(spdlog::level::trace);

  print_resource2 default_alloc{"Rouge PMR Allocation!", std::pmr::null_memory_resource()};
  std::pmr::set_default_resource(&default_alloc);

  print_resource2 oom{"Out of Memory", std::pmr::null_memory_resource()};

  // 32kilo bytes
  std::array<std::uint8_t, 32768> buffer{};
  std::pmr::monotonic_buffer_resource underlying_bytes{
    buffer.data(), buffer.size(), &oom};
  print_resource2 monotonic{"Monotonic Array", &underlying_bytes};

  // Lastly, chain to unsynchronize pool
  std::pmr::unsynchronized_pool_resource unsync_pool(&monotonic);

  print_resource2 pool{"Pool", &unsync_pool};

  for (auto i = 0; i < 10; ++i) {
    spdlog::debug("Starting Loop Iteration");
    auto vec = create_contianer<std::pmr::vector<std::pmr::string>>(
        &pool, "Hello", "World", "Hello Long String", "Another Long String");

    spdlog::trace("Emplacing Long String");
    vec.emplace_back("a different long string");

    spdlog::trace("Emplacing Long String");
    vec.emplace_back("a different long string 1");

    spdlog::trace("Emplacing Long String");
    vec.emplace_back("a different long string 2");

    spdlog::trace("Emplacing Long String");
    vec.emplace_back("a different long string 3");

    spdlog::trace("Emplacing Short String");
    vec.emplace_back("bobby");

    spdlog::trace("Emplacing Short String");
    vec.emplace_back("washy");

    spdlog::trace("Erasing First Element");
    vec.erase(vec.begin());

    spdlog::trace("Erasing First Element");
    vec.erase(vec.begin());

    spdlog::trace("Erasing First Element");
    vec.erase(vec.begin());

    spdlog::debug("Finishing Loop Iteration");
  }

  spdlog::debug("Exiting function");
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

  fmt::print("\n\n\n======={:^25}=======\n\n\n", "Mix and Match PMR");
  mix_and_match_pmr();

  return EXIT_SUCCESS;
}
