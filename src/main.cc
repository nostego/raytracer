#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdio>
#include "raytracer.hh"

int main()
{
  std::ofstream out;
  size_t w = 600;
  size_t h = 600;
  double epsi = 0.4;
  Vec3<double> camera(0.5, 0.5, -epsi);
  Raytracer rt;

  out.open("out.ppm");
  out << "P3" << std::endl
      << "# Raytracer" << std::endl
      << w << " " << h
      << std::endl
      << "255"
      << std::endl;

  for (size_t x = 0; x < w; ++x)
    for (size_t y = 0; y < h; ++y)
    {
      Ray r;
      r.ori = Vec3<double>(x / (double)w, y / (double)h, 0.0);
      r.dir = Vec3<double>(x / (double)w - camera.x,
                    y / (double)h - camera.y,
                    -camera.z);

      r.dir.normalize();
      rt.trace(r, 0).print(out);
    }
  out.close();
  return 0;
}
