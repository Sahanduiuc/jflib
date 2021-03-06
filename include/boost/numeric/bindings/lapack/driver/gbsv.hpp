//
// Copyright (c) 2003--2009
// Toon Knapen, Karl Meerbergen, Kresimir Fresl,
// Thomas Klimpel and Rutger ter Borg
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// THIS FILE IS AUTOMATICALLY GENERATED
// PLEASE DO NOT EDIT!
//

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_DRIVER_GBSV_HPP
#define BOOST_NUMERIC_BINDINGS_LAPACK_DRIVER_GBSV_HPP

#include <boost/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/numeric/bindings/lapack/detail/lapack.h>
#include <boost/numeric/bindings/traits/detail/array.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
namespace numeric {
namespace bindings {
namespace lapack {

//$DESCRIPTION

// overloaded functions to call lapack
namespace detail {
    inline void gbsv( integer_t const n, integer_t const kl,
            integer_t const ku, integer_t const nrhs, float* ab,
            integer_t const ldab, integer_t* ipiv, float* b,
            integer_t const ldb, integer_t& info ) {
        LAPACK_SGBSV( &n, &kl, &ku, &nrhs, ab, &ldab, ipiv, b, &ldb, &info );
    }
    inline void gbsv( integer_t const n, integer_t const kl,
            integer_t const ku, integer_t const nrhs, double* ab,
            integer_t const ldab, integer_t* ipiv, double* b,
            integer_t const ldb, integer_t& info ) {
        LAPACK_DGBSV( &n, &kl, &ku, &nrhs, ab, &ldab, ipiv, b, &ldb, &info );
    }
    inline void gbsv( integer_t const n, integer_t const kl,
            integer_t const ku, integer_t const nrhs, traits::complex_f* ab,
            integer_t const ldab, integer_t* ipiv, traits::complex_f* b,
            integer_t const ldb, integer_t& info ) {
        LAPACK_CGBSV( &n, &kl, &ku, &nrhs, traits::complex_ptr(ab), &ldab,
                ipiv, traits::complex_ptr(b), &ldb, &info );
    }
    inline void gbsv( integer_t const n, integer_t const kl,
            integer_t const ku, integer_t const nrhs, traits::complex_d* ab,
            integer_t const ldab, integer_t* ipiv, traits::complex_d* b,
            integer_t const ldb, integer_t& info ) {
        LAPACK_ZGBSV( &n, &kl, &ku, &nrhs, traits::complex_ptr(ab), &ldab,
                ipiv, traits::complex_ptr(b), &ldb, &info );
    }
}

// value-type based template
template< typename ValueType >
struct gbsv_impl {

    typedef ValueType value_type;
    typedef typename traits::type_traits<ValueType>::real_type real_type;

    // high-level solve typedefs and functions
    typedef boost::mpl::bool_<true> has_pivot;

    template< typename MatrixA, typename MatrixB, typename VectorP >
    static void solve( MatrixA& A, MatrixB& B, VectorP& pivot,
            integer_t& info ) {
        invoke( A, pivot, B, info );
    }

    template< typename MatrixA, typename MatrixB, typename VectorP >
    static void solve( MatrixA& A, MatrixB& B, VectorP const&,
            integer_t& info ) {
        traits::detail::array<
                integer_t > pivot( traits::matrix_num_columns(A) );
        invoke( A, pivot, B, info );
    }

    // templated specialization
    template< typename MatrixAB, typename VectorIPIV, typename MatrixB >
    static void invoke( integer_t const kl, integer_t const ku, MatrixAB& ab,
            VectorIPIV& ipiv, MatrixB& b, integer_t& info ) {
        BOOST_STATIC_ASSERT( (boost::is_same< typename traits::matrix_traits<
                MatrixAB >::value_type, typename traits::matrix_traits<
                MatrixB >::value_type >::value) );
        BOOST_ASSERT( traits::matrix_num_columns(ab) >= 0 );
        BOOST_ASSERT( kl >= 0 );
        BOOST_ASSERT( ku >= 0 );
        BOOST_ASSERT( traits::matrix_num_columns(b) >= 0 );
        BOOST_ASSERT( traits::leading_dimension(ab) >= 2 );
        BOOST_ASSERT( traits::vector_size(ipiv) >=
                traits::matrix_num_columns(ab) );
        BOOST_ASSERT( traits::leading_dimension(b) >= std::max(1,
                traits::matrix_num_columns(ab)) );
        detail::gbsv( traits::matrix_num_columns(ab), kl, ku,
                traits::matrix_num_columns(b), traits::matrix_storage(ab),
                traits::leading_dimension(ab), traits::vector_storage(ipiv),
                traits::matrix_storage(b), traits::leading_dimension(b),
                info );
    }
};


// template function to call gbsv
template< typename MatrixAB, typename VectorIPIV, typename MatrixB >
inline integer_t gbsv( integer_t const kl, integer_t const ku,
        MatrixAB& ab, VectorIPIV& ipiv, MatrixB& b ) {
    typedef typename traits::matrix_traits< MatrixAB >::value_type value_type;
    integer_t info(0);
    gbsv_impl< value_type >::invoke( kl, ku, ab, ipiv, b, info );
    return info;
}

}}}} // namespace boost::numeric::bindings::lapack

#endif
