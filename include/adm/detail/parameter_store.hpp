#pragma once
#include <adm/meta/meta.hpp>
#include <adm/detail/type_traits.hpp>
#include <tuple>
#include <boost/optional.hpp>

namespace adm {
  namespace detail {
    struct NoDefaultValues {
      using ParametersWithDefaults = meta::mp_list<>;
    };

    template <typename ManditoryParameters, typename OptionalParameters,
              typename DefaultValues = NoDefaultValues>
    class ParameterStore {
      template <typename T>
      using add_optional = boost::optional<T>;

     public:
      using Parameters =
          meta::mp_append<ManditoryParameters, OptionalParameters>;

      using ParametersWithDefaults =
          typename DefaultValues::ParametersWithDefaults;

      template <typename Parameter>
      using hasDefaultValue =
          meta::mp_contains<ParametersWithDefaults, Parameter>;

      template <typename Parameter>
      using isParameterValue = meta::mp_contains<Parameters, Parameter>;

      template <typename Parameter>
      using isOptionalValue = meta::mp_contains<OptionalParameters, Parameter>;

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
          meta::mp_append<ManditoryParameters,
                          meta::mp_transform<add_optional, OptionalParameters>>;

      template <typename Parameter, typename PropertyList,
                typename StorageLookup>
      using StoredParameterType =
          typename meta::mp_at<PropertyList,
                               meta::mp_find<StorageLookup, Parameter>>;

      template <typename Parameter>
      StoredParameterType<Parameter, StoredTypes, Parameters>&
      lookupInStorage() {
        using ParamIndex = meta::mp_find<Parameters, Parameter>;
        return std::get<ParamIndex::value>(storage_);
      }

      template <typename Parameter>
      const StoredParameterType<Parameter, StoredTypes, Parameters>&
      lookupInStorage() const {
        using ParamIndex = meta::mp_find<Parameters, Parameter>;
        return std::get<ParamIndex::value>(storage_);
      }

      using Storage = meta::mp_rename<StoredTypes, std::tuple>;
      Storage storage_;
    };
  }  // namespace detail

  template <typename... T>
  using ParameterList = meta::mp_list<T...>;
}  // namespace adm
