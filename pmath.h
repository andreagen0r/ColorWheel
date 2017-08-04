#ifndef PMATH_H
#define PMATH_H

#include <cmath>

namespace physis
{
namespace math
{

/// \brief This metafunction converts degrees to radians.
/// \param T angle in degrees.
/// \return T angle in radians.
template <typename T>
inline T degreeToRadians(const T &angle) { return angle * (M_PI / 180); }

/// \brief This metafunction converts radians to degrees.
/// \param T angle in radians.
/// \return T angle in degrees.
template <typename T>
inline T radiansToDegrees(const T &angle) { return angle * (180 / M_PI); }

} // END NAMESPACE MATH
} // END NAMESPACE PHYSIS

#endif // PMATH_H
