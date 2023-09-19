#include "ParticleManager.h"



ParticleManager::ParticleManager()
{
}

ParticleManager::ParticleManager(const ParticleInformation& info)
	:
	m_buffers(info.buffers),
	m_graphicsPipelineShaders(info.vertexShaderPath, info.fragmentShaderPath),
	m_computeShader(info.computeShaderPath),
	m_numParticles(info.numParticles),
	m_particleShapeVAO(info.particleShapeVAO)
{
	m_prepareSSBOs();
}

void ParticleManager::computePass()
{
	m_computeShader.use();
	// Bind the SSBOs to the correct places given in the hash map
	for (const auto& it : m_ssbos)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, it.first, it.second);
	}

	glDispatchCompute(m_numParticles / m_localWorkGroupSize, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}

void ParticleManager::drawPass()
{
	m_graphicsPipelineShaders.use();
	// Bind the SSBOs to the correct places given in the hash map
	// Note that not all of the SSBOs are needed in the draw pass (like velocity). For now, I will still define them inside the shaders not sure whether this has a performance impact
	for (const auto& it : m_ssbos)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, it.first, it.second);
	}

	glBindVertexArray(m_particleShapeVAO);
	//total 6 indices since we have triangles
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_numParticles);
	
}

ComputeShader ParticleManager::getComputeShader() 
{
	return m_computeShader;
}

Shader ParticleManager::getGraphicsPipelineShaders() 
{
	return m_graphicsPipelineShaders;
}

GLuint ParticleManager::getSSBO(int binding)
{
	return m_ssbos[binding];
}

void ParticleManager::m_prepareSSBOs()
{
	// For each buffer create the corresponding SSBO
	for (const auto& it : m_buffers)
	{
		const Buffer* buffer = it.second;
		glGenBuffers(1, &m_ssbos[it.first]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos[it.first]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, buffer->size(), buffer->data(), GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}
