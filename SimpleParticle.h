#pragma once
#ifndef SIMPLE_PARTICLE_H
#define SIMPLE_PARTICLE_H

#include <glm/glm.hpp>


#include "ParticleManager.h"
#include "Random.h"
#include "Primitives.h"


/*
	A simple particle system that uses the ParticleManager.

	Each new particle system must provide the following:
	- Buffers that extends base Buffer struct. The binding locations of those buffers must be provided and match the binding points defined in the compute shader
	- Shaders (vertex, fragment and compute) (IMPORTANT: The buffer binding locations must match with the buffers provided above)
	- Particle Shape VAO
	During rendering:
	- Call manager.computePass()
	- Call manager.drawPass()

	Notes:
	- All of the buffers must follow the std140 layout (For now for base type arrays like vector<float> I use std430 layout to get rid of padding overhead)
	- All of the buffer must be initialized before passing to ParticleManager as ParticleManager has no idea about the underlying buffer
	- Buffers contain only one piece of data.
	- All of the buffers must implement:
		- void* data() function which returns a void* to the beginning of the data
		- int size() function which is size of the data in bytes
		- int length() function which is the number of elements in the data
*/


class SimpleParticle
{
public:
	SimpleParticle();
	SimpleParticle(int numParticles);
	void render(const Camera& camera, double dt);
private:
	void m_initalizeBuffers();
	void m_prepareParticleInformation();
	void m_setComputePassUniforms(double dt);
	void m_setDrawPassUniforms(const Camera& camera);
private:
	// The Particle Manager
	ParticleManager manager;
	// Buffers
	PositionBuffer m_posBuffer;
	VelocityBuffer m_velBuffer;
	LifetimeBuffer m_lifetimeBuffer;
	// Properties
	int m_numParticles = 8192;
	int m_localWorkGroupSize = 64;
};


#endif