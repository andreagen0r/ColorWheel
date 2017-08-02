#ifndef PMATH_H
#define PMATH_H

#include <cmath>

namespace physis
{
namespace math
{

template <typename T>
///
/// \brief Converts degrees to radians.
/// \param angle
/// \return
///
inline T degreeToRadians(const T &angle) { return angle * (M_PI / 180); }

template <typename T>
///
/// \brief Converts radians to degrees.
/// \param angle
/// \return
///
inline T radiansToDegrees(const T &angle) { return angle * (180 / M_PI); }

} // END NAMESPACE MATH
} // END NAMESPACE PHYSIS

#endif // PMATH_H
