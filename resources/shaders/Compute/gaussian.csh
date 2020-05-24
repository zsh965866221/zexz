#version 320 es
precision highp float;
precision highp int;

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(rgba32f) highp uniform image2D uImageIn;
layout(rgba32f) highp uniform image2D uImageOut;

uniform float uSigma;

void main(void) {
  ivec2 id = ivec2(gl_GlobalInvocationID.xy);
  ivec2 size = imageSize(uImageOut);
  if (id.x >= size.x || id.y >= size.y) {
    return;
  }

  // 0.9544
  int kernel = int(ceil(2.0 * uSigma));

  vec4 pixel = vec4(0.0);
  float coef = 0.0;
  for (int dx = -kernel; dx <= kernel; dx++) {
    for (int dy = -kernel; dy <= kernel; dy++) {
      int x = id.x + dx;
      int y = id.y + dy;
      if (x < 0 || x >= size.x ||
          y < 0 || y >= size.y) {
        continue;
      }
      float c = exp(-float(dx * dx + dy * dy) / (2.0 * uSigma * uSigma + 1e-5));
      pixel += (imageLoad(uImageIn, ivec2(x, y)) * c);
      coef += c;
    }
  }
  imageStore(uImageOut, id, pixel / (coef + 1e-5));
}