#include "FireParticle.h"


FireParticle::FireParticle()
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
}

FireParticle::FireParticle(int numParticles)
	:
	m_numParticles(numParticles)
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
}

FireParticle::FireParticle(glm::vec3 emitCenter)
	:
	m_emitCenter(emitCenter)
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
}

FireParticle::FireParticle(glm::vec3 emitCenter, int numParticles)
	:
	m_emitCenter(emitCenter),
	m_numParticles(numParticles)
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
}

void FireParticle::render(const Camera& camera, double dt)
{
	m_setComputePassUniforms(dt);
	manager.computePass();
	m_setDrawPassUniforms(camera);
	manager.drawPass();
}

void FireParticle::m_initalizeBuffers()
{
	float pi = 3.14159265359;
	// Randomly initalize the buffers
	std::vector<glm::vec4> positions(m_numParticles);
	std::vector<glm::vec4> velocities(m_numParticles);
	std::vector<glm::vec4> colors(m_numParticles);
	std::vector<float> lifetimes(m_numParticles);

	// For now these are constants
	glm::vec2 emitPositionSpread = glm::vec2(1.0f, 0.05f);
	float emitAngleVariance = 0.42; // in radians
	glm::vec3 fireColor = glm::vec3(1.0f, 0.41f, 0.0f);

	for (int i = 0; i < m_numParticles; ++i)
	{
		positions[i] = glm::vec4(Random::RandomSpreadVec2(m_emitCenter, emitPositionSpread), 0.0f, 1.0f);
		velocities[i] = glm::vec4(m_options.speed * Random::RandomUnitVec2OnCircle(pi / 2.0f, emitAngleVariance), 0.0f, 1.0f);
		colors[i] = glm::vec4(fireColor, Random::RandomSpread(0.5f, 0.5f));
		lifetimes[i] = Random::RandomSpread(1.0f, 0.4f);
	}

	m_posBuffer.setData(positions);
	m_velBuffer.setData(velocities);
	m_colBuffer.setData(colors);
	m_lifetimeBuffer.setData(lifetimes);
}

void FireParticle::m_prepareParticleInformation()
{
	ParticleInformation info;
	// Set the buffers with corresponding binding locations in the compute shader
	// Using 4 5 6 for now because why not. Later when UBO's are included where we should bind SSBOs will be discussed.
	info.buffers[4] = &m_posBuffer;
	info.buffers[5] = &m_velBuffer;
	info.buffers[6] = &m_colBuffer;
	info.buffers[7] = &m_lifetimeBuffer;
	info.vertexShaderPath = "Shaders/fireParticle/fire_particle_vertex.glsl";
	info.fragmentShaderPath = "Shaders/fireParticle/fire_particle_fragment.glsl";
	info.computeShaderPath = "Shaders/fireParticle/fire_particle_compute.glsl";
	info.numParticles = m_numParticles;
	// Set the particle shape VAO. 
	info.particleShapeVAO = Primitives::quad();
	// Initialize the manager
	manager = ParticleManager(info);
}

void FireParticle::m_setParticleOptions()
{
	m_options.fire_death_speed = 0.125f;
	m_options.wind = false;
	m_options.omni_directional = false;
	m_options.wind_strength = 20.0f;
	m_options.wind_turbulence = 0.0003;
	m_options.fire_triangleness = 0.00015;
	m_options.speed = 30.0f;
}

void FireParticle::m_setComputePassUniforms(double dt)
{
	ComputeShader shader = manager.getComputeShader();
	shader.use();
	shader.setFloat("dt", (float)dt);
	shader.setFloat("time", glfwGetTime());
	shader.setFloat("fire_death_speed", m_options.fire_death_speed);
	shader.setBool("wind", m_options.wind);
	shader.setBool("omni_directional", m_options.omni_directional);
	shader.setFloat("wind_strength", m_options.wind_strength);
	shader.setFloat("wind_turbulence", m_options.wind_turbulence);
	shader.setFloat("fire_triangleness", m_options.fire_triangleness);
	shader.setVec3("emit_center", m_emitCenter);
	shader.setFloat("speed", m_options.speed);
}

void FireParticle::m_setDrawPassUniforms(const Camera& camera)
{
	Shader shader = manager.getGraphicsPipelineShaders();
	shader.use();
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 PV = projection * view;
	shader.setMat4("PV", PV);
}
