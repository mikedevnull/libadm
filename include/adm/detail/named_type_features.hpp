#pragma once
#include <adm/detail/crtp.hpp>

#include <iosfwd>

namespace adm {
  namespace detail {
    namespace nt {
      template <typename T>
      struct Comparable : Crtp<T, Comparable> {
        bool operator==(const T& other) const {
          return this->underlying().get() == other.get();
        }

        template <typename U>
        bool operator==(const U& other) const {
          return this->underlying().get() == other;
        }

        bool operator!=(const T& other) const {
          return this->underlying().get() != other.get();
        }

        template <typename U>
        bool operator!=(const U& other) const {
          return this->underlying().get() != other;
        }

        bool operator<(const T& other) const {
          return this->underlying().get() < other.get();
        }

        template <typename U>
        bool operator<(const U& other) const {
          return this->underlying().get() < other;
        }

        bool operator>(const T& other) const {
          return this->underlying().get() > other.get();
        }

        template <typename U>
        bool operator>(const U& other) const {
          return this->underlying().get() > other;
        }

        bool operator>=(const T& other) const {
          return this->underlying().get() >= other.get();
        }
        template <typename U>
        bool operator>=(const U& other) const {
          return this->underlying().get() >= other;
        }

        bool operator<=(const T& other) const {
          return this->underlying().get() <= other.get();
        }

        template <typename U>
        bool operator<=(const U& other) const {
          return this->underlying().get() <= other;
        }
      };

      template <typename T>
      struct Incrementable : Crtp<T, Incrementable> {
        T& operator++() {
          ++(this->underlying().get());
          return this->underlying();
        }

        T operator++(int) {
          T tmp = *this;
          operator++();
          return tmp;
        }

        T& operator--() {
          --(this->underlying().get());
          return this->underlying();
        }

        T operator--(int) {
          T tmp = *this;
          operator--();
          return tmp;
        }
      };

      template <typename T>
      struct Printable : Crtp<T, Printable> {
        void print(std::ostream& stream) const {
          stream << this->underlying().get();
        }
      };

    }  // namespace nt

  }  // namespace detail

}  // namespace adm
