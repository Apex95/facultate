#version 330

#define M_PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// uniforme pt lumina
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

// uniforme pt apa
uniform float A;
uniform vec2 C;
uniform float w;
uniform float phi;
uniform float t;

vec2 D;

vec3 world_pos;

uniform int isRenderingWater;

// catre fragment shader
out vec3 color;
out vec3 world_pos2;
out vec3 light_position2;
out vec3 eye_position2;
out vec3 N2;

float k = t/12 + 0.5;

out float dropThis;

float computeY(float x, float z)
{
    //return A * (sin(dot(D, vec2(x, z)) * w + t * phi)+1);
    if (pow(length(world_pos.xz), k) < 0.15)
        return 0;

    return A / pow(length(world_pos.xz), k) * (sin(dot(D, vec2(x, z)) * w + t * phi)+1);
}

float computeYdx(float x, float z)
{
    //return A * D.x * w * cos(dot(D, vec2(x, z)) * w + t * phi);

    if (pow(length(world_pos.xz), k) <0.15)
        return 0;

    return A / pow(length(world_pos.xz), k) * D.x * w * cos(dot(D, vec2(x, z)) * w + t * phi) - A * x * pow(x*x + z*z, -k/2.0-1) * (sin(dot(D, vec2(x, z)) * w + t * phi)+1);
}


float computeYdz(float x, float z)
{
    //return A * D.y * w * cos(dot(D, vec2(x, z)) * w + t * phi);

    if (pow(length(world_pos.xz), k) < 0.15)
        return 0;

    return A / pow(length(world_pos.xz), k) * D.y * w * cos(dot(D, vec2(x, z)) * w + t * phi) - A * z * pow(x*x+z*z, -k/2.0-1) * (sin(dot(D, vec2(x, z)) * w + t * phi)+1);
}


void main()
{
    dropThis = 0;
    world_pos = (Model * vec4(v_position, 1)).xyz;

    vec3 N = normalize(mat3(Model) * v_normal);

    if (isRenderingWater == 1)
    {
        if (length(C - world_pos.xz) < 0.01)
            D = vec2(C.x, C.y);
        else
        {
            vec2 _D = C - world_pos.xz;
            D = normalize(_D);
        }

        world_pos.x -= C.x;
        world_pos.z -= C.y;

        float L = 2 * M_PI / w;
        float S = phi * L / (2 * M_PI);



        if (length(world_pos.xz) < t * S + L/4 && length(world_pos.xz) > t * S - L/2 - L/4)
        {
            world_pos.y = computeY(world_pos.x, world_pos.z);
            float dx = computeYdx(world_pos.x, world_pos.z);
            float dz = computeYdz(world_pos.x, world_pos.z);

            N = normalize(vec3(-dx, 1, -dz));
        }
        else
            dropThis = 1;


        world_pos.x += C.x;
        world_pos.z += C.y;

    }
	
    color = object_color;
    
    world_pos2 = world_pos;
    light_position2 = light_position;
    eye_position2 = eye_position;
    N2 = N;


	gl_Position = Projection * View * vec4(world_pos, 1.0);   //Model * vec4(v_position, 1.0);
}
