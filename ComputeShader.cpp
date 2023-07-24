#include "ComputeShader.h"


ComputeShader::ComputeShader()
{
}

ComputeShader::ComputeShader(const char* computePath)
{
	//Retrieve and store the shader codes
	std::string computeCode;
	std::ifstream computeShaderFile;
	//Ensure ifstream objects can throw exceptions. Set the exception masks
	computeShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//Open the files
		computeShaderFile.open(computePath);
		std::stringstream computeShaderStream;
		//Read file's buffer contents into the streams
		computeShaderStream << computeShaderFile.rdbuf();
		//Close the files
		computeShaderFile.close();
		//Extract the strings from the streams
		computeCode = computeShaderStream.str();
	}
	catch (std::ifstream::failure fail)
	{
		std::cout << "ERROR::ComputeShader::FILE_NOT_SUCCESSFULLY_READ->" << fail.what() << std::endl;
	}

	const char* cShaderCode = computeCode.c_str();
	//COMPILE THE SHADER
	GLuint compute;
	//Compute Shader Compilation
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);
	//Check errors
	checkCompileErrors(compute, "COMPUTE");
	//Create the shader program
	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);
	//Check linking errors
	checkCompileErrors(ID, "PROGRAM");

	//After linking the program we dont need shaders anymore.
	glDeleteShader(compute);
}

void ComputeShader::use()
{
	glUseProgram(ID);
}

void ComputeShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ComputeShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void ComputeShader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void ComputeShader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void ComputeShader::setMat3(const std::string& name, const glm::mat3& matrix) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ComputeShader::setMat4(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint ComputeShader::getID() const
{
	return ID;
}

void ComputeShader::checkCompileErrors(GLuint IDtoCheck, std::string type) const
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		//Check shader errors
		glGetShaderiv(IDtoCheck, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(IDtoCheck, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		//Check program errors
		glGetProgramiv(IDtoCheck, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(IDtoCheck, 1024, NULL, infoLog);
			std::cout << "ERROR::COMPUTE_PROGRAM_LINKING_ERROR of type " << type << "\n" << infoLog << std::endl;
		}

	}
}