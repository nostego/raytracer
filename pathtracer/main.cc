#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.hh"
#include "ray.hh"
#include "sphere.hh"

Sphere spheres[] =
{
  //Scene: radius, position, emission, color, material
  // Light.
  Sphere(600, Vec3(90, 681.6-.27, 81.6), Vec3(12, 12, 12), Vec3(), DIFF),
  // Light 2.
  Sphere(600, Vec3(25, 681.6-.27, 81.6), Vec3(12, 12, 12), Vec3(), DIFF),
  // Left wall.
//  Sphere(1e5, Vec3(1e5+1, 40.8, 81.6), Vec3(),Vec3(.75, .25, .25),DIFF),
  // Right wall.
 // Sphere(1e5, Vec3(-1e5+99, 40.8, 81.6),Vec3(),Vec3(.25, .25, .75),DIFF),
  // Back wall.
  Sphere(1e5, Vec3(50, 40.8, 1e5), Vec3(), Vec3(.75, .75, .75), DIFF),
  // Front wall.
 // Sphere(1e5, Vec3(50, 40.8, -1e5+170), Vec3(),Vec3(), DIFF),
  // Bottom Wall.
  Sphere(1e5, Vec3(50, 1e5, 81.6), Vec3(), Vec3(.75, .25, .25), DIFF),
  // Top Wall.
  Sphere(1e5, Vec3(50, -1e5+81.6, 81.6), Vec3(), Vec3(.75, .75, .75), DIFF),
  // First Ball.
  //Sphere(16.5,Vec3(27,16.5,47), Vec3(),Vec3(1,1,1)*.999, SPEC),
  // Second Ball.
  Sphere(13.5,Vec3(93,16.5,78), Vec3(),Vec3(1,1,1)*.999, REFR),
  Sphere(10.5,Vec3(40, 26.5,48), Vec3(),Vec3(1,1,1)*.999, DIFF),
  Sphere(16.0,Vec3(10, 29.5,98), Vec3(),Vec3(1,1,1)*.999, REFR)
};

// Take values between 0 and 1.
inline double clamp(double x)
{
  return x < 0 ? 0 : x > 1 ? 1 : x;
}

