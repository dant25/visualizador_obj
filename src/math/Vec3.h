#ifndef _VEC3_H_
#define _VEC3_H_

#include "../utility.h"

class Vec3 {
public:
	Vec3();
	Vec3(Real _x, Real _y, Real _z);
	
    Vec3 operator+(const Vec3& v) const;
	Vec3& operator+=(const Vec3& v);

	Vec3 operator-(const Vec3& v) const;
	Vec3& operator-=(const Vec3& v);

	Vec3 operator*(Real s) const;
	Vec3& operator*=(Real s);

	Vec3 operator/(Real s) const;
	Vec3& operator/=(Real s);

	bool operator==(const Vec3& v) const;
	bool operator!=(const Vec3& v) const;

	Real length() const;

	Real x, y, z;
};

Real dot(const Vec3& v1, const Vec3& v2);
Vec3 cross(const Vec3& v1, const Vec3& v2);
Vec3 normalize(const Vec3& v);
Real angle(const Vec3& v1, const Vec3& v2);

#endif
