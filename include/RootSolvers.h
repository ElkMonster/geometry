#ifndef ROOTSOLVERS_H_
#define ROOTSOLVERS_H_

#include <complex>

namespace geom
{


struct Func
{
    virtual double operator()(double x) const = 0;
};


// x^4 + alpha*x^3 + beta*x^2 + gamma*x + delta = y
struct QuarticFunc : Func
{
    double alpha, beta, gamma, delta;

    QuarticFunc(double alpha, double beta, double gamma, double delta)
        :   alpha(alpha),
        beta(beta),
        gamma(gamma),
        delta(delta)
    {}

    double operator()(double x) const
    {
        double x2 = x * x;
        double x3 = x2 * x;
        return x * (x3 + (alpha * x2) + (beta * x) + gamma) + delta;
    }
};


struct Double2D
{
    Double2D(double x, double y) : x(x), y(y) {};

    Double2D() : x(0.), y(0.) {};

    Double2D(const Double2D& d) : x(d.x), y(d.y) {};

    double x;
    double y;
};


bool findPositive(const Func& func,
                  const double range[2],
                  double step,
                  Double2D& pos);


bool findNegative(const Func& func,
                  const double range[2],
                  double step,
                  Double2D& neg);


bool findChangeOfSign(const Func& func,
                      const double range[2],
                      double step,
                      Double2D& pos,
                      Double2D& neg);


bool bisectRoot(const Func& func,
                const double range[2],
                double step,
                double epsilon,
                double newRange[],
                Double2D& root);


/**
 *  The order of roots returned is guaranteed to be real first, complex last.
 */
void cubicPolynomialRoots(double r,
                          double s,
                          double t,
                          double u,
                          std::complex<double> roots[3],
                          int& numRealRoots,
                          int& numComplexRoots);


void cubicPolynomialRoots(double r,
                          double s,
                          double t,
                          double u,
                          double roots[3],
                          int& numRoots);


/**
 *  The order of roots returned is guaranteed to be real first, complex last.
 */
void quarticPolynomialRoots(double a,
                            double b,
                            double c,
                            double d,
                            double e,
                            std::complex<double> roots[4],
                            int& numRealRoots,
                            int& numComplexRoots);


void quarticPolynomialRoots(double a,
                            double b,
                            double c,
                            double d,
                            double e,
                            double roots[4],
                            int& numRoots);


bool newtonRoot(const Func& func,
                const Func& derivedFunc,
                double x0,
                double accuracy,
                int maxIter,
                Double2D& result);

bool findQuarticRoots(
    double a,
    double b,
    double c,
    double d,
    const double range[2],
    double step,
    double epsilon,
    double accuracy,
    Double2D roots[4],
    int& numRoots);


} // namespace geom

#endif // ROOTSOLVERS_H_


