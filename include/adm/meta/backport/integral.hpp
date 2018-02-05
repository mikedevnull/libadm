#pragma once
#include <type_traits>

namespace adm {
  namespace meta {

    template <bool B>
    using mp_bool = std::integral_constant<bool, B>;

    template <typename T>
    using mp_not = mp_bool<!T::value>;

    template <std::size_t N>
    using mp_size_t = std::integral_constant<std::size_t, N>;
  }  // namespace meta
}  // namespace adm
