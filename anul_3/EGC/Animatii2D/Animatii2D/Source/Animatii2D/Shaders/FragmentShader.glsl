#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
    out_color = texture2D(texture_1, texcoord);

	if(out_color.a < 0.1f) 
		discard;
}


