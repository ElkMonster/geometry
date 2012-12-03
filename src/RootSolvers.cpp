#include "RootSolvers.h"
#include "Limits.h"
#include <algorithm>


#define FILL_ARRAY2(arr, v1, v2) (arr)[0] = (v1); (arr)[1] = (v2)

typedef std::complex<double> complex;

namespace
{

    const double pi = std::atan(1.) * 4.;


    int
    sign(double x)
    {
        return (x > 0.) ? 1 : ((x == 0.) ? 0 : -1);
    }


    double cubicRoot(double x)
    {
        return (x < 0.) ? -std::pow(-x, 1. / 3.) : std::pow(x, 1. / 3.);
    }


    complex safeSqrt(double z)
    {
        return (z < 0.) ? complex(0., std::sqrt(-z)) : complex(std::sqrt(z), 0.);
    }


    complex complexSqrt(complex z)
    {
        if (z.imag() == 0.)
        {
            return safeSqrt(z.real());
        }
        // Cf. http://en.wikipedia.org/wiki/Square_root#Algebraic_formula
        // (2011/05/03)
        double r = std::sqrt(z.real() * z.real() + z.imag() * z.imag());
        double real = -std::sqrt(0.5 * (r + z.real()));
        double imag = sign(z.imag()) * std::sqrt(0.5 * (r - z.real()));

        return complex(real, imag);
    }


    // 4x^3 + 3alpha*x^2 + 2beta*x + gamma = y
    struct DerivedQuarticFunc : geom::Func {
        double alpha, beta, gamma;

        DerivedQuarticFunc(double alpha, double beta, double gamma)
        :   alpha(alpha),
            beta(beta),
            gamma(gamma)
        {}

        double operator()(double x) const
        {
            return x * ((4. * x * x) + (3. * alpha * x) + (2. * beta)) + gamma;
        }
    };


} // namespace

