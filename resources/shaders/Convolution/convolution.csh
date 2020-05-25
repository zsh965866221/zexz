#version 310 es
precision highp float;
precision highp int;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 4) in;

// Channels x Width x Height
layout(binding = 0, std430) readonly buffer InputData {
  float inputData[];
};

// Channels x Width x Height
layout(binding = 1, std430) writeonly buffer OutputData {
  float outputData[];
};

// Channels_out x Channels_in x Kernel_w x Kernel_h
layout(binding = 2, std430) readonly buffer KernelData {
  float kernelData[];
};

// Channels_out
layout(binding = 3, std430) readonly buffer BiasData {
  float biasData[];
};

// in out
uniform ivec2 uChannels;
uniform ivec2 uImageSize;
uniform ivec2 uKernel;

void main(void) {
  // W H C
  ivec3 id = ivec3(gl_GlobalInvocationID.xyz);
  if (id.x >= uImageSize.x || 
      id.y >= uImageSize.y ||
      id.z >= uChannels.y) {
    return;
  }

  // kernel stride
  int cxy_kindex = id.z * uChannels.y;

  float total = 0.0;
  for (int c = 0; c < uChannels.x; c++) {
    int xy_index = c * uImageSize.x * uImageSize.y;
    int xy_kindex = cxy_kindex + c * uKernel.x * uKernel.y;
    for (int dx = -uKernel.x / 2; dx <= uKernel.x / 2; dx++) {
      int x = id.x + dx;
      if (x < 0 || x >= uImageSize.x) {
        continue;
      }
      int y_index = xy_index + x * uImageSize.y;
      int y_kindex = xy_kindex + dx * uKernel.y;
      for (int dy = -uKernel.y / 2; dy <= uKernel.y / 2; dy++) {
        int y = id.y + dy;
        int index = y_index + y;
        int kindex = xy_kindex + dy;
        if (x < 0 || x >= uImageSize.x ||
            y < 0 || y >= uImageSize.y) {
          continue;
        }
        float v = inputData[index];
        float w = kernelData[kindex];
        total += (w * v);
      }
    }
  }

  int outIndex = id.z * uImageSize.x * uImageSize.y + id.x * uImageSize.y + id.y;
  outputData[outIndex] = total + biasData[id.z];
}