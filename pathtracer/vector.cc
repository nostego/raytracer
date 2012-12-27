#include "vector.hh"

Vec3::Vec3(double x, double y, double z)
  : x_(x),
    y_(y),
    z_(z)
{}

Vec3 Vec3::operator+(const Vec3 &b) const
{
  return Vec3(x_ + b.x_, y_ + b.y_, z_ + b.z_);
}

Vec3 Vec3::operator-(const Vec3 &b) const
{
  return Vec3(x_ - b.x_, y_ - b.y_, z_ - b.z_);
}

Vec3 Vec3::operator*(double b) const
{
  return Vec3(x_ * b, y_ * b, z_ * b);
}

Vec3 Vec3::mult(const Vec3 &b) const
{
  return Vec3(x_ * b.x_, y_ * b.y_, z_ * b.z_);
}

Vec3& Vec3::norm()
{
  return *this =
    *this * (1 / sqrt(x_ * x_ + y_ * y_ + z_ * z_));
}

double Vec3::dot(const Vec3 &b) const
{
  return x_ * b.x_ + y_ * b.y_ + z_ * b.z_;
}

// Cross product.
Vec3 Vec3::operator%(Vec3 &b) const
{
  return Vec3(y_ * b.z_ - z_ * b.y_,
              z_ * b.x_ - x_ * b.z_, x_ * b.y_ - y_ * b.x_);
}
