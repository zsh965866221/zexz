#version 300 es
precision highp float;
precision highp int;

uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

uniform vec2  iResolution;

in vec2 TexCoord;
out vec4 myFragColor;


#define colorRange 24.0

vec3 jodieReinhardTonemap(vec3 c){
    float l = dot(c, vec3(0.2126, 0.7152, 0.0722));
    vec3 tc = c / (c + 1.0);

    return mix(c / (l + 1.0), tc, tc);
}

vec3 bloomTile(float lod, vec2 offset, vec2 uv){
    return texture(iChannel1, uv * exp2(-lod) + offset).rgb;
}

vec3 getBloom(vec2 uv){

    vec3 blur = vec3(0.0);

    blur = pow(bloomTile(2., vec2(0.0,0.0), uv),vec3(2.2))       	   	+ blur;
    blur = pow(bloomTile(3., vec2(0.3,0.0), uv),vec3(2.2)) * 1.3        + blur;
    blur = pow(bloomTile(4., vec2(0.0,0.3), uv),vec3(2.2)) * 1.6        + blur;
    blur = pow(bloomTile(5., vec2(0.1,0.3), uv),vec3(2.2)) * 1.9 	   	+ blur;
    blur = pow(bloomTile(6., vec2(0.2,0.3), uv),vec3(2.2)) * 2.2 	   	+ blur;

    return blur * colorRange;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = vec2(TexCoord.x, TexCoord.y);
    
    vec3 color = pow(texture(iChannel0, uv).rgb * colorRange, vec3(2.2));
    color = pow(color, vec3(2.2));
    color += pow(getBloom(uv), vec3(2.2));
    color = pow(color, vec3(1.0 / 2.2));
    
    color = jodieReinhardTonemap(color);
    
	fragColor = vec4(color,1.0);
}

void main() {
  vec2 fragCoord = vec2(gl_FragCoord.x, gl_FragCoord.y);
  mainImage(myFragColor, fragCoord);
}
