/* ============================================================================
 * I B E X - Interval definition
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Bug fixes   : Gilles Trombettoni, Bertrand Neveu
 * Created     : Dec 05, 2011
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_INTERVAL_H_
#define _IBEX_INTERVAL_H_

/* ========================================================*/
/* The following header file is automatically generated by
 * the compilation. It only contains the definition of
 * _IBEX_WITH_GAOL_ or _IBEX_WITH_BIAS_ */
#include "ibex_bias_or_gaol.h"
/* ======================================================= */

#ifdef _IBEX_WITH_GAOL_
extern "C" {
#include "gdtoaimp.h"
}
#include "gaol/gaol.h"
#include "gaol/gaol_interval.h"
/** \brief NEG_INFINITY: double representation of -oo */
#define NEG_INFINITY (-GAOL_INFINITY)
/** \brief POS_INFINITY: double representation of +oo */
#define POS_INFINITY GAOL_INFINITY
#else
#ifdef _IBEX_WITH_BIAS_
#include "Interval.h"
/** \brief NEG_INFINITY: double representation of -oo */
#define NEG_INFINITY BiasNegInf
/** \brief POS_INFINITY: double representation of +oo */
#define POS_INFINITY BiasPosInf
#endif
#endif

namespace ibex {

/** \defgroup arithmetic Interval Arithmetic */

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Sets the rounding direction mode of the FPU towards -oo.
 */
void fpu_round_down();

/**
 * \brief Sets the rounding direction mode of the FPU towards +oo.
 */
void fpu_round_up();

/**
 * \brief Sets the rounding direction mode of the nearest
 */
void fpu_round_near();

/*
 * Sets the rounding direction mode of the FPU towards zero.
 */
void fpu_round_zero();

/**
 * \brief Return the previous float
 */
double previous_float(double x);

/**
 * \brief Return the next float
 */
double next_float(double x);


/*@}*/

/**
 * \ingroup arithmetic
 *
 * \brief Interval
 *
 * This class defines the interval interface of IBEX and encapsulates an interval "itv" whose
 * type depends on the chosen implementation (currently: Gaol or Bias).
 *
 * Note that some functions of the Gaol interval interface do not appear here (like "possibly relations")
 * because there are not used by ibex; while other have been introduced (like "ratio_delta"). Some
 * functions are also renamed to match more conventional use.
 *
 */
class Interval {
  public:
	/** \brief Create (-oo,+oo). */
    Interval();

    /** \brief Create [a,b].  */
    Interval(double a, double b);

    /** \brief Create [a,a]. */
    Interval(double a);

    /** \brief True iff *this and x are exactly the same intervals. */
    bool operator==(const Interval& x) const;

    /** \brief True iff *this and x are not exactly the same intervals. */
    bool operator!=(const Interval& x) const;

    /** \brief Set this interval to the empty set. */
    void set_empty();

    /** \brief Set *this to x.
     */
    Interval& operator=(const Interval& x);

    /** \brief Set *this to d.
     */
    Interval& operator=(double x);

    /** \brief Intersection of *this and x.
     * \param x - the interval to compute the intersection with.*/
    Interval& operator&=(const Interval& x);

    /** \brief Union of *this and I.
     * \param x - the interval to compute the hull with.*/
    Interval& operator|=(const Interval& x);

    /**
     * \brief Add [-rad,+rad] to *this.
     *
     * Return a reference to *this.
     */
    Interval& inflate(double rad);

    /** \brief Lower bound.
     *
     * Return the lower bound of *this. */
    double lb() const;

    /** \brief Upper bound.
     *
     * Return the upper bound of *this. */
    double ub() const;

    /** \brief Midpoint.
     *
     * Returns the midpoint of *this.
     * The return point is guaranteed to be included in *this
     * but not necessarily to be the closest floating point
     * from the real midpoint.
     *
     * Cases are:
     * - \emptyset  -> Quiet NaN
     * - [-oo, +oo] -> midP = 0.0
     * - [-oo, b]   -> midP = -MAXREAL
     * - [a, +oo]   -> midP = MAXREAL
     * - [a, b]     -> midP ~ a + .5*(b-a) */
    double mid() const;

