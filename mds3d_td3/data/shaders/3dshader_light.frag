#version 330 core

in vec3 var_color;
in vec3 var_normal;
in vec3 var_v;
in vec3 var_light;
in vec3 var_sCol;
in float var_exposant;



out vec4 out_color;



vec3 blinn(vec3 n, vec3 v, vec3 l, vec3 dCol, vec3 sCol, float s) {

  vec3 diffus = max(dot(n,l),0) * dCol ;

  vec3 h = normalize(v + l);
  vec3 speculaire = pow(max(dot(n,h),0),s)* sCol;

  vec3 ambiant = vec3(0.1,0.1,0.1);

  vec3 res = diffus + speculaire + ambiant;

  return res;
}

void main(void) {

    vec3 blinn_res = blinn(var_normal,var_v,var_light,var_color,var_sCol,var_exposant);
    out_color = vec4(blinn_res,1);
}