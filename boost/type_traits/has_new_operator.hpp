
//  (C) Copyright Runar Undheim, Robert Ramey & John Maddock 2008.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_NEW_OPERATOR_HPP_INCLUDED
#define BOOST_TT_HAS_NEW_OPERATOR_HPP_INCLUDED

#include <boost/type_traits/config.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/type_traits/detail/ice_or.hpp>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost {
namespace detail {
    template <class U, U x> 
    struct test;

    template <typename T>
    struct has_new_operator_impl {
        template<class U>
        static type_traits::yes_type check_sig(
            U*, 
            test<
            void *(*)(std::size_t),
                &U::operator new
            >* = NULL
        );
        template<class U>
        static type_traits::yes_type check_sig(
            U*, 
            test<
            void *(*)(std::size_t, const std::nothrow_t&),
                &U::operator new
            >* = NULL
        );
        template<class U>
        static type_traits::yes_type check_sig(
            U*, 
            test<
            void *(*)(std::size_t, void*),
                &U::operator new
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig(...);

        template<class U>
        static type_traits::yes_type check_sig2(
            U*, 
            test<
            void *(*)(std::size_t),
                &U::operator new[]
            >* = NULL
        );
        template<class U>
        static type_traits::yes_type check_sig2(
            U*, 
            test<
            void *(*)(std::size_t, const std::nothrow_t&),
                &U::operator new[]
            >* = NULL
        );
        template<class U>
        static type_traits::yes_type check_sig2(
            U*, 
            test<
            void *(*)(std::size_t, void*),
                &U::operator new[]
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig2(...);

        // GCC2 won't even parse this template if we embed the computation
        // of s1 in the computation of value.
        #ifdef __GNUC__
            BOOST_STATIC_CONSTANT(unsigned, s1 = sizeof(has_new_operator_impl<T>::template check_sig<T>(0)));
            BOOST_STATIC_CONSTANT(unsigned, s2 = sizeof(has_new_operator_impl<T>::template check_sig2<T>(0)));
        #else
            #if BOOST_WORKAROUND(_MSC_FULL_VER, >= 140050000)
                #pragma warning(push)
                #pragma warning(disable:6334)
            #endif

            BOOST_STATIC_CONSTANT(unsigned, s1 = sizeof(check_sig<T>(0)));
            BOOST_STATIC_CONSTANT(unsigned, s2 = sizeof(check_sig2<T>(0)));

            #if BOOST_WORKAROUND(_MSC_FULL_VER, >= 140050000)
                #pragma warning(pop)
            #endif
        #endif
        BOOST_STATIC_CONSTANT(bool, value = 
           (::boost::type_traits::ice_or<
            (s1 == sizeof(type_traits::yes_type)),
            (s2 == sizeof(type_traits::yes_type))
           >::value)
        );
    };
} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(has_new_operator,T,::boost::detail::has_new_operator_impl<T>::value)

} // namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>

#endif // BOOST_TT_HAS_NEW_OPERATOR_HPP_INCLUDED