    /**
     * \brief Radius.
     *
     * Return the diameter of *this.
     * By convention, 0 if *this is empty.*/
    double rad() const;

    /**
     * \brief Diameter.
     *
     * Return the diameter of *this.
     * By convention, 0 if *this is empty.*/
    double diam() const;

    /**
     * \brief Mignitude.
	 *
     * Returns the mignitude of *this:
     * <lu>
     * <li> +(lower bound)  if *this > 0
     * <li> -(upper bound) if *this < 0
     * <li> 0 otherwise.
     * </lu> */
    double mig() const;

    /**
     * \brief Magnitude.
	 *
     * Returns the magnitude of *this:
     * mag(*this)=max(|lower bound|, |upper bound|). */
    double mag() const;

    /**
     * \brief True iff this interval is a subset of \a x.
     *
     * \note Always return true if *this is empty. */
    bool is_subset(const Interval& x) const;

    /**
     * \brief True iff this interval is in the interior of \a x.
     *
     * \note In particular, (-oo,oo) is a strict subset of (-oo,oo)
     * and the empty set is a strict subset of the empty set.
     * \note Always return true if *this is empty. */
    bool is_strict_subset(const Interval& x) const;

    /**
     * \brief True iff this interval is a superset of \a x.
     *
     * \note Always return true if x is empty. */
    bool is_superset(const Interval& x) const;

    /**
     * \brief True iff the interior of *this is a superset of \a x.
     *
     * \note In particular, (-oo,oo) is a strict superset of (-oo,oo)
     */
    bool is_strict_superset(const Interval& x) const;

    /**
     * \brief True iff *this contains \a d.
     *
     * \note d can also be an "open bound", i.e., infinity.
     * So this function is not restricted to a set-membership
     * interpretation. */
    bool contains(double d) const;

    /**
     * \brief True iff the interior of *this contains \a d.
     *
     * \note d can also be an "open bound", i.e., infinity.
     * So this function is not restricted to a set-membership
     * interpretation. */
    bool strictly_contains(double d) const;

    /**
     * \brief True iff *this and \a x do not intersect.
     *
     */
    bool is_disjoint(const Interval &x) const;

    /**
     * \brief True iff *this is empty.
     */
    bool is_empty() const;

    /**
     * \brief True iff *this is degenerated.
     *
     * An interval is degenerated if it is of the form [a, a]
     *
     * \note An empty interval is considered here as degenerated. */
    bool is_degenerated() const;

    /**
     * \brief True if one bound of *this is infinite.
     *
     * \note An empty interval is always bounded.
     */
    bool is_unbounded() const;

    /**
     * \brief Relative Hausdorff distance between *this and x.
     *
     * The relative distance is basically distance(x)/diam(*this).
     */
    double rel_distance(const Interval& x) const;

    /** \brief Return -*this. */
    Interval operator-() const;

    /** \brief Add \a d to *this and return the result.  */
    Interval& operator+=(double d);

    /** \brief Subtract \a d to *this and return the result. */
    Interval& operator-=(double d);

    /** \brief Multiply *this by \a d and return the result. */
    Interval& operator*=(double d);

    /** \brief Divide *this by \a d and return the result. */
    Interval& operator/=(double d);

    /** \brief Add \a x to *this and return the result. */
    Interval& operator+=(const Interval& x);

    /** \brief Subtract \a x to *this and return the result. */
    Interval& operator-=(const Interval& x);

    /** \brief Multiply *this by \a x and return the result. */
    Interval& operator*=(const Interval& x);

    /**
     * \brief Divide *this by \a x and return the result.
     *
     * Does better than *this=*this/x: because calculates
     * the union of *this/x as intermediate result. */
    Interval& operator/=(const Interval& x);

