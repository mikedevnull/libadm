#pragma once

namespace adm {
  namespace detail {

    /**
     * Helper template to implement the Curiously recurring template pattern
     * https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
     */
    template <typename T, template <typename> class SelfType>
    struct Crtp {
      T& underlying() { return static_cast<T&>(*this); }
      const T& underlying() const { return static_cast<const T&>(*this); }
    };

  }  // namespace detail

}  // namespace adm
