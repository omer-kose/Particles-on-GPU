#version 460 core

layout (location = 0) in vec3 pos_in;

out float lifetime;

uniform mat4 PV;

// Not defining binding 5 as it will not be used here. Is this a problem ?

layout(std140, binding=4) buffer PositionBuffer
{
	vec4 positions[];
};

layout(std140, binding=6) buffer LifetimeBuffer
{
	float lifetimes[];
};


void main()
{
	lifetime = lifetimes[gl_InstanceID];
	gl_Position = PV * vec4(pos_in + positions[gl_InstanceID].xyz, 1.0);
}
