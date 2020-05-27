#version 300 es
precision highp float;
precision highp int;

in vec2 vTexCoord1;

uniform bool uFinerControls;

uniform float uAmount1;
uniform vec2 uCenter1;

uniform float uAmount2;
uniform vec2 uCenter2;

uniform bool uTileEdges;
uniform float uFalloff;

uniform float uTextureWidth1;
uniform float uTextureHeight1;

uniform sampler2D uBitmap1;

const float epsilon = 1e-5;

vec2 getDeltaByCenter(vec2 xy, vec2 center, float amount) {
	vec2 uv = xy - center;

	{
		float r = length(uv);
		float theta = atan(uv.y, uv.x);

		float x = r;
		if (uFinerControls == true) {
			x *= 100.0;
		}

		/** \brief Reference to https://www.desmos.com/calculator/ibiriw1tnm */
		float b = pow(uFalloff, 4.0);

		float coef = tanh(pow(x, 2.0) / b) / (1.0 + b * exp(pow(x / 2.0, b))) * (1.0 + 4.0 * log(uFalloff + 1.0));
		if (uFalloff > 3.0 - epsilon) {
			coef = 0.0;
		}
		float f = amount / x * coef + r;

		uv.x = f * cos(theta);
		uv.y = f * sin(theta);
	}

	vec2 nxy = uv + center;
	return nxy - xy;
}

void main() {
	vec2 uTextureSize = vec2(uTextureWidth1, uTextureHeight1);
	float aspect = uTextureSize.y / uTextureSize.x;

	vec2 xy = vTexCoord1 * 2.0 - 1.0;
	xy *= vec2(1.0, aspect);

	vec2 delta = vec2(0.0, 0.0);
	delta += getDeltaByCenter(xy, (uCenter1 / uTextureSize - 0.5) * 2.0 * vec2(1.0, aspect), uAmount1);
	delta += getDeltaByCenter(xy, (uCenter2 / uTextureSize - 0.5) * 2.0 * vec2(1.0, aspect), uAmount2);

	xy += delta;
	xy = xy / vec2(1.0, aspect);
	xy = (xy + 1.0) / 2.0;

	if (uTileEdges == false) {
		if (xy.x > 1.0 || xy.y > 1.0 || xy.x < 0.0 || xy.y < 0.0) {
			gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
			return;
		}
	} else {
		xy = abs(xy);
		vec2 frxy = fract(xy);
		float x = xy.x;
		int xI = int(x);
		if (xI % 2 == 1) {
			frxy.x = 1.0 - frxy.x;
		}
		float y = xy.y;
		int yI = int(y);
		if (yI % 2 == 1) {
			frxy.y = 1.0 - frxy.y;
		}
		xy = frxy;
	}

	
	gl_FragColor = texture2D(uBitmap1, xy);
}