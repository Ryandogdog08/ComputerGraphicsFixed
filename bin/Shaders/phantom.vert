// normal map vertex shader
#version 410
layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoords;
layout( location = 3 ) in vec4 Tangent;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vTangent;
out vec3 vBiTangent;

uniform mat4 ProjectionViewModel;

uniform mat4 ModelMatrix;

uniform float Time;


void main() {
float scaling = 0.05;
vPosition = ModelMatrix * vec4(Position.x + sin(Time) * scaling, Position.y, Position.z + cos(Time) * scaling, Position.w);
vNormal = (ModelMatrix * Normal).xyz;
vTexCoords = TexCoords;
vTangent = (ModelMatrix * vec4(Tangent.xyz, 0)).xyz;
vBiTangent = cross(vNormal, vTangent) * Tangent.w;

gl_Position = ProjectionViewModel * vec4(Position.x + sin(Time + Position.y) * scaling*Position.y, Position.y, Position.z + cos(Time + Position.y) * scaling*Position.y, Position.w);
}