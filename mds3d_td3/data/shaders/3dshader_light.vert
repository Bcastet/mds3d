#version 330 core

in vec3 vtx_position;
in vec3 vtx_normal;
in vec3 vtx_color;

out vec3 var_color;
out vec3 var_normal;
out vec3 var_v;
out vec3 var_light;
out vec3 var_sCol;
out float var_exposant;

uniform mat4 mat_obj;
uniform mat4 mat_view;
uniform mat4 mat_perspective;

uniform mat3 normal_mat;

uniform float exposant;

uniform vec3 light_dir;
uniform vec3 sCol;



void main()
{
  var_color = vtx_color;
  var_normal = normalize(normal_mat * vtx_normal);

  vec3 v = vec3(vtx_position.x, vtx_position.y ,vtx_position.z);
  var_v = - v / length(v);

  vec4 var_light_temp = vec4(light_dir,0)*mat_obj;
  var_light = normalize(vec3(var_light_temp.x,var_light_temp.y,var_light_temp.z));

  var_sCol = sCol;
  var_exposant = exposant;

  gl_Position = mat_perspective * mat_view * mat_obj * vec4(vtx_position, 1.);

}