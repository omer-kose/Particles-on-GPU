#version 460 core

out vec4 fs_out;

in float lifetime;

layout(std140, binding=6) buffer LifetimeBuffer
{
	float lifetimes[];
};

void main()
{
	float color = lifetime;
	if (color < 0.1) 
	{
		fs_out = mix(vec4(vec3(0.0),1.0), vec4(0.0,0.5,1.0,1.0), color*10.0);  
	} 
	else if (color > 0.9) 
	{
		fs_out = mix(vec4(0.6,0.05,0.0,1.0), vec4(vec3(0.0),1.0), (color-0.9)*10.0);  
	} 
	else 
	{
		fs_out = mix(vec4(0.0,0.5,1.0,1.0), vec4(0.6,0.05,0.0,1.0), color);
	}

}
