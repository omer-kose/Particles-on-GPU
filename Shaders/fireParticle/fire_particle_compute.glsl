#version 460 core

// Process particles in blocks of 64
layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

// Constants
const float pi = 3.14159265359;
// These constants could also be uniforms
const vec2 emit_position_spread = vec2(1.0f, 0.05f); 
const float emit_angle_variance = 0.42; // In radians


// Uniforms (would be better if I put them into a buffer but meh)
uniform float dt;
uniform float time;
uniform float fire_death_speed;
uniform bool wind;
uniform bool omni_directional; // omni directional wind
uniform float wind_strength;
uniform float wind_turbulence;
uniform float fire_triangleness;
uniform vec3 emit_center;
uniform float speed;


layout(std140, binding=4) buffer PositionBuffer
{
	vec4 positions[];
};

layout(std140, binding=5) buffer VelocityBuffer
{
	vec4 velocities[];
};

layout(std140, binding=6) buffer ColorBuffer
{
	vec4 colors[];
};

/*
	Using std430 for base type arrays like array of floats. std430 layout and C++ side base type strides are the same. 

	NOTE: If std140 is used, this will not work. Define a vec4 instead to match up with the padding that glsl compiler puts. But this will create an overhead.
*/
layout(std430, binding=7) buffer LifetimeBuffer
{
	float lifetimes[]; // [0, 1]
};


// RANDOMNESS 
//note: uniformly distributed, normalized rand, [0;1]
float nrand( vec2 n )
{
	return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}
// Uniform Distribution
float n1rand( vec2 n )
{
	float t = fract( time );
	float nrnd0 = nrand( n + 0.07*t );
	return nrnd0;
}
// Triangular Distribution
float n2rand( vec2 n )
{
	float t = fract( time );
	float nrnd0 = nrand( n + 0.07*t );
	float nrnd1 = nrand( n + 0.11*t );
	return (nrnd0+nrnd1) / 2.0;
}
float n3rand( vec2 n )
{
	float t = fract( time );
	float nrnd0 = nrand( n + 0.07*t );
	float nrnd1 = nrand( n + 0.11*t );
	float nrnd2 = nrand( n + 0.13*t );
	return (nrnd0+nrnd1+nrnd2) / 3.0;
}
// Gaussianish Distribution
float n4rand( vec2 n )
{
	float t = fract( time );
	float nrnd0 = nrand( n + 0.07*t );
	float nrnd1 = nrand( n + 0.11*t );	
	float nrnd2 = nrand( n + 0.13*t );
	float nrnd3 = nrand( n + 0.17*t );
	return (nrnd0+nrnd1+nrnd2+nrnd3) / 4.0;
}
// Spiky (around origin) Gaussianish Distribution
float n8rand( vec2 n )
{
	float t = fract( time );
	float nrnd0 = nrand( n + 0.07*t );
	float nrnd1 = nrand( n + 0.11*t );	
	float nrnd2 = nrand( n + 0.13*t );
	float nrnd3 = nrand( n + 0.17*t );
    
    float nrnd4 = nrand( n + 0.19*t );
    float nrnd5 = nrand( n + 0.23*t );
    float nrnd6 = nrand( n + 0.29*t );
    float nrnd7 = nrand( n + 0.31*t );
    
	return (nrnd0+nrnd1+nrnd2+nrnd3 +nrnd4+nrnd5+nrnd6+nrnd7) / 8.0;
}

//	Simplex 3D Noise 
//	by Ian McEwan, Ashima Arts
//
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}

float simplex3(vec3 v)
{ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //  x0 = x0 - 0. + 0.0 * C 
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;

// Permutations
  i = mod(i, 289.0 ); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients
// ( N*N points uniformly over a square, mapped onto an octahedron.)
  float n_ = 1.0/7.0; // N=7
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

vec2 unit_vec2_on_circle(float angle)
{
	return vec2(cos(angle), sin(angle));
}

float random_spread(float center, float spread)
{
	// Map random number between -1 and 1 (using uniform distribution)
	float r = 2.0 * (n1rand(positions[gl_GlobalInvocationID.x].xy * time) - 0.5);
	return center + r * spread;
}

vec2 random_spread_vec2(vec2 center, vec2 spread)
{
	return vec2(random_spread(center.x, spread.x), random_spread(center.y, spread.y));
}

vec2 random_unit_vec2_on_circle(float center, float spread)
{
	float angle = random_spread(center, spread);
	return unit_vec2_on_circle(angle);
}

void main()
{
	uint index = gl_GlobalInvocationID.x;

	vec3 p = positions[index].xyz;
	vec3 v = velocities[index].xyz;
	vec4 c = colors[index];
	float lifetime = lifetimes[index];

	// Motion
	if(wind)
	{
		// Multiply by dt ?
		if(omni_directional)
		{
			v += dt * wind_strength * vec3(unit_vec2_on_circle(pi * (simplex3(vec3(p.x, p.y, dt * wind_turbulence)) + 1.0f)), 0.0f);
		}
		else
		{
			v += dt * wind_strength * vec3(unit_vec2_on_circle(0.5f * pi * (simplex3(vec3(p.x, p.y, dt * wind_turbulence)) + 1.0f)), 0.0f);
		}
	}

	// Move the particle
	p += 0.01f * dt * v;

	// Fade the flame out
	c.a -= dt * (fire_death_speed + fire_triangleness * abs(p.x - emit_center.x));

	lifetime -= fire_death_speed * dt; // Scale this to prolong the lifetime
	// When the particle dies or it fades out
	if(lifetime <= 0.0f || c.a <= 0.0f)
	{
		// Reinstantiate the dead particle using random spread 
		// Note that z position never changes. Fire only flows in xy plane
		p.xy = random_spread_vec2(emit_center.xy, emit_position_spread);
		v.xy = speed * random_unit_vec2_on_circle(pi / 2.0f, emit_angle_variance);
		c.a = 0.5f;
		lifetime = random_spread(1.0f, 0.4f);
	}


	positions[index].xyz = p;
	velocities[index].xyz = v;
	colors[index] = c;
	lifetimes[index] = lifetime;

}
