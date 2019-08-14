#version 330

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

// catre fragment shader
out vec3 color;
out vec3 world_pos2;
out vec3 light_position2;
out vec3 eye_position2;
out vec3 N2;

void main()
{
    vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
    
    vec3 N = normalize(mat3(Model) * v_normal);
    
	color = object_color;
    
    world_pos2 = world_pos;
    light_position2 = light_position;
    eye_position2 = eye_position;
    N2 = N;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
