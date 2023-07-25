#version 460 core

out vec4 fs_out;

in float lifetime;

void main()
{
	float color = lifetime;
	vec3 black = vec3(0.0f);
	if (color < 0.1) 
	{
		fs_out = mix(vec4(black,1.0), vec4(0.9,0.05,0.0,1.0), color*10.0);  
	} 
	else if (color > 0.9) 
	{
		fs_out = mix(vec4(0.0,0.5,1.0,1.0), vec4(black,1.0), (color-0.9)*10.0);  
	} 
	else 
	{
		fs_out = mix(vec4(0.9,0.05,0.0,1.0), vec4(0.0,0.5,1.0,1.0), 1.11f * (color - 0.1));
	}

	fs_out.xyz = pow(fs_out.xyz, vec3(1.0f / 2.2f));
	
}
