#ifndef RAYTRACER_HH_
# define RAYTRACER_HH_
# include "sphere.hh"
# include <vector>
# include <cmath>

struct Ray
{
  Vec3<double> ori;
  Vec3<double> dir;
};

class Raytracer
{
public:
  Raytracer();
  Object* collide_with_object(Ray ray, double& impact);
  Vec3<int> trace(Ray ray, int bound);

  std::vector<Object*> objects;
};

#endif // !RAYTRACER_HH_
