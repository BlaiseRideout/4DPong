#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 cameraPosition;
uniform float in_Color;
uniform float in_Alpha;

out vec4 vertColor;

void main(){
  mat4 MVP = P * V * M;
  gl_Position = MVP * vec4(vertexPosition, 1);

  vec3 position_worldspace = (M * vec4(vertexPosition, 1)).xyz;

  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition, 1)).xyz;
  vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
   
  // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
  vec3 LightPosition_cameraspace = ( V * vec4(-2, 2, -2, 1)).xyz;
  vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
   
  // Normal of the the vertex, in camera space
  vec3 Normal_cameraspace = ( V * inverse(transpose(M)) * vec4(vertexNormal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

  vec3 n = normalize(Normal_cameraspace);
  vec3 l = normalize(LightDirection_cameraspace);

  // Eye vector (towards the camera)
  vec3 E = normalize(EyeDirection_cameraspace);
  // Direction in which the triangle reflects the light
  vec3 R = reflect(-l,n);
  // Cosine of the angle between the Eye vector and the Reflect vector,
  // clamped to 0
  //  - Looking into the reflection -> 1
  //  - Looking elsewhere -> < 1
  float cosAlpha = clamp( dot( E,R ), 0,1 );

  vec3 MaterialDiffuseColor = vec3(in_Color, 1.0f - in_Color, 0.0f);
  vec3 MaterialAmbientColor = vec3(0.2, 0.2, 0.2) * MaterialDiffuseColor;

  float distance = length(LightDirection_cameraspace);

  float cosTheta = clamp(dot( n,l ), 0, 1);
  vec3 color = MaterialDiffuseColor * vec3(1.0f, 1.0f, 1.0f) * 10 * cosTheta / pow(distance, 2);

  color = vec3(1.0f, 1.0f, 1.0f) * 2 * pow(cosAlpha, 5) / pow(distance, 2) + color;

  vertColor = vec4(MaterialAmbientColor + color, in_Alpha);
}
