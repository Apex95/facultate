#version 330

in vec3 color;


uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

in vec3 world_pos2;
in vec3 eye_position2;
in vec3 light_position2;

in vec3 N2;

in float dropThis;

vec3 N3;

float k = 0.4;

layout(location = 0) out vec4 out_color;

void main()
{
    if (dropThis > 0.9)
        discard;

    N3 = normalize(N2);

    vec3 L2 = normalize(light_position2 - world_pos2);
    vec3 V2 = normalize(eye_position2 - world_pos2);

    float ambient_light = 0.5;

    float diffuse_light = material_kd * max(dot(N3, L2), 0);
    
    float specular_light = 0;
    if (diffuse_light > 0)
	{
        vec3 H2 = normalize(L2 + V2);
        specular_light = material_ks * pow(max(dot(N3, H2), 0), material_shininess);
    }


    float f = 1;1 / (pow(k * distance(world_pos2, light_position2), 2) + 1);


    out_color = vec4(color * (ambient_light + f * (diffuse_light + specular_light)), 1);
}