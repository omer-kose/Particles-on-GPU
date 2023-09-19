#version 460 core

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec2 tex_in;


out vec4 color;
out vec2 tex_coords;

uniform mat4 PV;
uniform float time;


layout(std140, binding=4) buffer PositionBuffer
{
	vec4 positions[];
};

layout(std140, binding=5) buffer VelocityBuffer
{
	vec4 velocities[];
};

layout(std140, binding=6) buffer ColorBuffer
{
	vec4 colors[];
};

layout(std430, binding=7) buffer RandomFloatsBuffer
{
	float randomFloats[]; // [0, 1]
};



float hash11(float p)
{
    p = fract(p * 0.1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}


void main()
{
	tex_coords = tex_in;
    float size = 2.0f * hash11(gl_InstanceID);
	color = colors[gl_InstanceID];
	gl_Position = PV * vec4(0.08f * size * pos_in + positions[gl_InstanceID].xyz, 1.0);
}