    /**
     * \brief Set this interval to the intersection of itself with the division of two others.
     *
     * \param x - the numerator
     * \param y - the divisor
     * \param out2 - In return, *this and out2 contains the lower and upper part respectively
     * of the division. If the result of the generalized division and intersection
     * is a single interval, out2 is set to the empty interval.
     * \code
     * Interval intv(-10,10);
     * Interval out2;
     * intv.div2_inter(Interval(2,3), Interval(-1,2), out2);
     * cout << intv << " " << out2 << endl;  // will display: [-10,-2] [1,10]
     * \endcode
	 *
     * \return \c true if the intersection is non empty.
     * \note Contrary to the "cset" theory, the result is empty if \a y=[0,0] (whatever \a x is).
     */
    bool div2_inter(const Interval& x, const Interval& y, Interval& out2);

    /**
     * \brief Set this interval to the intersection of itself with the division of two others.
     */
    Interval& div2_inter(const Interval& x, const Interval& y);

    /**
     * \brief Return diam(*this)-diam(x), for x\subseteq *this [deprecated]
     *
     * Deprecated. Kept for compatibility with ibex 1.xx.
     *
     * \pre \a x must be included in this interval.
     * \note The result may be +oo (if the set difference is infinite).
     * \note An empty interval is considered here to have a null diamater (as a degenerated interval). <br>
     * If either \a x or this interval is empty, then the method returns the diameter of this interval
     * (which is 0 if the latter is empty).
     */
    double delta(const Interval& x) const;

    /**
     * \brief Compute the ratio of the diameter to #delta(x) [deprecated].
     *
     * Deprecated. Kept for compatibility with ibex 1.xx.
     *
     * \pre \a x must be included in this interval.
     * \note An empty interval is considered to have a null diamater (as a degenerated interval). <br>
     * <ul><li>If either \a x or this interval is empty, then
     * <ul><li>the method returns 1 (100% of reduction) if this diameter is not null,
     *     <li>0 otherwise (as if 0/0=0).</ul>
     * <li>As a pure convention, the method returns \c 1 if one bound of this interval is infinite and the corresponding bound of \a x
     * is not (in particular if this interval is unbounded and \a x not). </ul>
     */
    double ratiodelta(const Interval& x) const;

    /** \brief pi. */
    static const Interval PI;
    /** \brief 2*pi. */
    static const Interval TWO_PI;
    /** \brief pi/2. */
    static const Interval HALF_PI;
    /** \brief the empty interval. */
    static const Interval EMPTY_SET;
    /** \brief (-oo,oo). */
    static const Interval ALL_REALS;
    /** \brief [0,0]. */
    static const Interval ZERO;
    /** \brief [1,1]. */
    static const Interval ONE;
    /** \brief [0,+oo). */
    static const Interval POS_REALS;
    /** \brief (-oo,0]. */
    static const Interval NEG_REALS;

//    friend class IntervalVector;

//private:
#ifdef _IBEX_WITH_GAOL_
	/* \brief Wrap the gaol-interval [x]. */
    Interval(const gaol::interval& x);
    /* \brief Assign this to the gaol-interval [x]. */
    Interval& operator=(const gaol::interval& x);
	gaol::interval itv;
#else
#ifdef _IBEX_WITH_BIAS_
	/* \brief Wrap the bias-interval [x]. */
    Interval(const INTERVAL& x);
    /* \brief Assign this to the bias-interval [x]. */
    Interval& operator=(const INTERVAL& x);

    INTERVAL itv;
#endif
#endif
};

/** \ingroup arithmetic */
/*@{*/

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const Interval& x);

/** \brief $[x]_1\cap [x]_2$.
 * \return Interval::EMPTY if the intersection is empty. */
Interval operator&(const Interval& x1, const Interval& x2);

