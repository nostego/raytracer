#ifndef SPHERE_HH_
# define SPHERE_HH_

# include "material.hh"
# include "object.hh"

struct Ray;
struct Sphere : public Object
{
  virtual bool collide(Ray ray, double& impact);
  virtual Vec3<double> center();
  Vec3<double> c;
  double r;
};

#endif // !SPHERE_HH_
