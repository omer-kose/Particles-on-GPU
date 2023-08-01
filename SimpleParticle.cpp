#include "SimpleParticle.h"


SimpleParticle::SimpleParticle()
{
	m_initalizeBuffers();
	m_prepareParticleInformation();
}

SimpleParticle::SimpleParticle(int numParticles)
	:
	m_numParticles(numParticles)
{
	m_initalizeBuffers();
	m_prepareParticleInformation();
}

void SimpleParticle::render(const Camera& camera, double dt)
{
	m_setComputePassUniforms(dt);
	manager.computePass();
	m_setDrawPassUniforms(camera);
	manager.drawPass();
}

void SimpleParticle::m_initalizeBuffers()
{
	// Randomly initalize the buffers
	std::vector<glm::vec4> positions(m_numParticles);
	std::vector<glm::vec4> velocities(m_numParticles);
	std::vector<float> lifetimes(m_numParticles);

	for (int i = 0; i < m_numParticles; ++i)
	{
		positions[i] = Random::RandomOnSphere(5.0f);
		velocities[i] = glm::normalize(positions[i]);
		velocities[i].w = 1.0f;
		lifetimes[i] = 1.0f;
	}

	m_posBuffer.setData(positions);
	m_velBuffer.setData(velocities);
	m_lifetimeBuffer.setData(lifetimes);
}

void SimpleParticle::m_prepareParticleInformation()
{
	ParticleInformation info;
	// Set the buffers with corresponding binding locations in the compute shader
	// Using 4 5 6 for now because why not. Later when UBO's are included where we should bind SSBOs will be discussed.
	info.buffers[4] = &m_posBuffer;
	info.buffers[5] = &m_velBuffer;
	info.buffers[6] = &m_lifetimeBuffer;
	info.vertexShaderPath = "Shaders/simpleParticle/simple_particle_vertex.glsl";
	info.fragmentShaderPath= "Shaders/simpleParticle/simple_particle_fragment.glsl";
	info.computeShaderPath = "Shaders/simpleParticle/simple_particle_compute.glsl";
	info.numParticles = m_numParticles;
	// Set the particle shape VAO. 
	info.particleShapeVAO = Primitives::quad();
	// Initialize the manager
	manager = ParticleManager(info);
}

void SimpleParticle::m_setComputePassUniforms(double dt)
{
	ComputeShader shader = manager.getComputeShader();
	shader.use();
	shader.setFloat("dt", (float)dt);
}

void SimpleParticle::m_setDrawPassUniforms(const Camera& camera)
{
	Shader shader = manager.getGraphicsPipelineShaders();
	shader.use();
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 PV = projection * view;
	shader.setMat4("PV", PV);
}