/** \brief $\square([x]_1\cup [x]_2)$. */
Interval operator|(const Interval& x1, const Interval& x2);

/** \brief $[x]+d$. */
Interval operator+(const Interval& x, double d);

/** \brief $[x]-d$. */
Interval operator-(const Interval& x, double d);

/** \brief $[x]*d$. */
Interval operator*(const Interval& x, double d);

/** \brief $[x]/d$. */
Interval operator/(const Interval& x, double d);

/** \brief Generalized division
 *
 * The result of x/y is stored in out1 and out2. These intervals contain the lower and upper part respectively
 * of the division. If the result of the generalized division is a single interval, out2
 * is set to the empty interval. If the result is the empty set, both out1 and out2 are set to the
 * empty interval.
 *
 * \note Contrary to the "cset" theory, the result is empty if \a y=[0,0] (whatever \a x is).
 */
void div2(const Interval& x, const Interval& y, Interval& out1, Interval& out2);

/** \brief $d+[x]$. */
Interval operator+(double d,const Interval& x);

/** \brief $d-[x]$. */
Interval operator-(double d, const Interval& x);

/** \brief $d*[x]$. */
Interval operator*(double d, const Interval& x);

/** \brief $d/[x]$. */
Interval operator/(double d, const Interval& x);

/** \brief $[x]_1+[x]_2$. */
Interval operator+(const Interval& x1, const Interval& x2);

/** \brief $[x]_1-[x]_2$. */
Interval operator-(const Interval& x1, const Interval& x2);

/** \brief $[x]_1*[x]_2$. */
Interval operator*(const Interval& x1, const Interval& x2);

/** \brief $[x]_1/[x]_2$. */
Interval operator/(const Interval& x1, const Interval& x2);

/** \brief Hausdorff distance of $[x]_1$ and $[x]_2$. */
double distance(const Interval &x1, const Interval &x2);

/** \brief $[x]^2$ */
Interval sqr(const Interval& x);

/** \brief $\sqrt{[x]}$ */
Interval sqrt(const Interval& x);

/** \brief $[x]^n$. */
Interval pow(const Interval& x, int n);

/** \brief $[x]^d$. */
Interval pow(const Interval& x, double d);

/** \brief $[x]^[y]$. */
Interval pow(const Interval &x, const Interval &y);

/** \brief $n^{th}$ root of $[x]$. */
Interval root(const Interval& x, int n);

/** \brief $\exp([x])$. */
Interval exp(const Interval& x);

/** \brief $\log([x])$. */
Interval log(const Interval& x);

/** \brief $\cos([x])$. */
Interval cos(const Interval& x);

/** \brief $\sin([x])$. */
Interval sin(const Interval& x);

/** \brief $\tan([x])$. */
Interval tan(const Interval& x);

/** \brief $\acos([x])$. */
Interval acos(const Interval& x);

/** \brief $\asin([x])$. */
Interval asin(const Interval& x);

/** \brief $\atan([x])$. */
Interval atan(const Interval& x);

/** \brief $\atan2([x],[y])$. */
Interval atan2(const Interval& x, const Interval& y);

/** \brief $\cosh([x])$. */
Interval cosh(const Interval& x);

/** \brief $\sinh([x])$. */
Interval sinh(const Interval& x);

/** \brief $\tanh([x])$. */
Interval tanh(const Interval& x);

/** \brief $\acosh([x])$. */
Interval acosh(const Interval& x);

/** \brief $\asinh([x])$. */
Interval asinh(const Interval& x);

/** \brief $\atanh([x])$. */
Interval atanh(const Interval& x);

/** \brief $\abs([x]) = \{|x|, x\in[x]\}.$. */
Interval abs(const Interval &x);

/** \brief Maximum of two intervals.
 *
 * Return \f$\max([x],[y]) = \{ \max\{x,y\}, x\in[x], y\in[y] \}\f$. */
Interval max(const Interval& x, const Interval& y);

