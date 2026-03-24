#pragma once
#include "Camera.h"
#include "Mesh.h"	
#include "Shader.h"

class Scene;



class Instance
{
public:
	Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader);
	Instance(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Mesh* mesh, aie::ShaderProgram* shader);

	void draw(Scene* scene);

	glm::mat4 makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
	void setTransform(glm::mat4 transform) { m_transform = transform; }
	void setTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) { m_transform = makeTransform(position, rotation, scale); }

	aie::ShaderProgram* getShader() { return m_shader; }
	void setShader(aie::ShaderProgram* _shader) { m_shader = _shader; }
	Mesh* getMesh() { return m_mesh; }

	void setShaderScaling1(float _value) { m_shaderScaling1 = _value; }
	void setShaderScaling2(float _value) { m_shaderScaling2 = _value; }
	void setShaderScaling3(float _value) { m_shaderScaling3 = _value; }

protected:
	glm::mat4 m_transform;
	Mesh* m_mesh;
	aie::ShaderProgram* m_shader;

	float m_time = 0;
	float m_shaderScaling1;
	float m_shaderScaling2;
	float m_shaderScaling3;

};

