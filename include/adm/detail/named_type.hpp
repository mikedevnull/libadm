/// @file named_type.hpp
#pragma once

#include "adm/detail/named_type_validators.hpp"
#include "adm/detail/named_type_features.hpp"
#include "adm/detail/type_traits.hpp"

namespace adm {
  /// @brief Implementation details
  namespace detail {

    /**
     * @brief Named type class
     *
     * For more background information about strong types have a look at:
     * https://www.fluentcpp.com/2016/12/05/named-constructors/
     */
    template <typename T, typename Tag,
              typename Validator = detail::DefaultValidator,
              template <typename> class... Features>
    class NamedTypeBase
        : public Features<NamedTypeBase<T, Tag, Validator, Features...>>... {
     public:
      typedef NamedTypeBase<T, Tag, Validator, Features...> SelfType;
      typedef T value_type;
      typedef Tag tag;

      NamedTypeBase() : value_() { Validator::validate(get()); }
      explicit NamedTypeBase(T const& value) : value_(value) {
        Validator::validate(get());
      }
      explicit NamedTypeBase(T&& value) : value_(value) {
        Validator::validate(get());
      }
      T& get() { return value_; }
      T const& get() const { return value_; }

     private:
      T value_;
    };

    template <typename T, typename Tag,
              typename Validator = detail::DefaultValidator>
    using NamedType = NamedTypeBase<T, Tag, Validator, nt::Comparable,
                                    nt::Incrementable, nt::Printable>;

    template <typename T, typename Tag, typename Validator,
              template <typename> class... Features>
    std::ostream& operator<<(
        std::ostream& stream,
        const NamedTypeBase<T, Tag, Validator, Features...>& rhs) {
      rhs.print(stream);
      return stream;
    }
  }  // namespace detail
}  // namespace adm
