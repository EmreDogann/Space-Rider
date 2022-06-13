#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix;
	mat4 modelMatrix;
	mat3 normalMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

uniform float time;
uniform int type;

out vec4 vWorldPosition;
out vec2 vTexCoord;	// Texture coordinate
out vec4 positionView;	// position of shadow being shaded in view space.
out vec3 normalView;	// normal of shadow being shaded in view space.
out vec3 normal;
out float vNoise;
flat out int vType;

//	Simplex 4D Noise 
//	by Ian McEwan, Ashima Arts
//
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
float permute(float x){return floor(mod(((x*34.0)+1.0)*x, 289.0));}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}
float taylorInvSqrt(float r){return 1.79284291400159 - 0.85373472095314 * r;}

vec4 grad4(float j, vec4 ip) {
	const vec4 ones = vec4(1.0, 1.0, 1.0, -1.0);
	vec4 p,s;

	p.xyz = floor( fract (vec3(j) * ip.xyz) * 7.0) * ip.z - 1.0;
	p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
	s = vec4(lessThan(p, vec4(0.0)));
	p.xyz = p.xyz + (s.xyz*2.0 - 1.0) * s.www; 

	return p;
}

float snoise4D(vec4 v) {
	const vec2  C = vec2( 0.138196601125010504,  // (5 - sqrt(5))/20  G4
						0.309016994374947451); // (sqrt(5) - 1)/4   F4
	// First corner
	vec4 i  = floor(v + dot(v, C.yyyy) );
	vec4 x0 = v -   i + dot(i, C.xxxx);

	// Other corners

	// Rank sorting originally contributed by Bill Licea-Kane, AMD (formerly ATI)
	vec4 i0;

	vec3 isX = step( x0.yzw, x0.xxx );
	vec3 isYZ = step( x0.zww, x0.yyz );
	//  i0.x = dot( isX, vec3( 1.0 ) );
	i0.x = isX.x + isX.y + isX.z;
	i0.yzw = 1.0 - isX;

	//  i0.y += dot( isYZ.xy, vec2( 1.0 ) );
	i0.y += isYZ.x + isYZ.y;
	i0.zw += 1.0 - isYZ.xy;

	i0.z += isYZ.z;
	i0.w += 1.0 - isYZ.z;

	// i0 now contains the unique values 0,1,2,3 in each channel
	vec4 i3 = clamp( i0, 0.0, 1.0 );
	vec4 i2 = clamp( i0-1.0, 0.0, 1.0 );
	vec4 i1 = clamp( i0-2.0, 0.0, 1.0 );

	//  x0 = x0 - 0.0 + 0.0 * C 
	vec4 x1 = x0 - i1 + 1.0 * C.xxxx;
	vec4 x2 = x0 - i2 + 2.0 * C.xxxx;
	vec4 x3 = x0 - i3 + 3.0 * C.xxxx;
	vec4 x4 = x0 - 1.0 + 4.0 * C.xxxx;

	// Permutations
	i = mod(i, 289.0); 
	float j0 = permute( permute( permute( permute(i.w) + i.z) + i.y) + i.x);
	vec4 j1 = permute( permute( permute( permute (
				i.w + vec4(i1.w, i2.w, i3.w, 1.0 ))
			+ i.z + vec4(i1.z, i2.z, i3.z, 1.0 ))
			+ i.y + vec4(i1.y, i2.y, i3.y, 1.0 ))
			+ i.x + vec4(i1.x, i2.x, i3.x, 1.0 ));
	// Gradients
	// ( 7*7*6 points uniformly over a cube, mapped onto a 4-octahedron.)
	// 7*7*6 = 294, which is close to the ring size 17*17 = 289.

	vec4 ip = vec4(1.0/294.0, 1.0/49.0, 1.0/7.0, 0.0) ;

	vec4 p0 = grad4(j0,   ip);
	vec4 p1 = grad4(j1.x, ip);
	vec4 p2 = grad4(j1.y, ip);
	vec4 p3 = grad4(j1.z, ip);
	vec4 p4 = grad4(j1.w, ip);

	// Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;
	p4 *= taylorInvSqrt(dot(p4,p4));

	// Mix contributions from the five corners
	vec3 m0 = max(0.6 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.0);
	vec2 m1 = max(0.6 - vec2(dot(x3,x3), dot(x4,x4)            ), 0.0);
	m0 = m0 * m0;
	m1 = m1 * m1;
	return 49.0 * ( dot(m0*m0, vec3( dot( p0, x0 ), dot( p1, x1 ), dot( p2, x2 )))
				+ dot(m1*m1, vec2( dot( p3, x3 ), dot( p4, x4 ) ) ) );
}

float snoise4DFractal(vec4 m) {
	return 0.5333333 * snoise4D(m)
		+ 0.2666667 * snoise4D(2.0*m)
		+ 0.1333333 * snoise4D(4.0*m)
		+ 0.0666667 * snoise4D(8.0*m);
}

