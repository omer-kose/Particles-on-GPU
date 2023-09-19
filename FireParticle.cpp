#include "FireParticle.h"



std::vector<std::string> textureNames =
{
	"circle.png",
	"explosion.png",
	"flame.png",
	"gradient.png",
	"rectangle.png",
	"thicker_gradient.png",
	"noise.png"
};


//stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Texture Loader
GLuint textureFromFile(const char* filePath, bool verticalFlip)
{
	//Generate the texture
	GLuint textureId;
	glGenTextures(1, &textureId);

	//Load the data from the file
	stbi_set_flip_vertically_on_load(verticalFlip);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		//Bind and send the data
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Configure params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

	}
	else
	{
		std::cout << "Texture failed to load on path: " << filePath << std::endl;
		stbi_image_free(data);
	}

	return textureId;
}


FireParticle::FireParticle()
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
	m_loadTextures();
}

FireParticle::FireParticle(int numParticles)
	:
	m_numParticles(numParticles)
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
	m_loadTextures();

}

FireParticle::FireParticle(glm::vec3 emitCenter)
	:
	m_emitCenter(emitCenter)
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
	m_loadTextures();

}

FireParticle::FireParticle(glm::vec3 emitCenter, int numParticles)
	:
	m_emitCenter(emitCenter),
	m_numParticles(numParticles)
{
	m_setParticleOptions();
	m_initalizeBuffers();
	m_prepareParticleInformation();
	m_loadTextures();
}

void FireParticle::render(const Camera& camera, double dt)
{
	m_updateRandomNumbersBuffer();
	m_setComputePassUniforms(dt);
	manager.computePass();
	m_setDrawPassUniforms(camera);
	m_bindFireTexture("flame.png");
	manager.drawPass();
}

void FireParticle::m_initalizeBuffers()
{
	float pi = 3.14159265359;
	// Randomly initalize the buffers
	std::vector<glm::vec4> positions(m_numParticles);
	std::vector<glm::vec4> velocities(m_numParticles);
	std::vector<glm::vec4> colors(m_numParticles);
	std::vector<float> randomFloats(m_numParticles);

	// For now these are constants
	glm::vec2 emitPositionSpread = glm::vec2(1.0f, 0.05f);
	float emitAngleVariance = 0.42; // in radians
	glm::vec3 fireColor = glm::vec3(1.0f, 0.41f, 0.0f);

	for (int i = 0; i < m_numParticles; ++i)
	{
		positions[i] = glm::vec4(Random::RandomSpreadVec2(glm::vec2(m_emitCenter), emitPositionSpread), m_emitCenter.z, 1.0f);
		velocities[i] = glm::vec4(m_options.speed * Random::RandomUnitVec2OnCircle(pi / 2.0f, emitAngleVariance), 0.0f, 1.0f);
		colors[i] = glm::vec4(fireColor, Random::RandomFloat(-0.5f, 1.0f));
		randomFloats[i] = Random::RandomFloat(0.0f, 1.0f);
	}

	m_posBuffer.setData(positions);
	m_velBuffer.setData(velocities);
	m_colBuffer.setData(colors);
	m_randomFloatBuffer.setData(randomFloats);
}

void FireParticle::m_prepareParticleInformation()
{
	ParticleInformation info;
	// Set the buffers with corresponding binding locations in the compute shader
	// Using 4 5 6 for now because why not. Later when UBO's are included where we should bind SSBOs will be discussed.
	info.buffers[4] = &m_posBuffer;
	info.buffers[5] = &m_velBuffer;
	info.buffers[6] = &m_colBuffer;
	info.buffers[7] = &m_randomFloatBuffer;
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
	m_options.fire_death_speed = 2.0f;
	m_options.wind = false;
	m_options.omni_directional = false;
	m_options.wind_strength = 400.0f;
	m_options.wind_turbulence = 0.003;
	m_options.fire_triangleness = 3.0f;
	m_options.speed = 400.0f;
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
	shader.setFloat("time", glfwGetTime());
}

void FireParticle::m_updateRandomNumbersBuffer()
{
	std::vector<float> randomFloats(m_numParticles);
	GLuint ssboID = manager.getSSBO(7);
	// Update the floats
	for (int i = 0; i < m_numParticles; ++i)
	{
		randomFloats[i] = Random::RandomFloat(0.0f, 1.0f);
	}
	m_randomFloatBuffer.setData(randomFloats);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
	// Yes, this is not a good way
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_randomFloatBuffer.size(), m_randomFloatBuffer.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void FireParticle::m_loadTextures()
{
	std::string path;
	for (const std::string& textureName : textureNames)
	{
		path = "Textures/" + textureName;
		m_textures[textureName] = textureFromFile(path.c_str(), false);
	}
}

void FireParticle::m_bindFireTexture(const std::string& textureName)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[textureName]);
	// Also, set the sampler 
	Shader shader = manager.getGraphicsPipelineShaders();
	shader.use();
	// Set the texture sampler
	shader.setInt("fire_texture", 0);
}
