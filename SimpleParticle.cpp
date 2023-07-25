#include "SimpleParticle.h"


// Particle Shape will be passed from outside.
GLuint quad()
{
	GLfloat vertices[] =
	{
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	GLuint indices[] =
	{
		2, 1, 0,  // first Triangle
		2, 0, 3   // second Triangle
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind VAO
	glBindVertexArray(VAO);
	//Bind VBO, send data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Bind EBO, send indices 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Configure Vertex Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	//Data passing and configuration is done 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}


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
	manager.computePass(dt);
	manager.drawPass(camera);
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
	info.particleShapeVAO = quad();
	// Initialize the manager
	manager = ParticleManager(info);
}
