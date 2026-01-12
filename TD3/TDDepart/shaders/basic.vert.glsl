#version 150 core
// version du langage GLSL utilisée, ici 1.5

uniform mat4 mvp;

in vec3 in_pos;
in vec3 in_col;
in vec2 in_coord;

out vec2 texcoord;
out vec3 colors;

void main(void)
{
  gl_Position = mvp * vec4( in_pos, 1.0 );
  //texcoord = (mvp * vec4( in_coord, 0.0, 1.0 )).st;
  texcoord =  in_coord;
  colors = in_col;
}
