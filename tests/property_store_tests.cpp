#include <catch2/catch.hpp>
#include "adm/detail/property_store.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/meta/list.hpp"
#include <boost/optional.hpp>

struct NumericPropertyTag {};
using NumericProperty = adm::detail::NamedType<int, NumericPropertyTag>;

struct NameTag {};
using Name = adm::detail::NamedType<std::string, NameTag>;

struct FictionalScaleTag {};
using FictionalScale = adm::detail::NamedType<float, FictionalScaleTag>;

struct UnusedTag {};
using Unused = adm::detail::NamedType<int, UnusedTag>;

TEST_CASE("basic_store") {
  using namespace adm;

  using Properties = meta::mp_list<Name, NumericProperty>;
  using OptionalProperties = meta::mp_list<FictionalScale>;
  using Storage = PropertyStore<Properties, OptionalProperties>;

  Storage store;
  static_assert(Storage::isParameterValue<Name>::value,
                "Name should be a valid parameter");
  static_assert(Storage::isParameterValue<NumericProperty>::value,
                "NumericProperty should be a valid parameter");
  static_assert(Storage::isParameterValue<FictionalScale>::value,
                "FictionalScale should be a valid parameter");
  static_assert(!Storage::isParameterValue<Unused>::value,
                "Unused should not be a valid parameter");
  store.set(Name("MyName"));
}

TEST_CASE("access_optional") {
  using namespace adm;

  using Properties = meta::mp_list<Name, NumericProperty>;
  using OptionalProperties = meta::mp_list<FictionalScale>;
  using Storage = PropertyStore<Properties, OptionalProperties>;

  Storage store;
  static_assert(Storage::isParameterValue<FictionalScale>::value,
                "FictionalScale should be a valid parameter");

  REQUIRE(store.has<FictionalScale>() == false);

  store.set(FictionalScale{0.9f});
  REQUIRE(store.has<FictionalScale>() == true);
  FictionalScale scale = store.get<FictionalScale>();
  REQUIRE(scale.get() == 0.9f);
  store.unset<FictionalScale>();
  REQUIRE(store.has<FictionalScale>() == false);
}

struct SomeDefaultValues {
  using PropertiesWithDefaults = adm::meta::mp_list<Name>;
  static Name create(Name::tag) { return Name("mynameis"); }
};

TEST_CASE("default_values") {
  using namespace adm;

  using Properties = meta::mp_list<NumericProperty>;
  using OptionalProperties = meta::mp_list<Name, FictionalScale>;
  using Storage =
      PropertyStore<Properties, OptionalProperties, SomeDefaultValues>;

  Storage store;

  REQUIRE(store.has<Name>() == true);
  REQUIRE(store.get<Name>() == Name("mynameis"));
  store.set(Name("it's my name"));
  REQUIRE(store.has<Name>() == true);
  REQUIRE(store.get<Name>() == Name("it's my name"));
  store.unset<Name>();
  REQUIRE(store.has<Name>() == true);
  REQUIRE(store.get<Name>() == Name("mynameis"));
}
