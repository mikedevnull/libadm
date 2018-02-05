#include <catch2/catch.hpp>
#include <adm/meta/meta.hpp>
#include <type_traits>
#include <string>

template <typename T>
using add_pointer = T*;

template <typename T>
using is_pointer = typename std::is_pointer<T>::type;

TEST_CASE("static_tests") {
  // this test case succeeds if it compiles.
  using namespace adm;
  using l1 = meta::mp_list<int, double, std::string>;
  using first = meta::mp_front<l1>;
  using tail = meta::mp_pop_front<l1>;
  static_assert(std::is_same<int, first>::value, "front got wrong type");
  static_assert(std::is_same<meta::mp_list<double, std::string>, tail>::value,
                "pop_front got wrong type");
  using final_tail = meta::mp_pop_front<meta::mp_pop_front<tail>>;
  static_assert(std::is_same<meta::mp_list<>, final_tail>::value,
                "typelist not empty as it should be");

  static_assert(meta::mp_empty<final_tail>::value,
                "is_empty not empty as it should");
  static_assert(!meta::mp_empty<l1>::value,
                "is_empty says list is empty althought it isn't");

  using ListAsTuple = meta::mp_rename<l1, std::tuple>;
  static_assert(
      std::is_same<ListAsTuple, std::tuple<int, double, std::string>>::value,
      "typelist could not be converted to tuple");
  ListAsTuple tuple;

  using AList = meta::mp_list<double, int, float>;
  using PtrList = meta::mp_transform<add_pointer, AList>;
  static_assert(
      std::is_same<PtrList, meta::mp_list<double*, int*, float*>>::value,
      "typelist transform with add_pointer failed");

  using searchList = meta::mp_list<int, double, double, std::string>;

  static_assert(meta::mp_size<searchList>::value == 4, "size of list wrong");
  static_assert(meta::mp_find<searchList, int>::value == 0,
                "failed to find index of int");
  static_assert(meta::mp_find<searchList, double>::value == 1,
                "failed to find index of double");
  static_assert(meta::mp_find<searchList, std::string>::value == 3,
                "failed to find index of string");
  // past-the-end index (==size) indicates "not found"
  static_assert(meta::mp_find<searchList, float>::value ==
                    meta::mp_size<searchList>::value,
                "search for non-existing type failed");
}