namespace geom
{


bool
findPositive(
    const Func& func, const double range[2], double step, Double2D& pos)
{
    for (pos.x = range[0]; pos.x < range[1]; pos.x += step)
    {
        pos.y = func(pos.x);
        if (pos.y >= 0.)
        {
            return true;
        }
    }

    if (pos.x != range[1])
    {
        pos.x = range[1];
        pos.y = func(pos.x);
    }

    return (pos.y >= 0.);
}


bool
findNegative(
    const Func& func, const double range[2], double step, Double2D& neg)
{
    for (neg.x = range[0]; neg.x < range[1]; neg.x += step)
    {
        neg.y = func(neg.x);
        if (neg.y <= 0.)
        {
            return true;
        }
    }

    if (neg.x != range[1])
    {
        neg.x = range[1];
        neg.y = func(neg.x);
    }

    return (neg.y <= 0.);
}


bool
findChangeOfSign(
    const Func& func,
    const double range[2],
    double step,
    Double2D& pos,
    Double2D& neg
)
{
    double x = range[0];
    double y = func(x);

    bool success = false;
    double subrange[2];
    subrange[0] = x + step;
    subrange[1] = range[1];
    if (y > 0.)
    {
        success = findNegative(func, subrange, step, neg);
        pos.x = neg.x - step;
        pos.y = func(pos.x);

    }
    else if (y < 0.)
    {
        success = findPositive(func, subrange, step, pos);
        neg.x = pos.x - step;
        neg.y = func(neg.x);
    }
    else
    {
        success = true;
        pos.x = x;
        pos.y = y;
        neg.x = x;
        neg.y = y;
    }
    return success;
}


bool
bisectRoot(
    const Func& func,
    const double range[2],
    double step,
    double epsilon,
    double rootRange[],
    Double2D& root
)
{
    Double2D pos, neg;

    if (!findChangeOfSign(func, range, step, pos, neg))
    {
        return false;
    }

    double dist = std::abs(pos.x - neg.x);

    if (dist > epsilon)
    {
        double posDirection = (pos.x > neg.x) ? -1. : 1.;
        Double2D newPos, newNeg;
        while (dist > epsilon)
        {
            // Push the range edge with the greatest absolute function value
            // closer to the center (near which the root is expected to lie). If
            // such a step crosses the root, push other side's range edge
            // instead.
            if (pos.y > -neg.y)
            {
                newPos.x = pos.x + posDirection * dist / 2.;
                newPos.y = func(newPos.x);

                if (newPos.y >= 0.)
                {
                    pos = newPos;
                }
                else
                {
                    neg.x -= posDirection * dist / 2.;
                    neg.y = func(neg.x);
                }
            }
            else
            {
                newNeg.x = neg.x - posDirection * dist / 2.;
                newNeg.y = func(newNeg.x);

                if (newNeg.y <= 0.)
                {
                    neg = newNeg;
                }
                else
                {
                    pos.x += posDirection * dist / 2.;
                    pos.y = func(pos.x);
                }
            }

            dist = std::abs(pos.x - neg.x);
        }
    }

    if (pos.y != 0. && neg.y != 0.)
    {
        FILL_ARRAY2( rootRange, std::min(pos.x, neg.x), std::max(pos.x, neg.x) );
        root = (pos.y < -neg.y) ? pos : neg;
    }
    else
    {
        double x = (pos.y == 0.) ? pos.x : neg.x;
        FILL_ARRAY2( rootRange, x, x );
        root.x = x;
        root.y = 0.;
    }

    return true;
}


bool
findQuarticRoots(
    double a,
    double b,
    double c,
    double d,
    const double range[2],
    double step,
    double epsilon,
    double accuracy,
    Double2D roots[4],
    int& numRoots
)
{
    QuarticFunc func(a, b, c, d);
    DerivedQuarticFunc derivedFunc(a, b, c);
    numRoots = 0;

    double currRange[] = { range[0], range[1] };
    double rootRange[2];
    int maxIter = 15;

    while (bisectRoot(func, currRange, step, step / 4., rootRange, roots[numRoots]))
    {
        bool rootFound =
            newtonRoot(func, derivedFunc, rootRange[0], accuracy, maxIter, roots[numRoots]);
        if (!rootFound) // Newton failed, bisect instead
        {
            rootFound = bisectRoot(func, currRange, step, epsilon, rootRange, roots[numRoots]);
        }

        if (rootFound)
        {
            // If bisecting has exactly hit a root, add a small amount to the
            // next range's left border to avoid that the same root is found
            // again
            double add = (rootRange[0] == rootRange[1]) ? epsilon : 0.;
            currRange[0] = rootRange[1] + add;
            ++numRoots;
        }
    }

    return numRoots;
}


bool newtonRoot(
    const Func& func,
    const Func& derivedFunc,
    double x0,
    double accuracy,
    int maxIter,
    Double2D& result
)
{
    result.x = x0;
    result.y = func(x0);

    for (int i = 0; i < maxIter; ++i)
    {
        if (std::abs(result.y) < accuracy)
        {
            return true;
        }

        result.x = result.x - result.y / derivedFunc(result.x);
        result.y = func(result.x);
    }

    return false;
}


void
quadraticPolynomialRoots(
    double a,
    double b,
    double c,
    complex roots[2],
    int& numRealRoots,
    int& numComplexRoots)
{
    numRealRoots = 0;
    numComplexRoots = 0;

    if (a == 0.)
    {
        if (b == 0.)
        {
            return;
        }

        roots[0] = complex(-c / b, 0.);
        numRealRoots = 1;
        return;
    }

    // Initial equation: ax^2 + bx + c = 0
    // Transform to: x^2 + (a')x + b' = 0
    double at = a;
    a = b / at;
    b = c / at;

    // x1,2 = -a/2 +- sqrt((a/2)^2 - b)

    double h = a / 2;
    double r = h*h - b;
    if (r > 0.)
    {
        double rsqrt = std::sqrt(r);
        roots[0] = complex(-h + rsqrt, 0.);
        roots[1] = complex(-h - rsqrt, 0.);
        numRealRoots = 2;
        numComplexRoots = 0;
    }
    else if (r == 0.)
    {
        roots[0] = complex(-h, 0.);
        numRealRoots = 1;
        numComplexRoots = 0;
    }
    else
    {
        double rsqrt = std::sqrt(-r);
        roots[0] = complex(-h, rsqrt);
        roots[1] = complex(-h, -rsqrt);
        numRealRoots = 0;
        numComplexRoots = 2;
    }
}


/* Implementation following instructions on
   http://www.arndt-bruenner.de/mathe/scripts/polynome.htm (2011/03/05) */
void
quarticPolynomialRoots(
    double a,
    double b,
    double c,
    double d,
    double e,
    complex roots[4],
    int& numRealRoots,
    int& numComplexRoots)
{
    numRealRoots = 0;
    numComplexRoots = 0;

    if (a == 0.)
    {
        cubicPolynomialRoots(b, c, d, e, roots, numRealRoots, numComplexRoots);
        return;
    }

    // Initial equation: ax^4 + bx^3 + cx^2 + dx + e = 0
    // Transform to: x^4 + (a')x^3 + (b')x^2 + (c')x + d' = 0
    double at = a;
    a = b / at;
    b = c / at;
    c = d / at;
    d = e / at;

    // Substitution: x = y - a / 4, we get y^4 + py^2 + qy + r = 0

    double a2 = a*a;
    double a3 = a2*a;

    // p = b - 3/8 * a^2
    double p = b - (3. / 8. * a2);

    // q = a^3 / 8 - ab / 2 + c
    double q = (a3 / 8.) - (0.5 * a * b) + c;

    // r = -3/256 * a^4 + a^2 b / 16 - ac / 4 + d
    double r = (-3. / 256. * a2*a2) + (a2 * b / 16.) - (0.25 * a * c) + d;

    // Handle special case: bi-quadratic equation (x^4 + bx^2 + d = 0)
    // Set x^2 = z and solve z^2 + bz + d = 0
    if (q == 0.)
    {
        complex zRoots[2];
        int numRealZRoots, numComplexZRoots;
        quadraticPolynomialRoots(1., b, d, zRoots, numRealZRoots, numComplexZRoots);

        int count = 0;
        for (int i = 0; i != numRealZRoots + numComplexZRoots; ++i)
        {
            if (zRoots[i].real() == 0. && zRoots[i].imag() == 0.)
            {
                roots[count++] = zRoots[i];
            }
            else
            {
                complex v = complexSqrt(zRoots[i]);
                roots[count++] = v;
                roots[count++] = -v;
            }
        }

        // Sort such that order is real first, complex last
        for (int i = 0; i != count; ++i)
        {
            if (roots[i].imag() == 0.)
            {
                // If we have complex roots, swap with current real root such
                // that the returned roots' order is real first, complex last
                if (numComplexRoots)
                {
                    complex temp = roots[numRealRoots];
                    roots[numRealRoots] = roots[i];
                    roots[i] = temp;
                }

                numRealRoots++;
            }
            else
            {
                numComplexRoots++;
            }
        }

        return;
    }

    // Further transformation leads to a cubic equation:
    // z^3 - 2pz^2 + (p^2 - 4r)z + q^2 = 0
    int numRealZRoots;
    int numComplexZRoots;
    complex zRoots[3];

    double cuA = -2. * p;
    double cuB = p*p - 4. * r;
    double cuC = q*q;
    cubicPolynomialRoots(1., cuA, cuB, cuC, zRoots, numRealZRoots, numComplexZRoots);


    complex zsqrt[3];
    for (int i = 0; i != 3; ++i)
    {
        zsqrt[i] = complexSqrt(-zRoots[i]);
    }

    double zmul = (zsqrt[0] * zsqrt[1] * zsqrt[2]).real();
    double omega = (sign(zmul) == sign(q)) ? 0.5 : -0.5;

    roots[0] = omega * (zsqrt[0] + zsqrt[1] - zsqrt[2]);
    roots[1] = omega * (zsqrt[0] - zsqrt[1] + zsqrt[2]);
    roots[2] = omega * (-zsqrt[0] + zsqrt[1] + zsqrt[2]);
    roots[3] = omega * (-zsqrt[0] - zsqrt[1] - zsqrt[2]);

    // Re-substitute
    double m = 0.25 * a;
    for (int i = 0; i != 4; ++i)
    {
        roots[i] = roots[i] - m;
        if (roots[i].imag() == 0.)
        {
            // If we have complex roots, swap with current real root such that
            // the returned roots' order is real first, complex last
            if (numComplexRoots)
            {
                complex temp = roots[numRealRoots];
                roots[numRealRoots] = roots[i];
                roots[i] = temp;
            }

            numRealRoots++;
        }
        else
        {
            numComplexRoots++;
        }
    }
}


void quarticPolynomialRoots(
    double a,
    double b,
    double c,
    double d,
    double e,
    double roots[4],
    int& numRoots
)
{
    complex croots[4];
    int numComplexRoots;
    quarticPolynomialRoots(a, b, c, d, e, croots, numRoots, numComplexRoots);
    for (int i = 0; i != numRoots; ++i)
    {
        roots[i] = croots[i].real();
    }
}


void
cubicPolynomialRoots(
    double r,
    double s,
    double t,
    double u,
    complex roots[3],
    int& numRealRoots,
    int& numComplexRoots
)
{
    numRealRoots = 0;
    numComplexRoots = 0;

    if (r == 0.)
    {
        quadraticPolynomialRoots(s, t, u, roots, numRealRoots, numComplexRoots);
        return;
    }

    // Initial equation: rx^3 + sx^2 + tx + u = 0
    // Transform to: x^3 + (r')x^2 + (s')x + t' = 0
    double rt = r;
    r = s / rt;
    s = t / rt;
    t = u / rt;

    // Substitution: x = z - r / 3 leads to: z^3 + pz + q = 0

    double r2 = r*r;

    // p = s - r^2 / 3
    double p = s - r2 / 3.;
    // q = 2r^3 / 27 - rs / 3 + t
    double q = (2. * r2*r / 27.) - (r * s / 3.) + t;
    double p3 = p * p * p;

    // d = (q/2)^2 + (p/3)^3
    double d2 = p3 / 27.;
    double d = 0.25 * q*q + d2;

    complex z[3];

    // Solve with Cardano/Tartaglia
    // d > 0: One real and two complex solutions
    if (d > 0.)
    {
        double dsqrt = std::sqrt(d);
        double a = cubicRoot(-q * 0.5 + dsqrt);
        double b = cubicRoot(-q * 0.5 - dsqrt);
        double ab1 = -0.5 * (a + b);
        double ab2 = 0.5 * (a - b) * std::sqrt(3);

        z[0] = a + b;
        z[1] = complex(ab1, ab2);
        z[2] = complex(ab1, -ab2);
        numRealRoots = 1;
        numComplexRoots = 2;
    }
    else if (d == 0.) // d == 0: Three real solutions, two of them identical
    {
        if (p == 0 || q == 0)
        {
            z[numRealRoots++] = 0.;
        }
        else
        {
            z[numRealRoots++] = 3. * q / p;
            z[numRealRoots++] = (-3. / 2.) * q / p;
        }
    }
    else // d < 0, three different real solutions, casus irreducibilis
    {
        double m = std::sqrt(-4. / 3. * p);
        double n = 1. / 3. * std::acos(-0.5 * q * std::sqrt(-27. / p3));

        for (int i = -1; i != 2; ++i)
        {
            z[numRealRoots++] = (i ? -1. : 1.) * m * std::cos(n + i * (pi / 3.));
        }
    }

    // Re-substitute
    for (int i = 0; i != numRealRoots + numComplexRoots; ++i)
    {
        roots[i] = z[i] - r / 3.;
    }
}


void cubicPolynomialRoots(
    double r,
    double s,
    double t,
    double u,
    double roots[3],
    int& numRoots)
{
    complex croots[3];
    int numComplexRoots;
    cubicPolynomialRoots(r, s, t, u, croots, numRoots, numComplexRoots);
    for (int i = 0; i != numRoots; ++i)
    {
        roots[i] = croots[i].real();
    }
}


} // namespace geom

