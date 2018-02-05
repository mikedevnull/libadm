#pragma once
#include <adm/meta/meta.hpp>
#include <adm/detail/type_traits.hpp>
#include <tuple>
#include <boost/optional.hpp>

namespace adm {

  struct NoDefaultValues {
    using PropertiesWithDefaults = meta::mp_list<>;
  };

  template <typename ManditoryProperties, typename OptionalProperties,
            typename DefaultValues = NoDefaultValues>
  class PropertyStore {
    template <typename T>
    using add_optional = boost::optional<T>;

   public:
    using Properties = meta::mp_append<ManditoryProperties, OptionalProperties>;

    using PropertiesWithDefaults =
        typename DefaultValues::PropertiesWithDefaults;

    template <typename Parameter>
    using hasDefaultValue =
        meta::mp_contains<PropertiesWithDefaults, Parameter>;

    template <typename Parameter>
    using isParameterValue = meta::mp_contains<Properties, Parameter>;

    template <typename Parameter>
    using isOptionalValue = meta::mp_contains<OptionalProperties, Parameter>;

    template <typename Parameter>
    bool isDefault() const {
      return hasDefaultValue<Parameter>::value &&
             !isSet(lookupInStorage<Parameter>(),
                    typename isOptionalValue<Parameter>::type());
    }

    template <typename Parameter>
    Parameter get() const {
      return getValue(lookupInStorage<Parameter>(),
                      typename hasDefaultValue<Parameter>::type());
    }

    template <typename Parameter>
    void set(const Parameter& value) {
      lookupInStorage<Parameter>() = value;
    }

    template <typename Parameter>
    bool has() const {
      return hasDefaultValue<Parameter>::value ||
             isSet(lookupInStorage<Parameter>(),
                   typename isOptionalValue<Parameter>::type());
    }

    template <typename Parameter>
    void unset() {
      lookupInStorage<Parameter>() = boost::none;
    }

   private:
    template <typename T>
    bool isSet(const T&, std::false_type) const {
      return true;
    }
    template <typename T>
    bool isSet(const boost::optional<T>& v, std::true_type) const {
      return static_cast<bool>(v);
    }

    template <typename T>
    T getValue(const T& v, std::false_type) const {
      return v;
    }
    template <typename T>
    T getValue(const boost::optional<T>& v, std::false_type) const {
      return v.get();
    }
    template <typename T>
    T getValue(const boost::optional<T>& v, std::true_type) const {
      return v ? v.get() : dispatchCreateDefault<T>();
    }

    template <typename T>
    static T dispatchCreateDefault() {
      typedef typename detail::ParameterTraits<T>::tag Tag;
      return DefaultValues::create(Tag());
    }

    using StoredTypes =
        meta::mp_append<ManditoryProperties,
                        meta::mp_transform<add_optional, OptionalProperties>>;

    template <typename Parameter, typename PropertyList, typename StorageLookup>
    using StoredParameterType =
        typename meta::mp_at<PropertyList,
                             meta::mp_find<StorageLookup, Parameter>>;

    template <typename Parameter>
    StoredParameterType<Parameter, StoredTypes, Properties>& lookupInStorage() {
      using ParamIndex = meta::mp_find<Properties, Parameter>;
      return std::get<ParamIndex::value>(storage_);
    }

    template <typename Parameter>
    const StoredParameterType<Parameter, StoredTypes, Properties>&
    lookupInStorage() const {
      using ParamIndex = meta::mp_find<Properties, Parameter>;
      return std::get<ParamIndex::value>(storage_);
    }

    using Storage = meta::mp_rename<StoredTypes, std::tuple>;
    Storage storage_;
  };

  template <typename... T>
  using PropertyList = meta::mp_list<T...>;
}  // namespace adm
