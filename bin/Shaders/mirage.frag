// normal map fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

uniform vec3 CameraPos;
uniform float Scaling1;

out vec4 FragColour;

vec3 getDiffuse(vec3 direction, vec3 colour, vec3 normal)
{
return colour * max( 0, dot( normal, -direction ) );
}

vec3 getSpecular(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
vec3 R = reflect( direction, normal );
float specularTerm = pow( max( 0, dot( R, view ) ), specularPower );
return specularTerm * colour;
}

void main() {
// ensure normal and light direction are normalised
vec3 N = normalize(vNormal);
vec3 L = normalize(LightDirection);
vec3 T = normalize(vTangent);
vec3 B = normalize(vBiTangent);

mat3 TBN = mat3(T,B,N);

vec3 textureColour = texture(diffuseTex,vTexCoords).rgb;
vec3 textureSpecular = texture(specularTex,vTexCoords).rgb;
vec3 textureNormal = texture(normalTex,vTexCoords).rgb;

N = TBN * (textureNormal * 2 - 1);

// calculate lambert term (negate light direction)
vec3 diffuseTotal = getDiffuse(L,LightColour,N);

vec3 V = normalize(cameraPosition - vPosition.xyz);
vec3 R = reflect (L,N);

vec3 specularTotal = getSpecular(L, LightColour, N, V);

for (int i=0; i<numLights; i++)
{
vec3 direction = vPosition.xyz - PointLightPosition[i];
float distance = length(direction);
direction = direction/distance;
// attenuate the light intensity with inverse square law
vec3 colour = PointLightColour[i]/(distance * distance);
diffuseTotal += getDiffuse(direction, colour, N);
specularTotal += getSpecular(direction, colour, N, V);
}

vec3 ambient = AmbientColour * Ka * textureColour;
vec3 diffuse = diffuseTotal * Kd * textureColour;
vec3 specular = Ks * specularTotal * textureSpecular;

float distance = (sqrt((CameraPos.x - abs(vPosition.x))*(CameraPos.x - abs(vPosition.x)) + (CameraPos.y - abs(vPosition.y))*(CameraPos.y - abs(vPosition.y)) + (CameraPos.z - abs(vPosition.z))*(CameraPos.z - abs(vPosition.z))))/20 - 0.1*Scaling1;
if (distance > 1) { distance = 1;}

// output final colour
FragColour = vec4( ambient + diffuse + specular, distance );
//FragColour = vec4( N,1 );
}