/** \brief Minimum of two intervals.
 *
 *  Return \f$\min([x],[y]) = \{ \min\{x,y\}, x\in[x], y\in[y] \}\f$. */
Interval min(const Interval& x, const Interval& y);

/** \brief Sign of $[x]$.
 *
 *  Return \f$sign([x]) = hull \{ sign{x}, x\in[x] \}\f$.
 * \remark By convention, \f$ 0\in[x] \Longrightarrow sign[x]=[-1,1]\f$. */
Interval sign(const Interval& x);

/**
 * \brief Return the largest integer interval included in x.
 */
Interval integer(const Interval& x);

/** \brief Projection of $y=x_1+x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1+x_2\}$. */
bool proj_add(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of $y=x_1-x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1-x_2\}$. */
bool proj_sub(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of $y=x_1*x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1\times x_2\}$. */
bool proj_mul(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of $y=x_1/x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1/x_2\}$. */
bool proj_div(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of $y=x^2$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=x^2 \}$. */
bool proj_sqr(const Interval& y, Interval& x);

/** \brief Projection of $y=\sqrt{x}$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\sqrt{x} \}$. */
bool proj_sqrt(const Interval& y, Interval& x);

/** \brief Projection of $y=x^n$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=x^n \}$. */
bool proj_pow(const Interval& y, int n, Interval& x);

/** \brief Projection of $y=x_1^{x_2}$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1^{x_2}\}$. */
bool proj_pow(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of the $y=x^{\frac{1}{n}}$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=x^{\frac{1}{n}} \}$. */
bool proj_root(const Interval& y, int n, Interval& x);

/** \brief Projection of $y=\exp(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\exp(x) \}$. */
bool proj_exp(const Interval& y,  Interval& x);

/** \brief Projection of $y=\log(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\log(x) \}$. */
bool proj_log(const Interval& y,  Interval& x);

/** \brief Projection of $y=\cos(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\cos(x) \}$. */
bool proj_cos(const Interval& y,  Interval& x);

/** \brief Projection of $y=\sin(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\sin(x) \}$. */
bool proj_sin(const Interval& y,  Interval& x);

/** \brief Projection of $y=\tan(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\tan(x) \}$. */
bool proj_tan(const Interval& y,  Interval& x);

/** \brief Projection of $y=\acos(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\acos(x) \}$. */
bool proj_acos(const Interval& y,  Interval& x);

/** \brief Projection of $y=\asin(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\asin(x) \}$. */
bool proj_asin(const Interval& y,  Interval& x);

/** \brief Projection of $y=\atan(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\atan(x) \}$. */
bool proj_atan(const Interval& y,  Interval& x);

/** \brief Projection of $y=atan2(x_1,x_2)$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=atan2(x_1,x_2)$. */
bool proj_atan2(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of $y=\cosh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\cosh(x) \}$. */
bool proj_cosh(const Interval& y,  Interval& x);

/** \brief Projection of $y=\sinh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\sinh(x) \}$. */
bool proj_sinh(const Interval& y,  Interval& x);

/** \brief Projection of $y=\tanh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\tanh(x) \}$. */
bool proj_tanh(const Interval& y,  Interval& x);

/** \brief Projection of $y=\acosh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\acosh(x) \}$. */
bool proj_acosh(const Interval& y,  Interval& x);

/** \brief Projection of $y=\asinh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\asinh(x) \}$. */
bool proj_asinh(const Interval& y,  Interval& x);

/** \brief Projection of $y=\atanh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\atanh(x) \}$. */
bool proj_atanh(const Interval& y,  Interval& x);

/** \brief Projection of $y=|x|$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=|x| \}$. */
bool proj_abs(const Interval& y,  Interval& x);

/** \brief Projection of $y=\max(x_1,x_2)$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=\max(x_1,x_2)\}$. */
bool proj_max(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of $y=\min(x_1,x_2)$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=\min(x_1,x_2)\}$. */
bool proj_min(const Interval& y, Interval& x1, Interval& x2);

