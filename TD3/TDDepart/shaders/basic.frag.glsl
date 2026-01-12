
#version 150 core

in vec2 texcoord;
in vec3 colors;

out vec4 frag_color;

uniform sampler2D tex;

uniform sampler2D tex2;
void main(void)
{
  //frag_color = texture( tex2, texcoord );//+ vec4( 0.1, 0.1, 0.1, 0.1 );
  frag_color =  vec4( colors, 0.5 );
  //frag_color =  mix(vec4( colors, 1.0 ), texture( tex2, texcoord ),0.8) ;
  
  /*if(texture(tex, texcoord).x==1.0) frag_color=vec4( colors, 0.5 );
  else  frag_color = mix(texture(tex, texcoord), texture(tex2, texcoord), 0.8);*/
  
 /* if(texture(tex, texcoord).x==0.0)  frag_color =texture(tex, texcoord);
  else  frag_color = mix(texture(tex, texcoord), texture(tex2, texcoord), 0.8);
  */
  frag_color = texture( tex2, texcoord );
  //frag_color = mix(texture(tex, texcoord), texture(tex2, texcoord), 0.8);
  
  //   frag_color =  vec4( 0.7, 0.1, 0.1, 0.1 );
}
