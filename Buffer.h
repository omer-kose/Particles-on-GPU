#pragma once
#ifndef BUFFER_H
#define BUFFER_H

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

#endif