//	Simplex 3D Noise 
//	by Ian McEwan, Ashima Arts
//
float snoise3D(vec3 v) { 
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

float snoise3DFractal(vec3 m) {
	return 0.5333333 * snoise3D(m)
		+ 0.2666667 * snoise3D(2.0*m)
		+ 0.1333333 * snoise3D(4.0*m)
		+ 0.0666667 * snoise3D(8.0*m);
}

// Rotate a vertex around the global y axis by specified angle.
vec3 rotateY(vec3 v, float angle) {
	float s = sin(angle);
	float c = cos(angle);

	mat3 rotationMatrix = mat3(
		c, 0.0, -s,
		0.0, 1.0, 0.0,
		s, 0.0, c
	);

	return rotationMatrix * v;
}

void main() {
	float scaledTime = time * 0.00025;
	vec3 newPosition = vec3(0.0);
	if (type == 0) {
		vType = type;
		newPosition = inPosition;

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 1) { // Swirl
		float frequency = 1.0/1.5;
		float amplitude = 1.0/2.0;
		float displacement = 0.15 + amplitude * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime) * frequency);
		vNoise = displacement;
		vType = type;

		newPosition = inPosition - inNormal * displacement;
		float angle = sin(inCoord.y * 5 + scaledTime) * 4;
		newPosition = rotateY(newPosition, angle);

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 2) { // Erratic ball
		float displacement = 0.1 + 1.5 * snoise3DFractal(vec3(inPosition.x * 0.06 + scaledTime, inPosition.y * 0.12, inPosition.z * 0.06))
						   + snoise3DFractal(vec3(inPosition.x * 0.02, inPosition.y * 0.06, inPosition.z * 0.02 + scaledTime));
		vNoise = displacement;
		vType = type;
		newPosition = inPosition + inNormal * displacement;

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 3) { // Regular Planet
		float frequency = 1.0/1.1;
		float amplitude = 1.0/10.0;
		float displacement = 0.1 + amplitude * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.25) * frequency);
		vNoise = displacement;
		vType = type;

		newPosition = inPosition - inNormal * displacement;
		float angle = sin(inCoord.y / 2 + scaledTime*0.85);
		newPosition = rotateY(newPosition, angle);

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 4) { // Storm
		float frequency = 1.0/1.25;
		float amplitude = 1.0/3.0;
		float displacement = 0.5 + amplitude * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * frequency);
		displacement -= 1.0/25.0 * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * 15.0);
		displacement -= 1.0/5.0 * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * 2.0);
		
		vNoise = displacement;
		vType = type;

		newPosition = inPosition - inNormal * displacement;
		float angle = sin(inCoord.y / 5 + scaledTime*0.5);
		newPosition = rotateY(newPosition, angle);

		newPosition = inPosition + inNormal * displacement;

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 5) { // Sun
		float frequency = 1.0/1.25;
		float amplitude = 1.0/7.0;
		float displacement = -0.1 + amplitude * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * frequency);
		displacement -= 1.0/25.0 * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * 15.0);
		displacement -= 1.0/5.0 * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * 2.0);
		
		vNoise = displacement;
		vType = type;

		newPosition = inPosition - inNormal * displacement;
		float angle = sin(inCoord.y / 2 + scaledTime*0.5);
		newPosition = rotateY(newPosition, angle);

		newPosition = inPosition + inNormal * displacement;

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 6) { // Sun Jr.
		float frequency = 1.0/1.15;
		float amplitude = 1.0/4.0;
		float displacement = -0.1 + amplitude * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * frequency);
		displacement -= 1.0/25.0 * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.5) * 15.0);
		
		vNoise = displacement;
		vType = type;

		newPosition = inPosition - inNormal * displacement;
		float angle = sin(inCoord.y / 2 + scaledTime*0.5);
		newPosition = rotateY(newPosition, angle);

		newPosition = inPosition + inNormal * displacement;

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 7) { // Fireball
		float frequency = 1.0/3.0;
		float amplitude = 1.0/1.2;
		float displacement = amplitude * snoise3DFractal(vec3(inPosition.x, inPosition.y + scaledTime * 3.0, inPosition.z + scaledTime) * frequency);
		displacement += 1.0/10.0 * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime) * 4.0);
		vNoise = displacement;
		vType = type;

		newPosition = inPosition - inNormal * displacement;

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	} else if (type == 8) { // Regular Planet 2 (Earth)
		float frequency = 1.0/2.0;
		float amplitude = 1.0/10.0;
		float displacement = 0.1 + amplitude * snoise4DFractal(vec4(inPosition.x, inPosition.y, inPosition.z, scaledTime*0.25) * frequency);
		vNoise = displacement;
		vType = type;

		newPosition = inPosition - inNormal * displacement;
		float angle = sin(inCoord.y / 2 + scaledTime*0.85);
		newPosition = rotateY(newPosition, angle);

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	}
	
	// Get the vertex normal and vertex position in eye coordinates
	vec3 vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	vec4 vEyePosition = matrices.modelViewMatrix * vec4(newPosition, 1.0);
	
	vWorldPosition = matrices.modelMatrix * vec4(newPosition, 1.0);
	positionView = vEyePosition;
	normalView = vEyeNorm;
	normal = inNormal;

	// Pass through the texture coordinate
	vTexCoord = inCoord;
} 
	