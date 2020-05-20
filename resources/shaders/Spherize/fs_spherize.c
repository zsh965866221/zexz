#if defined(ETGL_EGL_IMAGE_EXTERNAL)
#extension GL_OES_EGL_image_external : require
#endif

precision mediump float;

#ifdef ETGL_TEXEL1_EXT
uniform samplerExternalOES uBitmap1;
#else
uniform sampler2D uBitmap1;
#endif

varying vec2 vTexCoord1;

uniform float uRadius;
uniform vec2 uCenter;
uniform float uTextureWidth1;
uniform float uTextureHeight1;

const float epsilon = 1e-5;

/** \brief the function based r, please visit: https://www.desmos.com/calculator/py5yksrcna */
void main() {
	vec2 uTextureSize = vec2(uTextureWidth1, uTextureHeight1);
	float aspect = uTextureSize.y / uTextureSize.x;
	float radius = uRadius / (uTextureSize.x / 2.0);

	radius = clamp(radius, 0.0, 1.5);

	vec2 xy = vTexCoord1 * 2.0 - 1.0;
	vec2 uv = xy - (uCenter / uTextureSize - 0.5) * 2.0;
	uv *= vec2(1.0, aspect);

	float r = length(uv);
	float theta = atan(uv.y, uv.x);
	
	if (r < radius  + epsilon) {
		float k = r / radius;
		float f = 
			+ 0.20 * pow(k, 4.0)
			+ 0.20 * pow(k, 3.0)
			+ 0.00 * pow(k, 2.0)
			+ 0.60 * pow(k, 1.0);
		r = f * radius;
	}

	uv.x = r * cos(theta);
	uv.y = r * sin(theta);

	xy = uv / vec2(1.0, aspect);
	xy = xy + (uCenter / uTextureSize - 0.5) * 2.0;
	xy = (xy + 1.0) / 2.0;
	
	gl_FragColor = texture2D(uBitmap1, xy);
}