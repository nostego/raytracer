#ifndef VEC3_HH_
# define VEC3_HH_

# include <iostream>

template <typename T>
struct Vec3
{
  Vec3()
    {
      x = 0;
      y = 0;
      z = 0;
    }
  Vec3(T x_, T y_, T z_) :
    x(x_),
    y(y_),
    z(z_)
    {
    }
  void print(std::ostream& out)
    {
      out << x << std::endl
          << y << std::endl
          << z << std::endl;
    }
  T operator*(Vec3& a)
    {
      return (x * a.x + y * a.y + z * a.z);
    }

  double length()
    {
      return sqrt(x*x+y*y+z*z);
    }
  void normalize()
    {
      T len = length();

      x /= len;
      y /= len;
      z /= len;
    }
  Vec3 operator* (double a)
    {
      Vec3 ret(x * a, y * a, z * a);
      return ret;
    }

  Vec3 operator- (Vec3& a)
    {
      Vec3 ret(x - a.x, y - a.y, z - a.z);
      return ret;
    }
  Vec3 operator+ (Vec3& a)
    {
      Vec3 ret(x + a.x, y + a.y, z + a.z);
      return ret;
    }

  T x;
  T y;
  T z;
};

#endif // !VEC3_HH_
