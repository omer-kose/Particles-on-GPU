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

void ParticleManager::computePass(double dt)
{
	m_computeShader.use();
	// Set uniforms
	m_computeShader.setFloat("dt", (float)dt);
	// Bind the SSBOs to the correct places given in the hash map
	for (const auto& it : m_ssbos)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, it.first, it.second);
	}

	glDispatchCompute(m_numParticles / m_localWorkGroupSize, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}

void ParticleManager::drawPass(const Camera& camera)
{
	m_graphicsPipelineShaders.use();
	// Set uniforms
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 PV = projection * view;
	m_graphicsPipelineShaders.setMat4("PV", PV);
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
