#ifndef Vec3_HH
# define Vec3_HH
# include <math.h>

 class Vec3
 {
   public:
     double x_, y_, z_;
     Vec3(double x = 0, double y = 0, double z = 0);
     Vec3 operator+(const Vec3 &b) const;
     Vec3 operator-(const Vec3 &b) const;
     Vec3 operator*(double b) const;
     Vec3 mult(const Vec3 &b) const;
     Vec3& norm();
     double dot(const Vec3 &b) const;
     Vec3 operator%(Vec3 &b) const;
 };
#endif
