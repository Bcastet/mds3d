#include "integrator.h"
#include "scene.h"
#include <algorithm>

class Direct : public Integrator {
public:
    Direct(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {


       Hit hit= Hit();
       scene->intersect(ray, hit);
       if(hit.t() == std::numeric_limits<float>::max()){
         return scene->backgroundColor();
       }else{
         Color3f c(0,0,0);
         for(Light* light : scene->lightList()){
           c += hit.shape()->material()->brdf( ray.direction , -light->direction(ray.at(hit.t())).normalized(), hit.normal()   ) 
           * std::max( light->direction(ray.at(hit.t())).normalized().dot(hit.normal()), 0.f   ) * light->intensity(ray.at(hit.t()));
         }
         return c;
       }
       
    
    }

    std::string toString() const {
        return "DirectIntegrator[]";
    }
};


REGISTER_CLASS(Direct, "direct")
