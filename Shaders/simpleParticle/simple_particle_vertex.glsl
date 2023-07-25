#version 460 core

layout (location = 0) in vec3 pos_in;

out float lifetime;

uniform mat4 PV;

// Not defining binding 5 as it will not be used here. Is this a problem ?

layout(std140, binding=4) buffer PositionBuffer
{
	vec4 positions[];
};

/*
	Using std430 for base type arrays like array of floats. std430 layout and C++ side base type strides are the same. 

	NOTE: If std140 is used, this will not work. Define a vec4 instead to match up with the padding that glsl compiler puts. But this will create an overhead.
*/
layout(std430, binding=6) buffer LifetimeBuffer
{
	float lifetimes[];
};


void main()
{
	lifetime = lifetimes[gl_InstanceID];
	gl_Position = PV * vec4(0.05f * pos_in + positions[gl_InstanceID].xyz, 1.0);
}
