#version 430
precision highp float;
precision highp int;

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(rgba32f) readonly highp uniform image2D uImageSrc;
layout(rgba32f) writeonly highp uniform image2D uImageOut;

void main(void) {
  ivec2 id = ivec2(gl_GlobalInvocationID.xy);
  ivec2 size = imageSize(uImageSrc);

  if (id.x >= size.x || id.y >= size.y) {
    return;
  }

  vec4 Color = imageLoad(uImageSrc, id);
  imageStore(uImageOut, id, Color);
}