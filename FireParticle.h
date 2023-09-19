#pragma once
#ifndef FIRE_PARTICLE_H
#define FIRE_PARTICLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

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

struct FireParticleOptions
{
	glm::vec3 emit_center;
	float dt;
	float time;
	float fire_death_speed;
	float wind_strength;
	float wind_turbulence;
	float fire_triangleness;
	float speed;
	bool wind;
	bool omni_directional; // omni directional wind
};


class FireParticle
{
public:
	FireParticle();
	FireParticle(int numParticles);
	FireParticle(glm::vec3 emitCenter);
	FireParticle(glm::vec3 emitCenter, int numParticles);
	void render(const Camera& camera, double dt);
private:
	void m_initalizeBuffers();
	void m_prepareParticleInformation();
	void m_setParticleOptions();
	void m_setComputePassUniforms(double dt);
	void m_setDrawPassUniforms(const Camera& camera);
	void m_updateRandomNumbersBuffer();
	void m_loadTextures();
	void m_bindFireTexture(const std::string& textureName);
private:
	// The Particle Manager
	ParticleManager manager;
	// Buffers
	PositionBuffer m_posBuffer;
	VelocityBuffer m_velBuffer;
	ColorBuffer m_colBuffer;
	RandomFloatBuffer m_randomFloatBuffer;
	// Properties
	glm::vec3 m_emitCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	int m_numParticles = 8192;
	int m_localWorkGroupSize = 64;
	FireParticleOptions m_options;
	std::unordered_map<std::string, GLuint> m_textures;

};


#endif