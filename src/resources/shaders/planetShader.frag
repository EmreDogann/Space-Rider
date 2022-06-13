#version 400 core

in vec4 vWorldPosition;
in vec4 positionView;	// position of shadow being shaded in view space.
in vec3 normalView;		// normal of shadow being shaded in view space.
in vec3 normal;
in vec2 vTexCoord;		// Interpolated texture coordinate using texture coordinate from the vertex shader
in float vNoise;
flat in int vType;

uniform float farPlane;
uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube depthMap;  // The point light shadow cube map sampler
uniform bool bUseNoise;
uniform bool bUseTexture;
uniform bool bLightEmitter;

layout(location = 0) out vec4 vOutputColour;	// The output colour

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

// Lights and materials passed in as uniform variables from client programme
uniform LightInfo light1; 
uniform MaterialInfo material1;

// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 PhongModel(vec4 eyePosition, vec3 eyeNorm) {
	vec3 s = normalize(vec3(light1.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 h = normalize(v + s);
	vec3 n = eyeNorm;
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light1.Ls * material1.Ms * pow(max(dot(h, n), 0.0f), material1.shininess + eps);
	
	return ambient + diffuse + specular;
}

// http://www.iquilezles.org/www/articles/palettes/palettes.htm
// As t runs from 0 to 1 (our normalized palette index or domain), 
//the cosine oscilates c times with a phase of d. 
//The result is scaled and biased by a and b to meet the desired constrast and brightness.
vec3 cosPalette( float t, vec3 a, vec3 b, vec3 c, vec3 d )
{
    return a + b*cos( 6.28318*(c*t+d) );
}

vec3 sampleOffsetDirections[20] = vec3[](
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 

float ShadowCalculation() {
	vec3 fragToLight = vWorldPosition.xyz - light1.position.xyz;
	float closestDepth = texture(depthMap, fragToLight).r;

	// Closest depth is in linear range [0,1]. So, re-transform to original value.
	closestDepth *= farPlane;
	float currentDepth = length(fragToLight);

	// Vary the bias depending on the surface normal. The further away the surface normal, the greater the bias.
	float bias = 0.01*tan(acos(dot(normal, (light1.position.xyz - vWorldPosition.xyz))));
	bias = clamp(bias, 0.0,300.0);

	float shadow = 0.0;
	int samples = 20;
	for(int i = 0; i < samples; ++i) {
		float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * 3.0).r;
		closestDepth *= farPlane;   // undo [0,1] mapping
		if(currentDepth - bias > closestDepth) {
			shadow += 0.95;
		}
	}
	shadow /= float(samples);

	return shadow;
}

void main() {
	vec3 vColor;
	if (bLightEmitter) {
		vColor = PhongModel(positionView, normalize(normalView));
	} else {
		vColor = PhongModel(positionView, normalize(normalView)) * (1.0 - ShadowCalculation());
	}

	if (bUseTexture) {
		vColor *= pow(texture(sampler0, vTexCoord).rgb, vec3(2.2));
	}

	vec3 color = vec3(1.0);

	if (vType == 1) { // Swirl
		vec3 brightness = vec3(0.5, 0.5, 0.5);
		vec3 contrast = vec3(0.5, 0.5, 0.5);
		vec3 oscilation = vec3(1.0, 1.0, 1.0);
		vec3 phase = vec3(0.0, 0.2, 0.3);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 2.5;
	} else if (vType == 2) { // Erratic ball
		vec3 brightness = vec3(0.8, 0.5, 0.4);
		vec3 contrast = vec3(0.2, 0.4, 0.2);
		vec3 oscilation = vec3(2.0, 1.0, 1.0);
		vec3 phase = vec3(0.0, 0.25, 0.25);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 2.0;
	} else if (vType == 3) { // Regular Planet
		vec3 brightness = vec3(0.5);
		vec3 contrast = vec3(0.5);
		vec3 oscilation = vec3(0.5);
		vec3 phase = vec3(1.35);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 13.0;
	} else if (vType == 4) { // Storm
		vec3 brightness = vec3(0.2);
		vec3 contrast = vec3(0.2);
		vec3 oscilation = vec3(0.5);
		vec3 phase = vec3(1.15);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 2.0;
	} else if (vType == 5) { // Sun
		vec3 brightness = vec3(0.5, 0.5, 0.5);
		vec3 contrast = vec3(1.0, 1.0, 2.0);
		vec3 oscilation = vec3(0.5, 0.5, 0.4);
		vec3 phase = vec3(0.2, 0.25, 0.3);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 5.0;
	} else if (vType == 6) { // Sun Jr.
		vec3 brightness = vec3(0.5, 0.5, 0.5);
		vec3 contrast = vec3(1.0, 1.0, 2.0);
		vec3 oscilation = vec3(0.5, 0.5, 0.4);
		vec3 phase = vec3(0.2, 0.25, 0.3);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 7.0;
	} else if (vType == 7) { // Fireball
		vec3 brightness = vec3(0.985, 0.1, 0.0);
		vec3 contrast = vec3(1.0, 0.62, 0.0);
		vec3 oscilation = vec3(0.27, 0.59, 0.0);
		vec3 phase = vec3(0.60, 0.64, 0.0);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 10.0;
	} else if (vType == 8) { // Regular Planet 2 (Earth)
		vec3 brightness = vec3(0.5);
		vec3 contrast = vec3(0.9);
		vec3 oscilation = vec3(0.5);
		vec3 phase = vec3(1.05);

		color = cosPalette(vNoise, brightness, contrast, oscilation, phase) * 3.0;
	}

	vOutputColour = vec4(color * vColor, 1.0);
}