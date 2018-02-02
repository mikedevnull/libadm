#pragma once
#include <type_traits>

namespace adm {
  namespace meta {

    // see adm/meta/backport/list.hpp for more information

    template <class A, template <class...> class B>
    struct rename_impl {};

    template <template <class...> class A, class... T,
              template <class...> class B>
    struct rename_impl<A<T...>, B> {
      using type = B<T...>;
    };

    /**
     * @headerfile algorithm.hpp <adm/meta/algorithm.hpp>
     * @ingroup metaprogramming
     * @brief Converts a typelist to another type
     *
     * Converts a typelist of type `A` to a new type `B`
     * holding the same list of types.
     @code
     using List = mp_list<double, int>;
     using Tuple = mp_rename<List, std::tuple>;

     static_assert(std::is_same<Tuple, std:tuple<double, int>>);
     @endcode
     */
    template <class A, template <class...> class B>
    using mp_rename = typename rename_impl<A, B>::type;

    // ------------------------- //

    template <template <class...> class F, class L>
    struct transform_impl {};

    template <template <class...> class F, template <class...> class L,
              class... T>
    struct transform_impl<F, L<T...>> {
      using type = L<F<T>...>;
    };

    /**
     * @headerfile algorithm.hpp <adm/meta/algorithm.hpp>
     * @ingroup metaprogramming
     * @brief Apply a metafunction to each type in the typelist.
     *
     * A `metafunction` in our case can be as simple as a type alias.
     * For example
     @code
     template<class T> using add_pointer = T*;
     @endcode
     is a metafunction that turns any `T` into `T*`.

     Now, this metafunction can be applied to a full list of types:
     @code
     using AList = mp_list<double, int, float>;
     using PtrList = mp_transform<add_pointer, AList>;

     static_assert(std::is_same<PtrList, mp_list<double*, int*, float*>>)
     @endcode
     */
    template <template <class...> class F, class L>
    using mp_transform = typename transform_impl<F, L>::type;

    // ------------------------- //

    template <class L, class V>
    struct index_of_first_impl;

    template <class L, class V>
    using mp_find = typename index_of_first_impl<L, V>::type;

    template <template <class...> class L, class V>
    struct index_of_first_impl<L<>, V> {
      // terminal condition,  emtpy lists
      using type = std::integral_constant<std::size_t, 0>;
    };

    template <template <class...> class L, class V, class... T>
    struct index_of_first_impl<L<V, T...>, V> {
      // terminal condition, type found
      using type = std::integral_constant<std::size_t, 0>;
    };

    template <template <class...> class L, class V, class T1, class... T>
    struct index_of_first_impl<L<T1, T...>, V> {
      // increase by 1 until terminal condition was encountered
      template <std::size_t N>
      using size_type = std::integral_constant<std::size_t, N>;
      using type = size_type<1 + mp_find<L<T...>, V>::value>;
    };

  }  // namespace meta
}  // namespace adm
