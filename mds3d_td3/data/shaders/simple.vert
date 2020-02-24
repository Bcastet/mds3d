#version 330 core

uniform float zoom;

uniform mat4 mat_obj;
uniform mat4 mat_view;
uniform mat4 mat_perspective;

in vec3 vtx_position;

in vec3 vtx_color;
out vec3 var_color;


void main()
{
  var_color = vtx_color;
  gl_Position = mat_obj*vec4(vtx_position, 1.);
  
}