inline int toInt(double x)
{
  return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

inline bool intersect(const Ray &r, double &t, int &id)
{
  double n = sizeof(spheres) / sizeof(Sphere);
  double d;
  double inf = t = 1e20;

  for (int i = int(n); i--;)
  {
    if ((d = spheres[i].intersect(r)) && d < t)
    {
      t = d;
      id = i;
    }
  }

  return t < inf;
}

Vec3 radiance(const Ray &r, int depth, unsigned short *Xi)
{
  // Distance to intersection.
  double t;

  // Id of the intersected object.
  int id = 0;

  // No intersection, put black.
  if (!intersect(r, t, id))
    return Vec3();

  const Sphere &obj = spheres[id];

  // Intersection.
  Vec3 x = r.o_ + r.d_ * t;
  // Distance between center of obs/intersection.
  Vec3 n = (x - obj.pos_).norm();
  Vec3 nl = n.dot(r.d_) < 0 ? n : n * -1;
  Vec3 f = obj.color_;

  double p = f.x_ > f.y_ && f.x_ > f.z_ ? f.x_ : f.y_ > f.z_ ? f.y_ : f.z_; // max refl
  if (++depth > 5)
  {
    if (erand48(Xi) < p)
      f = f * (1 / p);
    else
      return obj.emission_; //R.R.
  }

  if (obj.refl_ == DIFF)
  { // Ideal DIFFUSE reflection
    double r1 = 2 * M_PI * erand48(Xi);
    double r2 = erand48(Xi);
    double r2s = sqrt(r2);
    Vec3 w = nl;
    Vec3 u = ((fabs(w.x_) > .1 ? Vec3(0, 1) : Vec3(1)) % w).norm();
    Vec3 v = w % u;
    Vec3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
    return obj.emission_ + f.mult(radiance(Ray(x, d), depth, Xi));
  }
  else if (obj.refl_ == SPEC) // Ideal SPECULAR reflection
    return obj.emission_ + f.mult(radiance(Ray(x, r.d_ - n * 2 * n.dot(r.d_)), depth, Xi));
  Ray reflRay(x, r.d_ - n * 2 * n.dot(r.d_)); // Ideal dielectric REFRACTION

  bool into = n.dot(nl) > 0; // Ray from outside going in?
  double nc = 1;
  double nt = 1.5;
  double nnt = into ? nc / nt : nt / nc;
  double ddn = r.d_.dot(nl);
  double cos2t = 1 - pow(nnt, 2) * (1 - pow(ddn, 2));

  if (cos2t < 0) // Total internal reflection
    return obj.emission_ + f.mult(radiance(reflRay,depth,Xi));

  Vec3 tdir = (r.d_ * nnt - n * ((into ? 1 :-1) * (ddn * nnt + sqrt(cos2t)))).norm();
  double a = nt - nc;
  double b = nt + nc;
  double R0 = pow(a, 2) / (pow(b, 2));
  double c = 1 - (into ? - ddn : tdir.dot(n));
  double Re = R0 + (1 - R0) * pow(c, 5);
  double Tr = 1 - Re;
  double P = .25 +.5 * Re;
  double RP = Re / P;
  double TP = Tr / (1 - P);
  return obj.emission_ + f.mult(depth > 2 ? (erand48(Xi) < P ? // Russian roulette
				   radiance(reflRay, depth, Xi) * RP:radiance(Ray(x,tdir),depth,Xi) * TP) :
			radiance(reflRay,depth,Xi)*Re+radiance(Ray(x,tdir),depth,Xi)*Tr);
}

void write_image(Vec3 *img, int w, int h)
{
  FILE *f = fopen("image.ppm", "w"); // Write image to PPM file.

  // Creating a PPM image.
  fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);

  // Write the whole image.
  for (int i = 0; i < w * h; i++)
    fprintf(f,"%d %d %d ", toInt(img[i].x_), toInt(img[i].y_), toInt(img[i].z_));
}

#include <iostream>
int main(int argc, char *argv[])
{
  int w = 512 / 3;
  int h = 384 / 3;
  // Number of iterations.
  int samps = argc == 2 ? atoi(argv[1]) / 4 : 1;

  // Camera: Position, direction.
  Ray cam(Vec3(50, 52, 295.6), Vec3(0, -0.042612, -1).norm());

  // 0.5135 = field of view angle.
  Vec3 cx = Vec3(w * 0.5135 / h);
  Vec3 cy = (cx % cam.d_).norm() * 0.5135;
  Vec3 r;
  Vec3 *c = new Vec3[w * h];

  for (int y = 0; y < h; y++)
  {
    fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samps * 4, 100. * y / (h - 1));
    unsigned short Xi[3] = {0, 0, pow(y, 3)};
    for (unsigned short x = 0; x < w; x++)
    {
      // From last line (from left to right) to the first line.
      int i = (h - y - 1) * w + x;
      for (int sy = 0; sy < 2; sy++)
      // 2x2 subpixel rows.
      {
	for (int sx = 0; sx < 2; sx++)
        {
          r = Vec3();
          // 2x2 subpixel cols.
	  for (int s = 0; s < samps; s++)
          {
            // Tent filter.
	    double r1 = 2 * erand48(Xi);
            double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
	    double r2 = 2 * erand48(Xi);
            double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);

            // Find the ray direction according to the camera.
	    Vec3 d = cx * (((sx + 0.5 + dx) / 2 + x) / w - 0.5) +
	             cy * (((sy + 0.5 + dy) / 2 + y) / h - 0.5) + cam.d_;

            // Radiance divided by every samples.
	    r = r + radiance(Ray(cam.o_ + d, d.norm()), 0, Xi) * (1.0 / samps);
	  }
          // Camera rays are pushed ^^^^^ forward to start in interior
	  c[i] = c[i] + Vec3(clamp(r.x_), clamp(r.y_), clamp(r.z_)) * .25;
	}
      }
    }
  }
  write_image(c, w, h);
}
