#ifndef TRIANGLE_HH
# define TRIANGLE_HH
#include <iostream>
# include "vector.hh"
# include "ray.hh"

 class Triangle
 {
   public:
     Vec3 p1_;
     Vec3 p2_;
     Vec3 p3_;
     Vec3 emission_;
     Vec3 color_;
     Refl_t refl_;
     Triangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 e, Vec3 c, Refl_t refl)
       : p1_(p1),
         p2_(p2),
         p3_(p3),
         emission_(e),
         color_(c),
         refl_(refl)
   {}

     // Find the point ON the ray which intersects the triangle.
     double intersect(const Ray &r) const
     {
       Vec3 u = p2_ - p1_;
       Vec3 v = p3_ - p1_;

       Vec3 w(r.o_.x_ - p1_.x_, r.o_.y_ - p1_.y_, r.o_.z_ - p1_.z_);
       double D = -((u % v).dot(r.d_));

       double a = -((w % v).dot(r.d_)) / D;
       double b = -((u % w).dot(r.d_)) / D;
       double t = ((u % v).dot(w)) / D;

       if (a > 0 && b > 0 && a + b <= 1)
         return t;
       else
         return INT_MAX;

     }
 };

#endif
