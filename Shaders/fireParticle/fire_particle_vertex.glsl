#version 460 core

layout (location = 0) in vec3 pos_in;

out vec4 color;

uniform mat4 PV;


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


float hash11(float p)
{
    p = fract(p * 0.1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}


void main()
{
    float size = hash11(gl_InstanceID);
	color = colors[gl_InstanceID];
	gl_Position = PV * vec4(0.05f * size * pos_in + positions[gl_InstanceID].xyz, 1.0);
}
