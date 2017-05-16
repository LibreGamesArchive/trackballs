#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 proj_matrix;
uniform mat4 model_matrix;

uniform float gameTime;

attribute vec3 in_Position;
attribute vec4 in_Color;
attribute vec2 in_Texcoord;
attribute vec2 in_Velocity;

varying vec4 fcolor;
varying vec2 texco;
varying vec3 cpos;

void main(void) {
  vec4 pos = vec4(in_Position.x, in_Position.y, in_Position.z, 1.);
  mat4 mvp_matrix = proj_matrix * model_matrix;
  gl_Position = mvp_matrix * pos;
  cpos = vec4(model_matrix * pos).xyz;
  fcolor = in_Color;
  vec2 vel = mod(-gameTime * 4. * in_Velocity, 1.);
  texco = in_Texcoord + 0.125 * vel;
}
