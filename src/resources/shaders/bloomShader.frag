#version 420 core

layout(location = 0) out vec4 FragColor;
  
in vec2 vTexCoords;

layout(binding = 0) uniform sampler2D image;
layout(binding = 1) uniform sampler2D bloomImage;
  
uniform float sampleScale;
uniform float colorIntesity;
uniform vec4 threshold;
uniform float params;
uniform int pass;

#define HALF_MAX 65504.0 // (2 - 2^-10) * 2^15

/* Clamp to avoid black square artifacts if a pixel goes NaN. */
/* Currently unused */
vec4 SafeColor(vec4 c) {
//  return clamp(c, vec4(0.0), vec4(1e20)); /* 1e20 arbitrary. */
	return min(c, HALF_MAX);
}

/* Quadratic color thresholding
 * curve = (threshold - knee, knee * 2, 0.25 / knee)
 */
vec4 QuadraticThreshold(vec4 color, float threshold, vec3 curve, vec2 texelSize) {

    // Pixel brightness
    float br = max(max(color.r, color.g), color.b);

    // Under-threshold part: quadratic curve
    float rq = clamp(br - curve.x, 0.0, curve.y);
    rq = curve.z * rq * rq;

    // Combine and apply the brightness response curve.
    color *= max(rq, br - threshold) / max(br, 1.0e-4);

    return color;
}

// Better, temporally stable box filtering
// http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
// . . . . . . .
// . A . B . C .
// . . D . E . .
// . F . G . H .
// . . I . J . .
// . K . L . M .
// . . . . . . .
vec4 DownsampleBox13(vec2 texelSize) {
	vec4 A = texture(image, vTexCoords + texelSize * vec2(-1.0, -1.0));
	vec4 B = texture(image, vTexCoords + texelSize * vec2(0.0, -1.0));
	vec4 C = texture(image, vTexCoords + texelSize * vec2(1.0, -1.0));
	vec4 D = texture(image, vTexCoords + texelSize * vec2(-0.5, -0.5));
	vec4 E = texture(image, vTexCoords + texelSize * vec2(0.5, -0.5));
	vec4 F = texture(image, vTexCoords + texelSize * vec2(-1.0, 0.0));
	vec4 G = texture(image, vTexCoords);		  
	vec4 H = texture(image, vTexCoords + texelSize * vec2(1.0, 0.0));
	vec4 I = texture(image, vTexCoords + texelSize * vec2(-0.5, 0.5));
	vec4 J = texture(image, vTexCoords + texelSize * vec2(0.5, 0.5));
	vec4 K = texture(image, vTexCoords + texelSize * vec2(-1.0, 1.0));
	vec4 L = texture(image, vTexCoords + texelSize * vec2(0.0, 1.0));
	vec4 M = texture(image, vTexCoords + texelSize * vec2(1.0, 1.0));

	vec2 div = (1.0 / 4.0) * vec2(0.5, 0.125);

	vec4 o = (D + E + I + J) * div.x;
	o += (A + B + G + F) * div.y;
	o += (B + C + H + G) * div.y;
	o += (F + G + L + K) * div.y;
	o += (G + H + M + L) * div.y;

	return o;
}

// 9-tap bilinear upsampler (tent filter)
vec4 UpsampleTent(vec2 texelSize) {
	vec4 d = texelSize.xyxy * vec4(1.0, 1.0, -1.0, 0.0) * sampleScale;

	vec4 s;
	s = texture(image, vTexCoords - d.xy);
	s += texture(image, vTexCoords - d.wy) * 2.0;
	s += texture(image, vTexCoords - d.zy);
	
	s += texture(image, vTexCoords + d.zw) * 2.0;
	s += texture(image, vTexCoords) * 4.0;
	s += texture(image, vTexCoords + d.xw) * 2.0;

	s += texture(image, vTexCoords + d.zy);
	s += texture(image, vTexCoords + d.wy) * 2.0;
	s += texture(image, vTexCoords + d.xy);

	return s * (1.0 / 16.0);
}

vec4 Prefilter(vec4 color, vec2 texelSize) {
	color = QuadraticThreshold(color, threshold.x, threshold.yzw, texelSize);
	return color;
}

vec4 Combine(vec4 bloom) {
	vec4 color = texture(bloomImage, vTexCoords);

	return bloom + color;
}

void main() {
	if (pass == 0) {
		vec2 texelSize = 1.0 / textureSize(image, 0); // Gets size of a single texel.
		FragColor = Prefilter(SafeColor(DownsampleBox13(texelSize)), texelSize);
	} else if (pass == 1) {
		vec2 texelSize = 1.0 / textureSize(image, 0); // Gets size of a single texel.
		FragColor = DownsampleBox13(texelSize);
	} else if (pass == 2) {
		vec2 texelSize = 1.0 / textureSize(image, 0); // Gets size of a single texel.
		FragColor = Combine(UpsampleTent(texelSize));
	}
}