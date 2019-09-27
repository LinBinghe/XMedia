$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor0, 0);
SAMPLER2D(s_texColor1, 1);

void main()
{
   vec4 base = texture2D(s_texColor0, v_texcoord0);
   vec4 overlayer = texture2D(s_texColor1, v_texcoord0);
   gl_FragColor = overlayer * base + overlayer * (1.0 - base.a) + base * (1.0 - overlayer.a);
}
