#ifndef PHVECTOR3_H
#define PHVECTOR3_H

#include <smmintrin.h> // SSE 4.2

namespace Physis {

#if !defined(__forceinline)
#define __forceinline          inline __attribute__((always_inline))
#endif

// Simple SSE 4.2 vector class
#ifdef __GNUC__
class __attribute__((aligned (16))) PhVector3
#else
_MM_ALIGN16 class PhVector3
#endif
{
 public:

  // constructors
  __forceinline PhVector3() : m_value(_mm_setzero_ps()) {}
  __forceinline PhVector3(float x, float y, float z) : m_value(_mm_set_ps(0, z, y, x)) {}
  __forceinline PhVector3(__m128 m) : m_value(m) {}

  // arithmetic operators with vector3
  __forceinline PhVector3 operator+(const PhVector3& b) const { return _mm_add_ps(m_value, b.m_value); }
  __forceinline PhVector3 operator-(const PhVector3& b) const { return _mm_sub_ps(m_value, b.m_value); }
  __forceinline PhVector3 operator*(const PhVector3& b) const { return _mm_mul_ps(m_value, b.m_value); }
  __forceinline PhVector3 operator/(const PhVector3& b) const { return _mm_div_ps(m_value, b.m_value); }

  // op= operators
  __forceinline PhVector3& operator+=(const PhVector3& b) { m_value = _mm_add_ps(m_value, b.m_value); return *this; }
  __forceinline PhVector3& operator-=(const PhVector3& b) { m_value = _mm_sub_ps(m_value, b.m_value); return *this; }
  __forceinline PhVector3& operator*=(const PhVector3& b) { m_value = _mm_mul_ps(m_value, b.m_value); return *this; }
  __forceinline PhVector3& operator/=(const PhVector3& b) { m_value = _mm_div_ps(m_value, b.m_value); return *this; }

  // arithmetic operators with float
  __forceinline PhVector3 operator+(float b) const { return _mm_add_ps(m_value, _mm_set1_ps(b)); }
  __forceinline PhVector3 operator-(float b) const { return _mm_sub_ps(m_value, _mm_set1_ps(b)); }
  __forceinline PhVector3 operator*(float b) const { return _mm_mul_ps(m_value, _mm_set1_ps(b)); }
  __forceinline PhVector3 operator/(float b) const { return _mm_div_ps(m_value, _mm_set1_ps(b)); }

  // op= operators with float
  __forceinline PhVector3& operator+=(float b) { m_value = _mm_add_ps(m_value, _mm_set1_ps(b)); return *this; }
  __forceinline PhVector3& operator-=(float b) { m_value = _mm_sub_ps(m_value, _mm_set1_ps(b)); return *this; }
  __forceinline PhVector3& operator*=(float b) { m_value = _mm_mul_ps(m_value, _mm_set1_ps(b)); return *this; }
  __forceinline PhVector3& operator/=(float b) { m_value = _mm_div_ps(m_value, _mm_set1_ps(b)); return *this; }

  // cross product
  __forceinline PhVector3 cross(const PhVector3& b) const
  {
   return _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 0, 2, 1)),
                                _mm_shuffle_ps(b.m_value, b.m_value, _MM_SHUFFLE(3, 1, 0, 2))),
                     _mm_mul_ps(_mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 1, 0, 2)),
                                _mm_shuffle_ps(b.m_value, b.m_value, _MM_SHUFFLE(3, 0, 2, 1))));
  }

  // dot product with another vector
  __forceinline float dot(const PhVector3& b) const { return _mm_cvtss_f32(_mm_dp_ps(m_value, b.m_value, 0x71)); }
  // length of the vector
  __forceinline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(m_value, m_value, 0x71))); }
  // 1/length() of the vector
  __forceinline float rlength() const { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(m_value, m_value, 0x71))); }
  // returns the vector scaled to unit length
  __forceinline PhVector3 normalize() const { return _mm_mul_ps(m_value, _mm_rsqrt_ps(_mm_dp_ps(m_value, m_value, 0x7F))); }

  // overloaded operators that ensure alignment
#ifdef WIN32
  __forceinline void* operator new[](size_t x) { return _aligned_malloc(x, 16); }
  __forceinline void operator delete[](void* x) { if (x) _aligned_free(x); }
#endif

#ifdef __APPLE__
  __forceinline void* operator new[](size_t x) { return _mm_malloc(x, 16); }
  __forceinline void operator delete[](void* x) { if (x) _mm_free(x); }
#endif

  // Member variables
  union
  {
   struct { float x, y, z; };
   __m128 m_value;
  };
};

__forceinline PhVector3 operator+(float a, const PhVector3& b) { return b + a; }
__forceinline PhVector3 operator-(float a, const PhVector3& b) { return PhVector3(_mm_set1_ps(a)) - b; }
__forceinline PhVector3 operator*(float a, const PhVector3& b) { return b * a; }
__forceinline PhVector3 operator/(float a, const PhVector3& b) { return PhVector3(_mm_set1_ps(a)) / b; }

} // END NAMESPACE PHYSIS
#endif // PHVECTOR3_H
