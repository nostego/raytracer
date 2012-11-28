#include "triangle.hh"
#include "raytracer.hh"

bool Triangle::collide(Ray ray, double& impact)
{
  Vec3<double> norma = normal();
  double d = a.z / norma.z;
  double t = -(ray.ori * norma + d) / (ray.dir * norma);

  if (t < 0)
    return false;
  else
  {
    {
      Vec3<double> P = ray.ori + ray.dir * t;
      Vec3<double> V1 = b - P;
      Vec3<double> V2 = c - P;
      Vec3<double> N1 = V2^V1;
      N1.normalize();
      double d1 = -(ray.ori*N1);

      if ((P*N1+d1) < 0)
        return false;
    }
    {
      Vec3<double> P = ray.ori + ray.dir * t;
      Vec3<double> V1 = a - P;
      Vec3<double> V2 = c - P;
      Vec3<double> N1 = V2^V1;
      N1.normalize();
      double d1 = -(ray.ori*N1);

      if ((P*N1+d1) < 0)
        return false;
    }
    {
      Vec3<double> P = ray.ori + ray.dir * t;
      Vec3<double> V1 = b - P;
      Vec3<double> V2 = a - P;
      Vec3<double> N1 = V2^V1;
      N1.normalize();
      double d1 = -(ray.ori*N1);

      if ((P*N1+d1) < 0)
        return false;
    }

    impact = t;
    return true;
  }
}

Vec3<double> Triangle::normal()
{
  Vec3<double> ret((b - a)^(c - a));
  ret.normalize();
  return ret;
}

Vec3<double> Triangle::center()
{
  return (a + b + c) * (1.0/3.0);
}
