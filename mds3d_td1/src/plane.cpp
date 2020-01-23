#include "plane.h"

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position",Point3f(0,0,0));
    m_normal = propList.getVector("normal",Point3f(0,0,1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    /// TODO
    Vector3f d = ray.direction ;
    Point3f o = ray.origin ;

    Vector3f n = this->m_normal ;
    Point3f a = this->m_position ;

    float res = ((a - o).dot(n)) / (d.dot(n)) ;

    if(res > 0 ){
      hit.setT(res);
      hit.setShape(this);
      hit.setNormal( m_normal );
      return true ;
    }else if(res < 0){
      return false ;
    }

    return false;
}

REGISTER_CLASS(Plane, "plane")
