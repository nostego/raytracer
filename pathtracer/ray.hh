#ifndef RAY_HH
# define RAY_HH
# include "vector.hh"

 enum Refl_t { DIFF, SPEC, REFR }; // material types, used in radiance()

 class Ray
 {
   public:
     Vec3 o_, d_;
     Ray(Vec3 o, Vec3 d);
 };
#endif
