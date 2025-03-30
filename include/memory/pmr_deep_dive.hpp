#pragma once

#include <algorithm>
#include <cctype>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <memory>

template <typename ItrBegin, typename ItrEnd>
void print_line(const int offset, ItrBegin begin, const ItrEnd end) {
  fmt::print("(dec) {:02x}:  {:3}\n", offset, fmt::join(begin, end, " "));
  fmt::print("(hex) {:02x}:   {:02x}\n", offset, fmt::join(begin, end, "   "));

  fmt::print("(asc) {:02x}:", offset);
  std::for_each(begin, end, [](const auto c) {
    if (std::isgraph(c)) {
      fmt::print("   {} ", static_cast<char>(c));
    } else {
      fmt::print(" \\{:03o}", c);
    }
  });
  fmt::print("\n");
}

template <typename Buffer, typename Container>
void print_buffer(const std::string_view title, const Buffer &buffer,
                  const Container &container) {
  fmt::print("======={:^25}=======\n", title);

  auto begin = buffer.begin();
  fmt::print("Buffer Address Start: {}\n",
             static_cast<const void *>(buffer.data()));

  for (const auto &e : container) {
    fmt::print(" Item Address: {}\n", static_cast<const void *>(&e));
  }

  for (auto offset = 0uz; offset < buffer.size(); offset+=16) {
    print_line(offset, std::next(begin, offset), std::next(begin, offset + 16));
  }

  fmt::print("\n");
}


// Define a custom allocator
template <typename T>
struct MyAllocator {
  using value_type = T;

  // Allocate memory
  T* allocate(size_t n) {
    return static_cast<T*>(std::malloc(n * sizeof(T)));
  }

  // Deallocate memory
  // size_t is to conform with standards of allocator requirements
  // where std::vector, std::list, etc. does
  void deallocate(T* p, [[maybe_unused]] size_t n) {
    std::free(p);
  }

  // Construct an object in memory
  void construct(T* p, const T& value) {
    new (p) T(value);
  }

  // Destroy an object in memory
  void destroy(T* p) {
    p->~T();
  }
};

// Allocator-aware class
template <typename T, typename Allocator = std::allocator<T>>
class AllocatorAwareClass {
public:
  using value_type = T;
  using allocator_type = Allocator;

  // Default constructor
  AllocatorAwareClass() : data_(nullptr), size_(0), allocator_() {}

  // Constructor that takes an allocator
  explicit AllocatorAwareClass(const allocator_type& allocator) : data_(nullptr), size_(0), allocator_(allocator) {}

  // Constructor that takes an allocator and an initial size
  AllocatorAwareClass(const allocator_type& allocator, size_t size) : data_(nullptr), size_(0), allocator_(allocator) {
    if (size > 0) {
      data_ = allocator_.allocate(size);
      size_ = size;
    }
  }

  // Destructor
  ~AllocatorAwareClass() {
    if (data_) {
      for (size_t i = 0; i < size_; ++i) {
        allocator_.destroy(&data_[i]);
      }
      allocator_.deallocate(data_, size_);
      data_ = nullptr;
      size_ = 0;
    }
  }

  // Get the allocator
  allocator_type get_allocator() const {
    return allocator_;
  }

  // Access data
  T& operator[](size_t index) {
    return data_[index];
  }

private:
  T* data_;
  size_t size_;
  allocator_type allocator_;
};
