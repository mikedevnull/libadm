#pragma once
#include <type_traits>
#include <adm/meta/backport/integral.hpp>

namespace adm {

  /**
   * @brief Poor man's meta programming tools
   *
   * @sa metaprogramming
   */
  namespace meta {

    /**
     * @defgroup metaprogramming Poor man's meta programming tools
     *
     * @brief Metaprogramming tools for the adm library
     *
     * Started out by implementing some ideas from:
     * http://www.pdimov.com/cpp2/simple_cxx11_metaprogramming.html
     *
     * In the meantime, this has been released as a boost library.
     * So we keep our implmentation subset for systems with older boost
    version,
     * but use boost::mp11 if available.
     *
     * Thus, we only need to implement what we actually need ( which is not
    much),
     * but this should be compatible with the boost::mp11 version.
     *
     * See http://www.boost.org/doc/libs/1_66_0/libs/mp11/doc/html/mp11.html
     */

    /**
      * @headerfile typelist.hpp <adm/meta/backport/list.hpp>
      * @ingroup metaprogramming
      * @brief Very simple C++11 typelist for meta programming
      *
      *
      * Creating a typelist is as simple as creating an alias:
      * @code
        using MyTypes = mp_list<double, int, std:string>;
        @endcode
      * @sa metaprogramming
      */
    template <typename... T>
    class mp_list;

    /**
     * @headerfile typelist.hpp <adm/meta/backport/list.hpp>
     * @ingroup metaprogramming
     * @brief Check if TypeList is empty
     *
     * If given an empty Typelist (i.e. TypeList<>), this
     * inherits `std::true_type`.
     * Otherwise, it inherits `std::false_type`.
     @code
     using MyTypes = TypeList<>;
     if(is_empty_list<MyTypes>::value) {
       // ... if branch always selected
     }
     @endcode
     * @sa TypeList
     */
    template <typename L>
    using mp_empty = std::is_same<mp_list<>, L>;

    // ------------------------- //

    template <typename L, typename... T>
    struct pop_front_impl {};

    template <template <typename...> class L, typename... U, typename T>
    struct pop_front_impl<L<T, U...>> {
      using type = L<U...>;
    };

    /**
     * @headerfile typelist.hpp <adm/meta/backport/list.hpp>
     * @ingroup metaprogramming
     * @brief Remove first element of a TypeList
     *
     * If given an Typelist, this
     * "returns" a modifed typelist with the front type removed.
     @code
     using MyTypes = TypeList<int, double>;
     using TestTypes = TypeList<int>;
     static_assert(std::is_same<TestType, pop_front<MyTypes>);
     @endcode
     * @sa TypeList
     */
    template <class L>
    using mp_pop_front = typename pop_front_impl<L>::type;

    // ------------------------- //

    template <typename L, typename... T>
    struct front_impl {};

    template <template <class...> class L, typename... U, typename T>
    struct front_impl<L<T, U...>> {
      using type = T;
    };

    /**
     * @headerfile typelist.hpp <adm/meta/backport/list.hpp>
     * @ingroup metaprogramming
     * @brief Get first element of TypeList
     *
     @code
     using FirstType = front<TypeList<double,int>>;
     static_assert(std::is_same<FirstType, double>);
     @endcode
     * @sa TypeList
     */
    template <class L>
    using mp_front = typename front_impl<L>::type;

    // ------------------------- //

    template <class L>
    struct list_size_impl;

    template <class L>
    using mp_size = typename list_size_impl<L>::type;

    template <template <class...> class L>
    struct list_size_impl<L<>> {
      // terminal condition,  emtpy lists
      using type = mp_size_t<0>;
    };

    template <template <class...> class L, class T1, class... T>
    struct list_size_impl<L<T1, T...>> {
      // increase by 1 until terminal condition was encountered
      template <std::size_t N>
      using size_type = mp_size_t<N>;
      using type = size_type<1 + mp_size<L<T...>>::value>;
    };

    // ------------------------- //

    template <class... L>
    struct append_impl;

    template <template <class...> class L, class... T1, class... T2>
    struct append_impl<L<T1...>, L<T2...>> {
      using type = L<T1..., T2...>;
    };

    template <template <class...> class L, class... T1, class... T2,
              class... Tn>
    struct append_impl<L<T1...>, L<T2...>, Tn...> {
      using type = typename append_impl<L<T1..., T2...>, Tn...>::type;
    };

    template <typename L1, typename... L2>
    using mp_append = typename append_impl<L1, L2...>::type;

  }  // namespace meta
}  // namespace adm