/** \brief Projection of $y=sign(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=sign(x) \}$. */
bool proj_sign(const Interval& y, Interval& x);

/** \brief Contract x w.r.t. the fact that it must be integral.
 *
 */
bool proj_integer(Interval& x);

} // end namespace ibex

#ifdef _IBEX_WITH_GAOL_
#include "ibex_gaol_Interval.h_"
#else
#ifdef _IBEX_WITH_BIAS_
#include "ibex_bias_Interval.h_"
#endif
#endif

/*@}*/

/*============================================ features with common implementation ============================================ */

namespace ibex {

inline Interval::Interval() : itv(NEG_INFINITY, POS_INFINITY) {

}

inline Interval::Interval(double a, double b) : itv(a,b) {
	if (a==POS_INFINITY || b==NEG_INFINITY || a>b) *this=EMPTY_SET;
}

inline Interval::Interval(double a) : itv(a,a) {
	if (a==NEG_INFINITY || a==POS_INFINITY) *this=EMPTY_SET;
}

inline bool Interval::operator==(const Interval& x) const {
	return (is_empty() && x.is_empty()) || (lb()==x.lb() && ub()==x.ub());
}

inline Interval& Interval::operator=(double x) {
	if (x==NEG_INFINITY || x==POS_INFINITY)
		*this=EMPTY_SET;
	else
		itv = x;
	return *this;
}

inline Interval& Interval::operator=(const Interval& x) {
	itv = x.itv;
	return *this;
}

inline Interval& Interval::inflate(double rad) {
	(*this) += Interval(-rad,rad);
	return *this;
}

inline bool Interval::operator!=(const Interval& x) const {
	return !(*this==x);
}

inline double Interval::rad() const {
	return 0.5*diam();
}

inline bool Interval::is_strict_subset(const Interval& x) const {
	return is_empty() ||
			(!x.is_empty() && (x.lb()==NEG_INFINITY || x.lb()<lb()) &&
					(x.ub()==POS_INFINITY || x.ub()>ub()));
}

inline bool Interval::is_strict_superset(const Interval& x) const {
	return x.is_strict_subset(*this);
}

inline double Interval::rel_distance(const Interval& x) const {
	  double d=distance(*this,x);
	  if (d==POS_INFINITY) return 1;
	  double D=diam();
	  return (D==0 || D==POS_INFINITY) ? 0.0 : (d/D); // if diam(this)=infinity here, necessarily d=0
}

inline double distance(const Interval &x1, const Interval &x2) {

    if (x1.is_empty()) return x2.rad();

    if (x2.is_empty()) return x1.rad();

    if (x1.lb()==NEG_INFINITY) {
    	if (x2.lb()!=NEG_INFINITY)
    		return POS_INFINITY;
    	else if (x1.ub()==POS_INFINITY) {
    		if (x2.ub()==POS_INFINITY) return 0.0;
    		else return POS_INFINITY;
    	}
    	else if (x2.ub()==POS_INFINITY) return POS_INFINITY;
    	else return fabs(x1.ub()-x2.ub());
    }
    else if (x1.ub()==POS_INFINITY) {
    	if (x2.ub()!=POS_INFINITY)
    		return POS_INFINITY;
    	else if (x2.lb()==NEG_INFINITY)
    		return POS_INFINITY;
    	else return fabs(x1.lb()-x2.lb());
    }
    else if (x2.is_unbounded())
    	return POS_INFINITY;
    else
#ifdef _IBEX_WITH_GAOL_
    	return hausdorff(x1,x2);
#else
#ifdef _IBEX_WITH_BIAS_
    	return Distance(x1.itv,x2.itv);
#endif
#endif

}

inline Interval sign(const Interval& x) {
	return x.ub()<0 ? Interval(-1,-1) : x.lb()>0 ? Interval(1,1) : Interval(-1,1);
}

inline bool proj_add(const Interval& y, Interval& x1, Interval& x2) {
	if ((x1 &= y-x2).is_empty()) { x2.set_empty(); return false; }
	if ((x2 &= y-x1).is_empty()) { x1.set_empty(); return false; }
	return true;
}

inline bool proj_sub(const Interval& y, Interval& x1, Interval& x2) {
	if ((x1 &= y+x2).is_empty()) { x2.set_empty(); return false; }
	if ((x2 &= x1-y).is_empty()) { x1.set_empty(); return false; }
	return true;
}

inline bool proj_div(const Interval& y, Interval& x1, Interval& x2) {
	if ((x1 &= y*x2).is_empty()) { x2.set_empty(); return false; }
	Interval tmp=y;
	proj_mul(x1, tmp, x2);
	if (x2.is_empty()) { x1.set_empty(); return false; }
	return true;
}

inline bool proj_sqrt(const Interval& y, Interval& x) {
	x &= sqr(y);
	return !x.is_empty();
}

inline bool proj_root(const Interval& y, int n, Interval& x) {
	x &= pow(y,n);
	return !x.is_empty();
}

inline bool proj_exp(const Interval& y,  Interval& x) {
	x &= log(y);
	return !x.is_empty();
}

inline bool proj_log(const Interval& y,  Interval& x) {
	x &= exp(y);
	return !x.is_empty();
}

inline bool proj_acos(const Interval& y,  Interval& x) {
	x &= cos(y);
	return !x.is_empty();
}

inline bool proj_asin(const Interval& y,  Interval& x) {
	x &= sin(y);
	return !x.is_empty();
}

inline bool proj_atan(const Interval& y,  Interval& x) {
	x &= tan(y);
	return !x.is_empty();
}

inline bool proj_acosh(const Interval& y,  Interval& x) {
	if (y.ub()<0.0) {
		x.set_empty(); return false;
	}
	else {
		x &= cosh(Interval(y.lb()<0?0:y.lb(),y.ub()));
		return !x.is_empty();
	}
}

inline bool proj_asinh(const Interval& y,  Interval& x) {
	x &= sinh(y);
	return !x.is_empty();
}

inline bool proj_atanh(const Interval& y,  Interval& x) {
	x &= tanh(y);
	return !x.is_empty();
}

inline bool proj_max(const Interval& y, Interval& x1, Interval& x2) {

	/* ---- Disjoint intervals ---- */
	if (x2.lb()>x1.ub() || y.lb()>x1.ub()) {
		/* then, max(x,x2) is necessarily x2 */
		x2 &= y; return !x2.is_empty();
	} else if (x1.lb()>x2.ub() || y.lb()>x2.ub()) {
		x1 &= y; return !x1.is_empty();
	}
	/*------------------------------*/

	if (y.ub()<x1.lb() || y.ub()<x2.lb()) {
		return false; // inconsistency
	}

	/* At this point, x, y and y all mutually intersect. */
	if (x1.ub()>y.ub())
		x1=Interval(x1.lb(),y.ub());
	if (x2.ub()>y.ub())
		x2=Interval(x2.lb(),y.ub());

	return true;
}

inline bool proj_min(const Interval& y, Interval& x1, Interval& x2) {

	Interval mx1=-x1;
	Interval mx2=-x2;

	if (!proj_max(-y,mx1,mx2)) return false;

	x1=-mx1;
	x2=-mx2;
	return true;
}

inline bool proj_sign(const Interval& y,  Interval& x) {

	if (y.contains(0)) return true;

	if(y.lb()<0) {
		x &= Interval::NEG_REALS;
	}

	/* At this point, y.ub()>0. */
	x &= Interval::POS_REALS;
	return !x.is_empty();
}

inline bool proj_integer(Interval& x) {
	return !(x = integer(x)).is_empty();
}

} // end namespace ibex

#endif // _IBEX_INTERVAL_H_
