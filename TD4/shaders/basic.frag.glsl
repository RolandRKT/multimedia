#version 150 core

// couleur émise pour le pixel
out vec4 frag_color;
in vec3 color;

void main(void)
{
  frag_color = (vec4( sin((gl_FragCoord.x)/5), sin((gl_FragCoord.x+10)/5), sin((gl_FragCoord.x+20)/5), 1.0 ) / 2) * vec4(color, 1.0);
}
