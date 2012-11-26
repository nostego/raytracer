#include "sphere.hh"
#include "raytracer.hh"

bool Sphere::collide(Ray ray, double& impact)
{
  double lsq = -1;

  c = c - ray.ori;
  lsq = pow(ray.dir*c, 2.0) -
    (ray.dir* ray.dir)*(c*c - pow(r, 2.0));

  if (lsq >= 0)
  {
    double d = ray.dir*c + sqrt(lsq);
    double d2 = ray.dir*c - sqrt(lsq);

    if (d2 >= 0)
      impact = d2;
    else
      impact = d;
    if (impact >= 0)
    {
      c = c + ray.ori;
      return true;
    }
  }
  c = c + ray.ori;
  return false;
}

Vec3<double> Sphere::center()
{
  return c;
}
