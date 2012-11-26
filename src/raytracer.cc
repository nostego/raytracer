#include "raytracer.hh"
#define EPSILON 0.00001

Object* Raytracer::collide_with_object(Ray ray, double& impact)
{
  double minimpact = 100000;
  int kmin = -1;

  for (size_t k = 0; k < objects.size(); ++k)
  {
    if (objects[k]->collide(ray, impact))
    {
      if (minimpact > impact)
      {
        kmin = k;
        minimpact = impact;
      }
    }
  }
  if (kmin < 0)
    return 0;
  else
  {
    impact = minimpact;
    return objects[kmin];
  }
}

Vec3<int> Raytracer::trace(Ray ray, int bound)
{
  Object * obj;
  double impact;

  if ((bound < 2) && (obj = collide_with_object(ray, impact)))
  {
    Object* obj2;
    double lumen = obj->m.lumen + 0.1;

    for (size_t k = 0; k < objects.size(); ++k)
    {
      if (obj != objects[k])
      {
        Ray ray2;

        ray2.ori = ray.dir * impact;
        ray2.ori = ray2.ori + ray.ori;
        ray2.dir = objects[k]->center() - ray2.ori;
        ray2.dir.normalize();
	ray2.ori = ray2.ori + ray2.dir * EPSILON;

        if ((obj2 = collide_with_object(ray2, impact)))
          lumen += obj2->m.lumen;
      }
    }
    return obj->m.col * lumen;
  }
  else
    return Vec3<int>();
}

Raytracer::Raytracer()
{
  Sphere* tmp;

  tmp = new Sphere;
  tmp->c.x = 2.0;
  tmp->c.y = 0.0;
  tmp->c.z = 5.0;
  tmp->r = 2.0;
  tmp->m.col.x = 255;
  tmp->m.col.y = 0;
  tmp->m.col.z = 0;
  tmp->m.lumen = 0.0;
  objects.push_back(tmp);

  tmp = new Sphere;
  tmp->c.x = 5.0;
  tmp->c.y = 5.0;
  tmp->c.z = 5.0;
  tmp->r = 2;
  tmp->m.col.x = 0;
  tmp->m.col.y = 255;
  tmp->m.col.z = 0;
  tmp->m.lumen = 1.0;
  objects.push_back(tmp);
}
