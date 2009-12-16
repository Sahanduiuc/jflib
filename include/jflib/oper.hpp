#ifndef		__OPER_TEMPLATE_HPP__
#define		__OPER_TEMPLATE_HPP__

#include <boost/type_traits.hpp>
#include <complex>


namespace jflib {

#define QM_PROMOTE(A,B) promote_trait<A,B>::promote

template<typename T>
struct precision_trait {
    static const int precisionRank = 0;
    static const bool knowPrecisionRank = false;
};

#define QM_DECLARE_PRECISION(T,rank)                  \
    template<>                                        \
    struct precision_trait< T > {                     \
        static const int precisionRank = rank;        \
        static const bool knowPrecisionRank = true;   \
    };


QM_DECLARE_PRECISION(int,100)
QM_DECLARE_PRECISION(unsigned int,200)
QM_DECLARE_PRECISION(long,300)
QM_DECLARE_PRECISION(unsigned long,400)
QM_DECLARE_PRECISION(float,500)
QM_DECLARE_PRECISION(double,600)
QM_DECLARE_PRECISION(long double,700)
QM_DECLARE_PRECISION(std::complex<float>,800)


template<typename T>
struct autopromote_trait {
    typedef T numtype;
};

#define QM_DECLARE_AUTOPROMOTE(T1,T2)			\
    template<> struct autopromote_trait<T1> {	\
      typedef T2 numtype;						\
    };

// These are the odd cases where small integer types
// are automatically promoted to int or unsigned int for
// arithmetic.
QM_DECLARE_AUTOPROMOTE(bool, int)
QM_DECLARE_AUTOPROMOTE(char, int)
QM_DECLARE_AUTOPROMOTE(unsigned char, int)
QM_DECLARE_AUTOPROMOTE(short int, int)
QM_DECLARE_AUTOPROMOTE(short unsigned int, unsigned int)

template<typename T1, typename T2, bool promote_to_T1>
struct _promote2 {
    typedef T1 promote;
};

template<typename T1, typename T2>
struct _promote2<T1,T2,false> {
    typedef T2 promote;
};


template<typename T1_orig, typename T2_orig>
struct promote_trait {
    // Handle promotion of small integers to int/unsigned int
    typedef typename autopromote_trait<T1_orig>::numtype T1;
    typedef typename autopromote_trait<T2_orig>::numtype T2;

    // True if T1 is higher ranked
    static const bool T1_is_better =  precision_trait<T1>::precisionRank > precision_trait<T2>::precisionRank;

    // True if we know ranks for both T1 and T2
    static const bool know_both_ranks = precision_trait<T1>::knowPrecisionRank && precision_trait<T2>::knowPrecisionRank;

    // True if we know T1 but not T2
    static const bool know_T1_but_not_T2 = precision_trait<T1>::knowPrecisionRank && !precision_trait<T2>::knowPrecisionRank;

    // True if we know T2 but not T1
    static const bool know_T2_but_not_T1 = precision_trait<T2>::knowPrecisionRank && !precision_trait<T1>::knowPrecisionRank;

    // True if T1 is bigger than T2
    static const bool T1_is_larger = sizeof(T1) >= sizeof(T2);

    static const bool
        promote_to_T1 = know_both_ranks ? T1_is_better : (know_T1_but_not_T2 ? false : (know_T2_but_not_T1 ? true : T1_is_larger));

    typedef typename _promote2<T1,T2,promote_to_T1>::promote promote;
};


#define QM_DEFINE_BINARY_OP(name,op)	        	            			\
template<typename numtype1, typename numtype2> 	    	        			\
struct name {                                           	    			\
																			\
	typedef typename QM_PROMOTE(numtype1, numtype2) numtype;		       	\
																			\
    static numtype			                                  				\
    apply(numtype1 a, numtype2 b) { return a op b; }            			\
};


QM_DEFINE_BINARY_OP(Add,+)
QM_DEFINE_BINARY_OP(Subtract,-)
QM_DEFINE_BINARY_OP(Multiply,*)
QM_DEFINE_BINARY_OP(Divide,/)
QM_DEFINE_BINARY_OP(Modulo,%)


template<class, bool>
struct Sqrt_impl;

template<typename T>
struct Sqrt: public Sqrt_impl<T,boost::is_floating_point<T>::value>{};

template<class T>
struct Sqrt_impl<T,true> {
	static T apply(T a) { return std::sqrt(a); }
};



template<typename numtype1, typename numtype2>
struct LogRatio;

template<typename numtype1, typename numtype2>
struct LogRatio {
	typedef typename QM_PROMOTE(numtype1, numtype2) numtype;
    static numtype apply(numtype1 a, numtype2 b) { return std::log(a/b); }
};


}

#endif	//	__OPER_TEMPLATE_HPP__
