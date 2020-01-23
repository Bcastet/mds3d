#include "integrator.h"
#include "scene.h"

class FlatIntegrator : public Integrator {
public:
    FlatIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

       Hit hit= Hit();
       scene->intersect(ray, hit);

       //
       Color3f c;
       if(hit.t() == std::numeric_limits<float>::max()){
         c = scene->backgroundColor();
       }else{
         c = hit.shape()->material()->ambientColor();
       }
       return c;
        //return Color3f(0.f);
    }

    std::string toString() const {
        return "FlatIntegrator[]";
    }
};


REGISTER_CLASS(FlatIntegrator, "flat")
