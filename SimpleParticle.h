#pragma once
#ifndef SIMPLE_PARTICLE_H
#define SIMPLE_PARTICLE_H

#include <glm/glm.hpp>


#include "ParticleManager.h"
#include "Random.h"


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
	- All of the buffers must follow the std140 layout
	- All of the buffer must be initialized before passing to ParticleManager as ParticleManager has no idea about the underlying buffer
	- Buffers contain only one piece of data.
	- All of the buffers must implement the void* data() function
*/

class PositionBuffer : public Buffer
{
public:
	virtual void* data() const override
	{
		return (void*)pos.data();
	}

	virtual int size() const override
	{
		return  pos.size() * sizeof(glm::vec4);
	}

	virtual int length() const override
	{
		return pos.size();
	}

	void setData(const std::vector<glm::vec4>& data)
	{
		pos = data;
	}
private:
	std::vector<glm::vec4> pos;
};

class VelocityBuffer : public Buffer
{
public:
	virtual void* data() const override
	{
		return (void*)velocity.data();
	}

	virtual int size() const override
	{
		return  velocity.size() * sizeof(glm::vec4);
	}

	virtual int length() const override
	{
		return velocity.size();
	}

	void setData(const std::vector<glm::vec4>& data)
	{
		velocity = data;
	}
private:
	std::vector<glm::vec4> velocity;
};

class LifetimeBuffer : public Buffer
{
public:
	virtual void* data() const override
	{
		return (void*)lifetime.data();
	}
	 
	virtual int size() const override
	{
		return  lifetime.size() * sizeof(float);
	}

	virtual int length() const override
	{
		return lifetime.size();
	}

	void setData(const std::vector<float>& data)
	{
		lifetime = data;
	}
private:
	std::vector<float> lifetime;
};


class SimpleParticle
{
public:
	SimpleParticle();
	SimpleParticle(int numParticles);
	void render(const Camera& camera, double dt);
private:
	void m_initalizeBuffers();
	void m_prepareParticleInformation();
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