#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "vector.hh"
#include "ray.hh"
#include "sphere.hh"
#include "triangle.hh"

// Scene 1.
/*
Sphere spheres[] =
{
  Sphere(1e3,   Vec3(1,1,-2)*1e4,    Vec3(1,1,1)*5e2,     Vec3(), DIFF),
  Sphere(3e2,   Vec3(.6,.2,-2)*1e4,    Vec3(1,1,1)*5e3,     Vec3(), DIFF),
  Sphere(2.5e3,   Vec3(.82,.92,-2)*1e4,    Vec3(1,1,1)*.8e2,     Vec3(), DIFF),

  Sphere(2.5e4, Vec3(50, 0, 0),     Vec3(1,1,1)*1e-3,    Vec3(.2,.2,1)*0.0075, DIFF),
  Sphere(2.5e4, Vec3(50, 0, 0),  Vec3(0.114, 0.133, 0.212)*1e-2,  Vec3(.216,.384,1)*0.0007, DIFF),

  Sphere(2.5e4, Vec3(50, 0, 0),  Vec3(0.114, 0.133, 0.212)*1e-2,  Vec3(.216,.384,1)*0.003, DIFF),

  Sphere(5e0,   Vec3(-.2,0.16,-1)*1e4, Vec3(1.00, 0.843, 0.698)*1e2,   Vec3(), DIFF),
  Sphere(5e0,   Vec3(0,  0.18,-1)*1e4, Vec3(1.00, 0.851, 0.710)*1e2,  Vec3(), DIFF),
  Sphere(5e0,   Vec3(.3, 0.15,-1)*1e4, Vec3(0.671, 0.780, 1.00)*1e2,   Vec3(), DIFF),
  Sphere(3.5e4,   Vec3(600,-3.5e4+1, 300), Vec3(),   Vec3(.6,.8,1)*.01,  REFR),
  Sphere(5e4,   Vec3(-500,-5e4+0, 0),   Vec3(),      Vec3(1,1,1)*.35,  DIFF),
  Sphere(16.5,  Vec3(27,0,47),         Vec3(),              Vec3(1,1,1)*.33, DIFF),
  Sphere(7,     Vec3(27+8*sqrt(2),0,47+8*sqrt(2)),Vec3(),  Vec3(1,1,1)*.33,  DIFF),
  Sphere(500,   Vec3(-1e3,-300,-3e3), Vec3(),  Vec3(1,1,1)*.351,    DIFF),
  Sphere(830,   Vec3(0,   -500,-3e3), Vec3(),  Vec3(1,1,1)*.354,    DIFF),
  Sphere(490,  Vec3(1e3,  -300,-3e3), Vec3(),  Vec3(1,1,1)*.352,    DIFF),
};
*/


Triangle triangles[] =
{
  Triangle(Vec3(15, 33.5, 98), Vec3(27, 33.5, 98), Vec3(19, 48.4, 98), Vec3(),Vec3(1,1,1)*.999, REFR)
};
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

inline bool intersect_sphere(const Ray &r, double &t, int &id)
{
  double n = sizeof(spheres) / sizeof(Sphere);
  double d;
  double inf = t = INT_MAX;

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

inline bool intersect_triangle(const Ray &r, double &t, int &id)
{
  double n = sizeof(triangles) / sizeof(Triangle);
  double d;
  double inf = t = INT_MAX;

  for (int i = int(n); i--;)
  {
    if ((d = triangles[i].intersect(r)) && d < t)
    {
      t = d;
      id = i;
    }
  }

  return t < inf;
}

Vec3 radiance(const Ray &r, int depth, unsigned short *Xi)
{
  bool is_sphere = false;
  bool is_triangle = false;
  // Distance to intersection.
  double ts;
  double tt;

  // Id of the intersected object.
  int ids = 0;
  int idt = 0;

  // No intersection, put black.
  is_sphere = intersect_sphere(r, ts, ids);
  is_triangle = intersect_triangle(r, tt, idt);
  if (!is_sphere && !is_triangle)
    return Vec3();

  Vec3 x, n, nl, f;
  Vec3 em;
  Refl_t refl;
  if (is_sphere)
  {
    const Sphere obj = spheres[ids];
    x = r.o_ + r.d_ * ts;
    // Intersection point.
    // Sphere normal.
    n = (x - obj.pos_).norm();
    // Oriented Sphere normal. (Is is entering of exiting).
    nl = n.dot(r.d_) < 0 ? n : n * -1;
    f = obj.color_;
    refl = obj.refl_;
    em = obj.emission_;
  }
  else
  {
    const Triangle &obj = triangles[idt];
    x = r.o_ + r.d_ * ts;
    // Intersection point.
    // Triangle normal.
    //n = (x - obj.pos_).norm();
    // Oriented Sphere normal. (Is is entering of exiting).
    //nl = n.dot(r.d_) < 0 ? n : n * -1;
    f = obj.color_;
  }


  double p = f.x_ > f.y_ && f.x_ > f.z_ ? f.x_ : f.y_ > f.z_ ? f.y_ : f.z_; // max refl
  if (++depth > 5)
  {
    if (erand48(Xi) < p)
      f = f * (1 / p);
    else
      return em; //R.R.
  }

  if (refl == DIFF)
  { // Ideal DIFFUSE reflection

    // Random Angle.
    double r1 = 2 * M_PI * erand48(Xi);
    double r2 = erand48(Xi);
    // Random distance from center.
    double r2s = sqrt(r2);
    // Generate an unit hemisphere for random reflection.
    Vec3 w = nl;
    Vec3 u = ((fabs(w.x_) > .1 ? Vec3(0, 1) : Vec3(1)) % w).norm();
    Vec3 v = w % u;
    // Random reflection array.
    Vec3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
    return em + f.mult(radiance(Ray(x, d), depth, Xi));
  }
  // Ideal specular reflection (Mirror).
  else if (refl == SPEC)
    return em + f.mult(radiance(Ray(x, r.d_ - n * 2 * n.dot(r.d_)), depth, Xi));

  // Ideal dielectric REFRACTION

  // Reflected ray.
  Ray reflRay(x, r.d_ - n * 2 * n.dot(r.d_));

  // Entering or exiting ?
  bool into = n.dot(nl) > 0;
  double nc = 1;
  // Refractive index.
  double nt = 1.5;
  double nnt = into ? nc / nt : nt / nc;
  double ddn = r.d_.dot(nl);
  double cos2t = 1 - pow(nnt, 2) * (1 - pow(ddn, 2));

 // If angle is too shallow => total internal reflection.
  if (cos2t < 0)
    return em + f.mult(radiance(reflRay, depth,Xi));

  // Refraction ray !
  Vec3 tdir = (r.d_ * nnt - n * ((into ? 1 :-1) * (ddn * nnt + sqrt(cos2t)))).norm();

  // Fresnel term, percentage of light reflected.
  double a = nt - nc;
  double b = nt + nc;
  double R0 = pow(a, 2) / (pow(b, 2));
  double c = 1 - (into ? - ddn : tdir.dot(n));
  double Re = R0 + (1 - R0) * pow(c, 5);
  double Tr = 1 - Re;
  double P = .25 +.5 * Re;
  double RP = Re / P;
  double TP = Tr / (1 - P);
  return em + f.mult(depth > 2 ? (erand48(Xi) < P ? // Russian roulette
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
