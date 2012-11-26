#ifndef OBJECT_HH_
# define OBJECT_HH_

struct Ray;
struct Object
{
  virtual bool collide(Ray ray, double& impact) = 0;
  virtual Vec3<double> center() = 0;
  Material m;
};

#endif // !OBJECT_HH_
