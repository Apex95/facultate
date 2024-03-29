#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
uniform int ok;

in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec4 color1 = vec4(1);
	vec4 color2 = vec4(1);
	
	color1 = texture2D(texture_1, texcoord);

	if(ok == 1)
	{
		color2 = texture2D(texture_2, texcoord);
		out_color = mix(color1,color2,0.5f);
	} 
	else 
	{
		out_color = color1;
	}

	if(out_color.a < 0.5f) 
		discard;
}


