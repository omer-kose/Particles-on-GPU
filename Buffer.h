#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include <glm/glm.hpp>

// This Buffer struct is an empty struct that will be extended by other buffer types. This will be used for Polymorphism
class Buffer 
{
public:
	virtual ~Buffer() {}

    virtual void* data() const
    {
        return nullptr;
    }

    // Size in bytes!
    virtual int size() const
    {
        return 0;
    }

    // Number of elements
    virtual int length() const
    {
        return 0;
    }
}; 

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


class ColorBuffer : public Buffer
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


/*
	Contains random numbers in range [0-1]. Will be used for better GPU Randomness
*/
class RandomFloatBuffer : public Buffer
{
public:
	virtual void* data() const override
	{
		return (void*)randomFloat.data();
	}

	virtual int size() const override
	{
		return  randomFloat.size() * sizeof(float);
	}

	virtual int length() const override
	{
		return randomFloat.size();
	}

	void setData(const std::vector<float>& data)
	{
		randomFloat = data;
	}
private:
	std::vector<float> randomFloat;
};


#endif