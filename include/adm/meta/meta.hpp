#pragma once

/**
 * @file irt/adm/meta/meta.hpp
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

#include <adm/meta/list.hpp>
#include <adm/meta/algorithm.hpp>
