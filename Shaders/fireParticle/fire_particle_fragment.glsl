#version 460 core

out vec4 fs_out;

in vec4 color;
in vec2 tex_coords;

uniform sampler2D fire_texture;

void main()
{
	vec4 tex_color = texture2D(fire_texture, tex_coords);
	
	fs_out = color * tex_color;
	fs_out.xyz = pow(fs_out.xyz, vec3(1.0f / 2.2f));
}
