#pragma once

#include <algorithm>
#include <cctype>
#include <fmt/core.h>
#include <fmt/ranges.h>

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

template <typename Buffer, typename Container>
void print_buffer20(const std::string_view title, const Buffer &buffer,
                  const Container &container) {
  fmt::print("======={:^25}=======\n", title);

  auto begin = buffer.begin();
  fmt::print("Buffer Address Start: {}\n",
             static_cast<const void *>(buffer.data()));

  for (const auto &e : container) {
    fmt::print(" Item Address: {}\n", static_cast<const void *>(&e));
  }

  for (auto offset = 0uz; offset < buffer.size(); offset+=20) {
    print_line(offset, std::next(begin, offset), std::next(begin, offset + 20));
  }

  fmt::print("\n");
}
