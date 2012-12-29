#ifndef SPHERE_HH
# define SPHERE_HH
# include "vector.hh"
# include "ray.hh"

 class Sphere
 {
   public:
     Vec3 pos_, emission_, color_;
     Refl_t refl_; // reflection type (DIFFuse, SPECular, REFRactive)
     Sphere(double rad, Vec3 p, Vec3 e, Vec3 c, Refl_t refl)
       : radius_(rad),
         pos_(p),
         emission_(e),
         color_(c),
         refl_(refl)
   {}

     double radius_;

     // Find the point ON the ray which intersects the sphere.
     double intersect(const Ray &r) const
     {
       // Quadratic equation.
       Vec3 op = pos_ - r.o_;
       double t;
       double eps = 1e-4;
       double b = op.dot(r.d_);
       // b² - 4ac.
       double det = b * b - op.dot(op) + radius_ * radius_;

       // det < 0 Ray behind the sphere.
       if (det < 0)
         return 0;
       else
         det = sqrt(det);

       // Two intersection, x1 and x2.
       return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
     }
 };

#endif
