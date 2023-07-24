#version 460 core

// Constants
const vec3 G = vec3(0.0, -0.0, 0.0);

// Uniforms
uniform float dt;

// Process particles in blocks of 64
layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(std140, binding=4) buffer PositionBuffer
{
	vec4 positions[];
};

layout(std140, binding=5) buffer VelocityBuffer
{
	vec4 velocities[];
};

layout(std140, binding=6) buffer LifetimeBuffer
{
	float lifetimes[];
};

void main()
{
	uint index = gl_GlobalInvocationID.x;

	vec3 p = positions[index].xyz;
	vec3 v = velocities[index].xyz;
	float lifetime = lifetimes[index];

	// Most basic Newtonian Motion only with gravity
	positions[index].xyz = p + dt * v + 0.5 * dt * dt * G;
	velocities[index].xyz = v + dt * G;
	lifetimes[index] -= dt;
	// When this is 0 do smth
	if(lifetimes[index] <= 0.0f)
	{
		lifetimes[index] = 1.0f;
	}

}

