#ifndef NKMATH_H
#define NKMATH_H

namespace nkn
{
namespace math
{

template <typename T>
constexpr T PI = T(3.14159265358979323846264338327950288L);

/// \brief This metafunction converts degrees to radians.
/// \param T angle in degrees.
/// \return T angle in radians.
template <typename T>
constexpr inline T degreeToRadians(const T angle)
{
    return angle * ((PI<T>) / 180);
}

/// \brief This metafunction converts radians to degrees.
/// \param T angle in radians.
/// \return T angle in degrees.
template <typename T>
constexpr inline T radiansToDegrees(const T angle)
{
    return angle * (180 / PI<T>);
}

} // END NAMESPACE MATH
} // END NAMESPACE PHYSIS

#endif // NKMATH_H
