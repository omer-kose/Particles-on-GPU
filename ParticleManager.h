#pragma once
#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H


#include <unordered_map>

#include "Buffer.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Camera.h"


struct ParticleInformation
{
	std::unordered_map<int, Buffer*> buffers;
	const char* vertexShaderPath;
	const char* fragmentShaderPath;
	const char* computeShaderPath;
	int numParticles;
	GLuint particleShapeVAO;
};


class ParticleManager
{
public:
	ParticleManager();
	ParticleManager(const ParticleInformation& info);
	void computePass();
	void drawPass();
	ComputeShader getComputeShader();
	Shader getGraphicsPipelineShaders();
private:
	void m_prepareSSBOs();
private:
	std::unordered_map<int, Buffer*> m_buffers;
	std::unordered_map<int, GLuint> m_ssbos;
	Shader m_graphicsPipelineShaders;
	ComputeShader m_computeShader;
	int m_numParticles;
	int m_localWorkGroupSize = 64; // Constant 
	GLuint m_particleShapeVAO;
};


#endif