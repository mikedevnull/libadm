#pragma once
#include <adm/meta/meta.hpp>
#include <adm/detail/type_traits.hpp>
#include <tuple>
#include <boost/optional.hpp>

namespace adm {

  template <typename... T>
  using ParameterList = meta::mp_list<T...>;

  using NoParameters = meta::mp_list<>;

  namespace detail {
    struct NoDefaultValues {
      using ParametersWithDefaults = meta::mp_list<>;
    };

    /**
     * @brief Implements logic to store and handle ADM parameter
     *
     * Parameters of ADM elements can be one of the following:
     *   - Manditory - Must always be present
     *   - Optional - May not be present
     *   - Optional with default value - If not set explicitly, the default
     * value will be used
     *
     * Instead of repeating the logic to deal with these three types over and
     * over again, this helper class aims to provide a single implementation
     * that can be used for all ADM elements.
     *
     * As a bonus, using a typelist approach to describe enables various other
     * useful additions, for example improved error messages or implementing
     * runtime iteration over all parameters of an element.
     *
     * @tparam ManditoryParameters A `ParameterList` of all manditory parameters
     * @tparam OptionalParameters A `ParameterList` of all optional parameters
     * without default value
     * @tparam ParametersWithDefaults A `ParameterList` of all optional
     * parameters with default value
     */
    template <typename ManditoryParameters, typename OptionalParameters,
              typename ParametersWithDefaults = NoParameters,
              typename DefaultValueCreator = NoDefaultValues>
    class ParameterStore {
      // list of all parameters that are stored as boost::optional<T>
      using AllOptionalParameters =
          meta::mp_append<OptionalParameters, ParametersWithDefaults>;
      // list of all parameters, both manditory and optional
      using Parameters =
          meta::mp_append<ManditoryParameters, AllOptionalParameters>;

      // helper to calculate if a given parameter has a default value
      template <typename Parameter>
      using hasDefaultValue =
          meta::mp_contains<ParametersWithDefaults, Parameter>;

      // helper to calculate if a given parameter is optional
      template <typename Parameter>
      using isOptionalValue =
          meta::mp_contains<AllOptionalParameters, Parameter>;

     public:
      /**
       * @brief Check parameter is one of the parameters of this store
       *
       * Evaluates to a true integral_type if the parameter is either one of the
       manditory or optional parameters.
       */
      template <typename Parameter>
      using isValidParameter = meta::mp_contains<Parameters, Parameter>;

      /**
       * @brief Check if the given parameter is currently set to its default
       * value.
       *
       * If the parameter does not have a default value, this method always
       * returns false;
       */
      template <typename Parameter>
      bool isDefault() const {
        return hasDefaultValue<Parameter>::value &&
               !isSet(lookupInStorage<Parameter>(),
                      typename isOptionalValue<Parameter>::type());
      }

      /**
       * @brief Get the current value of a parameter
       *
       * @warning
       * If the parameter is an optional parameter without default value,
       * callers of this method need to check beforehand if a valid value is
       * currently set. This can be done using the `has()` member function.
       */
      template <typename Parameter>
      Parameter get() const {
        return getValue(lookupInStorage<Parameter>(),
                        typename hasDefaultValue<Parameter>::type());
      }

      /**
       * @brief Set the value of a parameter
       */
      template <typename Parameter>
      void set(const Parameter& value) {
        lookupInStorage<Parameter>() = value;
      }

      /**
       * @brief Check if a value is set for a parameter
       *
       * This is always true for manditory parameters and parameters with
       * default values.
       */
      template <typename Parameter>
      bool has() const {
        return hasDefaultValue<Parameter>::value ||
               isSet(lookupInStorage<Parameter>(),
                     typename isOptionalValue<Parameter>::type());
      }

      /**
       * @brief Unset the value of a parameter
       *
       * This is only possible for optional parameters.
       */
      template <typename Parameter>
      void unset() {
        lookupInStorage<Parameter>() = boost::none;
      }

     private:
      // some tags to make method dispatching more expressive
      struct tags {
        using OptionalParameter = std::true_type;
        using ManditoryParameter = std::false_type;
        using ParameterWithDefault = std::true_type;
        using ParameterWithoutDefault = std::false_type;
      };

      template <typename T>
      bool isSet(const T&, typename tags::ManditoryParameter) const {
        return true;
      }
      template <typename T>
      bool isSet(const boost::optional<T>& v,
                 typename tags::OptionalParameter) const {
        return static_cast<bool>(v);
      }

      template <typename T>
      T getValue(const T& v, typename tags::ParameterWithoutDefault) const {
        return v;
      }
      template <typename T>
      T getValue(const boost::optional<T>& v,
                 typename tags::ParameterWithoutDefault) const {
        return v.get();
      }
      template <typename T>
      T getValue(const boost::optional<T>& v,
                 typename tags::ParameterWithDefault) const {
        return v ? v.get() : dispatchCreateDefault<T>();
      }

      template <typename T>
      static T dispatchCreateDefault() {
        typedef typename detail::ParameterTraits<T>::tag Tag;
        return DefaultValueCreator::create(Tag());
      }

      template <typename T>
      using add_optional = boost::optional<T>;

      // List of the actual types to be stored
      using StoredTypes = meta::mp_append<
          ManditoryParameters,
          meta::mp_transform<add_optional, AllOptionalParameters>>;

      // Metafunction to calculate the correct return type for a parameter,
      // which is either `Parameter` or `boost::optional<Parameter>`
      template <typename Parameter, typename ParameterList,
                typename StorageLookup>
      using StoredParameterType =
          typename meta::mp_at<ParameterList,
                               meta::mp_find<StorageLookup, Parameter>>;

      // Access the stored parameter type
      template <typename Parameter>
      StoredParameterType<Parameter, StoredTypes, Parameters>&
      lookupInStorage() {
        using ParamIndex = meta::mp_find<Parameters, Parameter>;
        return std::get<ParamIndex::value>(storage_);
      }

      // Access the stored parameter type
      template <typename Parameter>
      const StoredParameterType<Parameter, StoredTypes, Parameters>&
      lookupInStorage() const {
        using ParamIndex = meta::mp_find<Parameters, Parameter>;
        return std::get<ParamIndex::value>(storage_);
      }

      // The actual tuple-type to store the parameter values
      using Storage = meta::mp_rename<StoredTypes, std::tuple>;
      Storage storage_;
    };
  }  // namespace detail

}  // namespace adm
