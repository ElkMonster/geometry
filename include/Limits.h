#ifndef LIMITS_H_
#define LIMITS_H_


#include <cmath>

#define ZERO_LIMIT (1. / 10000.)

#define NEAR_ZERO(exp) (std::abs(exp) <= ZERO_LIMIT)

#define NEAR_EQUAL(v1, v2) (NEAR_ZERO((v1) - (v2)))

#define GREATERTHAN_ZERO(exp) ((exp) > ZERO_LIMIT)

#define LESSTHAN_ZERO(exp) ((exp) < -ZERO_LIMIT)

#endif // LIMITS_H_
