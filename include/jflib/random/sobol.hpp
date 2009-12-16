/********************************************************************************
 * qmlib/math/rndgen.hpp
 *
 * Copyright (C) 2007-2009 Luca Sbardella <luca.sbardella@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Suggestions:          quant.mind@gmail.com
 * Bugs:                 quant.mind@gmail.com
 *
 * For more information, please see the quantmind Home Page:
 *    http://www.quantmind.com
 *
 *******************************************************************************/


/// \file
/// \ingroup random
///
/// \brief SOBOL Low-Discrepancy sequence generator
///
///     - A Gray code counter and bitwise operations are used for very
///  	  fast sequence generation.
///
///     - The implementation relies on primitive polynomials modulo two
///       from the book "Monte Carlo Methods in Finance" by Peter Jackel.
///
///     - 21,200 primitive polynomials modulo two are provided by
///       default in qmlib. Jackel has calculated 8,129,334
///       polynomials, also available in a different file that can be
///       downloaded from http://quantlib.org. If you need that many
///       dimensions you must replace the default version of the
///       primitivepolynomials.c file with the extended one.
///
///     - The choice of initialization numbers (also know as free direction
///       integers) is crucial for the homogeneity properties of the sequence.
///       Sobol defines two homogeneity properties: Property A and Property A'.
///
///        The unit initialization numbers suggested in "Numerical
///        Recipes in C", 2nd edition, by Press, Teukolsky, Vetterling,
///        and Flannery (section 7.7) fail the test for Property A even
///        for low dimensions.
///
///     Bratley and Fox published coefficients of the free direction
///     integers up to dimension 40, crediting unpublished work of
///     Sobol' and Levitan. See Bratley, P., Fox, B.L. (1988)
///     "Algorithm 659: Implementing Sobol's quasirandom sequence
///     generator," ACM Transactions on Mathematical Software
///     14:88-100. These values satisfy Property A for d<=20 and d =
///     23, 31, 33, 34, 37; Property A' holds for d<=6.
///
///     Jackel provides in his book (section 8.3) initialization
///     numbers up to dimension 32. Coefficients for d<=8 are the same
///     as in Bradley-Fox, so Property A' holds for d<=6 but Property
///     A holds for d<=32.
///
///     The implementation of Lemieux, Cieslak, and Luttmer includes
///     coefficients of the free direction integers up to dimension
///     360.  Coefficients for d<=40 are the same as in Bradley-Fox.
///     For dimension 40<d<=360 the coefficients have
///     been calculated as optimal values based on the "resolution"
///     criterion. See "RandQMC user's guide - A package for
///     randomized quasi-Monte Carlo methods in C," by C. Lemieux,
///     M. Cieslak, and K. Luttmer, version January 13 2004, and
///     references cited there
///     (http://www.math.ucalgary.ca/~lemieux/randqmc.html).
///     The values up to d<=360 has been provided to the QuantLib team by
///     Christiane Lemieux, private communication, September 2004.
///
///     For more info on Sobol' sequences see also "Monte Carlo
///     Methods in Financial Engineering," by P. Glasserman, 2004,
///     Springer, section 5.2.3
///
///     \test
///     - the correctness of the returned values is tested by
///       reproducing known good values.
///     - the correctness of the returned values is tested by checking
///       their discrepancy against known good values.



#ifndef __SOBOL_QM_HPP__
#define __SOBOL_QM_HPP__



#include <jflib/random/rndgen.hpp>


namespace jflib { namespace math { namespace rndgen {


/// \brief The Heston Low-discrepancy Number generator
/// \ingroup random
/// \note
/// Adapted form the book "Monte Carlo Methods in Finance" by Peter Jackel.
template<class RealType>
class Halton: public lowDiscrepacySequenceGenerator<RealType> {
public:
	typedef lowDiscrepacySequenceGenerator<RealType>	super_type;
	typedef typename super_type::size_type				size_type;
   	Halton(size_type dim, size_type seed):super_type(dim,1){}

   	std::string		name()  const {return "Halton low discrepancy number generator";}
protected:
   	void nextSequence() const;
};


template<int> struct SobolName;

template<> struct SobolName<0> { static std::string name() {return "Sobol - Unit initialization";} };
template<> struct SobolName<1> { static std::string name() {return "Sobol - Regularity breacking initialization";} };
template<> struct SobolName<2> { static std::string name() {return "Sobol - Sobol-Levitan initialization";} };
template<> struct SobolName<3> { static std::string name() {return "Sobol - Sobol-Levitan-Lemieux initialization";} };



//static const qm_int  SOBOL_BITS 	= 8*sizeof(qm_uns_long);
//static const qm_real SOBOL_NFACT 	= 0.5/(1UL<<(SOBOL_BITS-1));


/// \brief The Sobol Low-discrepancy Number generator
/// \ingroup random
/// \note
/// Adapted form the book "Monte Carlo Methods in Finance" by Peter Jackel.
///
/// The template argument defined the free direction of integers.
///	- D = 0	Unit initialization ("Numerical Recipes in C", 2nd edition)
/// - D = 1 Jaeckel
/// - D = 2 SobolLevitan
/// - D = 3 SobolLevitanLemieux
template<class RealType, int D = 1>
class Sobol: public lowDiscrepacySequenceGenerator<RealType> {
public:
	typedef lowDiscrepacySequenceGenerator<RealType>	super_type;
	typedef typename super_type::size_type				size_type;
	typedef typename super_type::seed_type				seed_type;
   	typedef std::vector<seed_type>						integers;
   	typedef std::vector<integers>    					vector_integers;

   	//  pre dimensionality must be <= PPMT_MAX_DIM
   	Sobol(size_type dim, size_type seed);

   	std::string name() const {return SobolName<1>::name();}

protected:
   	void nextSequence() const;
   	RealType uniform(seed_type n) const {return n * c_normalizationFactor;}

private:
   	static const int       	c_bits 				  = 8*sizeof(seed_type);
   	static const RealType  	c_normalizationFactor = 0.5/(1UL<<(c_bits-1));
   	//
  	vector_integers			  	m_directionIntegers;
   	mutable integers			m_lastsequence;
};


#include <jflib/random/impl/sobol_impl.hpp>


}}}

#endif  //  __SOBOL_QM_HPP__



