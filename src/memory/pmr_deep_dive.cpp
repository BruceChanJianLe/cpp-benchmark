#include "memory/pmr_deep_dive.hpp"

#include <array>
#include <cstdint>
#include <cstdlib>
#include <memory_resource>
#include <string>
#include <vector>

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

  return EXIT_SUCCESS;
}
