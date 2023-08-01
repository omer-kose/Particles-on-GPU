#version 460 core

out vec4 fs_out;

in vec4 color;

void main()
{
	// Will sample and mix with the color later.
	fs_out = color;
}
