#version 460 core

// Process particles in blocks of 64
layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

// Constants
const vec3 G = vec3(0.0, -0.0, 0.0);

// Uniforms
uniform float dt;


layout(std140, binding=4) buffer PositionBuffer
{
	vec4 positions[];
};

layout(std140, binding=5) buffer VelocityBuffer
{
	vec4 velocities[];
};

/*
	Using std430 for base type arrays like array of floats. std430 layout and C++ side base type strides are the same. 

	NOTE: If std140 is used, this will not work. Define a vec4 instead to match up with the padding that glsl compiler puts. But this will create an overhead.
*/
layout(std430, binding=6) buffer LifetimeBuffer
{
	float lifetimes[];
};


highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}


void main()
{
	uint index = gl_GlobalInvocationID.x;

	vec3 p = positions[index].xyz;
	vec3 v = velocities[index].xyz;
	float lifetime = lifetimes[index];

	// Most basic Newtonian Motion only with gravity
	positions[index].xyz = p + dt * v + 0.5 * dt * dt * G;
	velocities[index].xyz = v + dt * G;
	lifetime -= 0.2 * dt;
	// When this is 0 do smth
	if(lifetime <= 0.0f)
	{
	    vec3 s = positions[index].xyz;
		s  = -s + rand(s.xy)*20.0 - rand(s.yz)*20.0;
		//positions[index].xyz = s;
		lifetime = 1.0f;
	}

	lifetimes[index] = lifetime;

}

