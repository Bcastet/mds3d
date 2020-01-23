#include "integrator.h"
#include "scene.h"

class Normals : public Integrator {
public:
    Normals(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {


       Hit hit= Hit();
       scene->intersect(ray, hit);

       //
       if(hit.t() == std::numeric_limits<float>::max()){
         Color3f c = scene->backgroundColor();
         return c;
       }else{
         Color3f c(abs(hit.normal().x()),abs(hit.normal().y()),abs(hit.normal().z()));
         return c;
       }
        //return Color3f(0.f);
    }

    std::string toString() const {
        return "NormalIntegrator[]";
    }
};


REGISTER_CLASS(Normals, "normals")
