#ifndef TRIANGLE_HH_
# define TRIANGLE_HH_

# include "material.hh"
# include "object.hh"

struct Ray;
struct Triangle : public Object
{
  virtual bool collide(Ray ray, double& impact);
  virtual Vec3<double> center();
  Vec3<double> normal();

  Vec3<double> a;
  Vec3<double> b;
  Vec3<double> c;
};

#endif // !TRIANGLE_HH_
