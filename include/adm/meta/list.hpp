#pragma once

#include <boost/version.hpp>

/**
 * @file irt/adm/meta/list.hpp
 *
 * This header is a small wrapper for boost::mp11 metaprogramming library,
 * which is only available in relatively new boost version (1.66).
 *
 * It uses the boost::mp11 implementation if available.
 * If boost is older than 1.66, we use a poor man's drop in replacement.
 * This only provide a very small subset of mp11, but that's all we need for
 * this library.
 *
 * Because we only need a few trivial things, it's easy to implement our own
 * version based on http://www.pdimov.com/cpp2/simple_cxx11_metaprogramming.html
 * But because the author turned his ideas into a boost library, we
 * obviously use that if it's available.
 */

#if BOOST_VERSION < 106600
#include <adm/meta/backport/list.hpp>
#else

#include <boost/mp11/list.hpp>

namespace adm {
  namespace meta {
    using namespace boost::mp11;

  }  // namespace meta
}  // namespace adm

#endif
