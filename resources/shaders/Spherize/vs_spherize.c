attribute vec4 aPosition;
attribute vec2 aTexCoord1;

uniform highp mat4 uMVPMatrix;
uniform highp mat4 uTexuvMat1;
uniform highp vec2 uTextureSize;

varying highp vec2 vTexCoord1;

void main() {
	gl_Position = uMVPMatrix * aPosition;
	vTexCoord1 = (uTexuvMat1 * vec4(aTexCoord1, 0.0, 1.0)).xy;
}