#version 300 es
precision highp float;
precision highp int;

out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

uniform float angle_X;
uniform float angle_Y;
uniform float distance_plane;
uniform bool border;

const float PI = 3.1415926535898;
const float distance_screen = 1.0;
const float border_radius = 0.005;

const vec3 plane_origin = vec3(0.0, 0.0, 0.0);
const vec3 plane_norm = vec3(0.0, 0.0, 1.0);
const vec3 plane_up = vec3(0.0, 1.0, 0.0);
const vec3 camera_origin = vec3(0.0, 0.0, -2.0);

const float epsilon = 1e-7;

struct Plane {
  vec3 origin;
  vec3 norm;
  vec3 up;
};

struct Ray {
  vec3 origin;
  vec3 direction;
};

struct Camera {
  vec3 origin;
  vec3 lookat;
  vec3 up;
};

// https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
vec3 intersectoinPoint(Ray ray,Plane plane) {
  float d = dot((plane.origin - ray.origin), plane.norm) 
            / dot(plane.norm, ray.direction);
  return d * ray.direction + ray.origin;
}

vec3 fromUVtoWrold(vec2 uv, Camera camera) {
  Plane screen;
  vec3 n = normalize(camera.lookat);
  screen.origin = camera.origin + n * distance_screen;
  screen.norm = n;
  vec3 axis_x = cross(camera.up, screen.norm);
  axis_x = normalize(axis_x);
  vec3 axis_y = cross(screen.norm, axis_x);
  axis_y = normalize(axis_y);
  uv = (uv - 0.5);
  return screen.origin + axis_x * uv.x + axis_y * uv.y;
}

vec2 fromWroldtoPlane(vec3 world, Plane plane) {
  // camera not rotate with lookat
  vec3 axis_x = cross(plane.up, plane.norm);
  vec3 axis_y = cross(plane.norm, axis_x);
  float x = dot(world - plane.origin, axis_x);
  float y = dot(world - plane.origin, axis_y);
  return (vec2(x, y) + 1.0) / 2.0;
}

vec2 fromUVtoPlane(vec2 uv, Plane plane, Camera camera) {
  vec3 world = fromUVtoWrold(uv, camera);
  Ray ray;
  ray.origin = camera.origin;
  ray.direction = normalize(world - camera.origin);
  vec3 point = intersectoinPoint(ray, plane);
  return fromWroldtoPlane(point, plane);
}

float fromAngleToRadian(float angle) {
  return angle / 180.0 * PI;
}

// https://en.wikipedia.org/wiki/Rotation_matrix
mat3 fromAxisAngleToMatrix(vec3 axis, float theta) {
  axis = normalize(axis);
  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  float ct = cos(theta);
  float st = sin(theta);
  float ct1 = 1.0 - ct;
  return transpose(mat3(
    ct + x * x * ct1,       x * y * ct1 - z * st,     x * y * ct1 + y * st,
    y * x * ct1 + z * st,   ct + y * y * ct1,         y * z * ct1 - x * st,
    z * x * ct1 - y * st,   z * y * ct1 + x * st,     ct + z * z * ct1 
  ));
}

void rotateCamera(float angle, vec3 axis, Plane plane, inout Camera camera) {
  float theta = fromAngleToRadian(angle);
  mat3 M = fromAxisAngleToMatrix(axis, theta);
  vec3 vector = camera.origin - plane.origin;
  vector = M * vector;
  camera.origin = plane.origin + vector;
  camera.lookat = normalize(plane.origin - camera.origin);
  camera.up = M * camera.up;
}

void rotatePlane(float angle, vec3 axis, inout Plane plane) {
  float theta = fromAngleToRadian(angle);
  mat3 M = fromAxisAngleToMatrix(axis, theta);
  plane.norm = M * plane.norm;
  plane.up = M * plane.up;
}

void main() {
	Plane plane;
  plane.origin = plane_origin + vec3(0.0, 0.0, distance_plane);
  plane.norm = plane_norm;
  plane.up = plane_up;
  
  Camera camera;
  camera.origin = camera_origin;
  camera.lookat = normalize(plane.origin - camera.origin);
  camera.up = plane_up;

	rotatePlane(angle_X, vec3(1.0, 0.0, 0.0), plane);
  rotatePlane(angle_Y, vec3(0.0, 1.0, 0.0), plane);

	vec2 uv = TexCoord;

  uv = fromUVtoPlane(uv, plane, camera);

  if (uv.x < 0.0 - epsilon || 
      uv.x > 1.0 + epsilon || 
      uv.y < 0.0 - epsilon || 
      uv.y > 1.0 + epsilon) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  } else {
    FragColor = texture(texture1, uv);
  }
  if (border == true) {
    if (TexCoord.x < 0.0 + border_radius || 
        TexCoord.x > 1.0 - border_radius || 
        TexCoord.y < 0.0 + border_radius || 
        TexCoord.y > 1.0 - border_radius) {
      FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
  }
}