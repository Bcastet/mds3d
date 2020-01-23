#include "sphere.h"
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius",1.f);
    m_center = propList.getPoint("center",Point3f(0,0,0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray& ray, Hit& hit) const
{
    /// TODO: compute ray-sphere intersection
    Vector3f d = ray.direction ;

    Vector3f size = ray.origin - this->m_center;
    float s = size.norm();

    float r = this->m_radius ;

    float a = d.dot(d);
    float b = 2*d.dot(size);
    float c = s*s - r*r;


    //float discriminant =  (2*d * s*(2*d * s) - 4*( d*d * (s*s - r)));

    float discriminant = b*b - 4*a*c ;


    if(discriminant > 0){
      float t1 = (-b - sqrt(discriminant)) / (2*a) ;
      float t2 = (-b + sqrt(discriminant)) / (2*a)  ;
      if ( t1 > t2 && t2 > 0){
        hit.setT(t2);
        hit.setShape(this);
        hit.setNormal(( ray.at(t2) - m_center).normalized());



      }else if(t1 > 0) {
        hit.setT(t1);
        hit.setShape(this);
        hit.setNormal( (ray.at(t1) - m_center).normalized());
      }else{
        return false;
      }
      return true;
    }else if(discriminant == 0){
      hit.setT(-b / (2*a)) ;
      hit.setShape(this);
      hit.setNormal(( ray.at(-b / (2*a)) - m_center).normalized());
      return true;
    }else{
      return false;
    }


    throw RTException("Sphere::intersect not implemented yet.");

    return false;
}

REGISTER_CLASS(Sphere, "sphere")
