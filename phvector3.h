#ifndef PHVECTOR3_H
#define PHVECTOR3_H

#include <nmmintrin.h>

namespace Physis {

// Simple SSE 4.2 vector class
class PhVector3
{
 public:

  // constructors
  inline PhVector3() : m_value(_mm_setzero_ps()) {}
  inline PhVector3(float x, float y, float z) : m_value(_mm_set_ps(0, z, y, x)) {}
  inline PhVector3(__m128 m) : m_value(m) {}

  // arithmetic operators with vector3
  inline PhVector3 operator+(const PhVector3& b) const { return _mm_add_ps(m_value, b.m_value); }
  inline PhVector3 operator-(const PhVector3& b) const { return _mm_sub_ps(m_value, b.m_value); }
  inline PhVector3 operator*(const PhVector3& b) const { return _mm_mul_ps(m_value, b.m_value); }
  inline PhVector3 operator/(const PhVector3& b) const { return _mm_div_ps(m_value, b.m_value); }

  // op= operators
  inline PhVector3& operator+=(const PhVector3& b) { m_value = _mm_add_ps(m_value, b.m_value); return *this; }
  inline PhVector3& operator-=(const PhVector3& b) { m_value = _mm_sub_ps(m_value, b.m_value); return *this; }
  inline PhVector3& operator*=(const PhVector3& b) { m_value = _mm_mul_ps(m_value, b.m_value); return *this; }
  inline PhVector3& operator/=(const PhVector3& b) { m_value = _mm_div_ps(m_value, b.m_value); return *this; }

  // arithmetic operators with float
  inline PhVector3 operator+(float b) const { return _mm_add_ps(m_value, _mm_set1_ps(b)); }
  inline PhVector3 operator-(float b) const { return _mm_sub_ps(m_value, _mm_set1_ps(b)); }
  inline PhVector3 operator*(float b) const { return _mm_mul_ps(m_value, _mm_set1_ps(b)); }
  inline PhVector3 operator/(float b) const { return _mm_div_ps(m_value, _mm_set1_ps(b)); }

  // op= operators with float
  inline PhVector3& operator+=(float b) { m_value = _mm_add_ps(m_value, _mm_set1_ps(b)); return *this; }
  inline PhVector3& operator-=(float b) { m_value = _mm_sub_ps(m_value, _mm_set1_ps(b)); return *this; }
  inline PhVector3& operator*=(float b) { m_value = _mm_mul_ps(m_value, _mm_set1_ps(b)); return *this; }
  inline PhVector3& operator/=(float b) { m_value = _mm_div_ps(m_value, _mm_set1_ps(b)); return *this; }

  // cross product
  inline PhVector3 cross(const PhVector3& b) const
  {
   return _mm_sub_ps(
    _mm_mul_ps(_mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b.m_value, b.m_value, _MM_SHUFFLE(3, 1, 0, 2))),
    _mm_mul_ps(_mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b.m_value, b.m_value, _MM_SHUFFLE(3, 0, 2, 1)))
   );
  }

  // dot product with another vector
  inline float dot(const PhVector3& b) const { return _mm_cvtss_f32(_mm_dp_ps(m_value, b.m_value, 0x71)); }
  // length of the vector
  inline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(m_value, m_value, 0x71))); }
  // 1/length() of the vector
  inline float rlength() const { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(m_value, m_value, 0x71))); }
  // returns the vector scaled to unit length
  inline PhVector3 normalize() const { return _mm_mul_ps(m_value, _mm_rsqrt_ps(_mm_dp_ps(m_value, m_value, 0x7F))); }

  // overloaded operators that ensure alignment
#ifdef WIN32
  inline void* operator new[](size_t x) { return _aligned_malloc(x, 16); }
  inline void operator delete[](void* x) { if (x) _aligned_free(x); }
#endif

#ifdef __APPLE__
  inline void* operator new[](size_t x) { return _mm_malloc(x, 16); }
  inline void operator delete[](void* x) { if (x) _mm_free(x); }
#endif

  // Member variables
  union
  {
   struct { float x, y, z; };
   __m128 m_value;
  };
};

inline PhVector3 operator+(float a, const PhVector3& b) { return b + a; }
inline PhVector3 operator-(float a, const PhVector3& b) { return PhVector3(_mm_set1_ps(a)) - b; }
inline PhVector3 operator*(float a, const PhVector3& b) { return b * a; }
inline PhVector3 operator/(float a, const PhVector3& b) { return PhVector3(_mm_set1_ps(a)) / b; }

} // END NAMESPACE PHYSIS
#endif // PHVECTOR3_H



//#include <nmmintrin.h>


