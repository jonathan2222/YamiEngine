#pragma once

#include "Engine/Core/Layer.h"

#include "Engine/Core/Camera.h"
#include "Engine/Core/Graphics/UniformBuffer.h"
#include "Engine/Core/Graphics/Texture.h"

class SandboxLayer2 : public ym::Layer
{
public:
	struct MatrixBuffer
	{
		glm::mat4 world;
		glm::mat4 view;
		glm::mat4 projection;
	};

	void onStart() override;
	void onUpdate(float dt) override;
	void onRender() override;
	void onRenderImGui() override;
	void onQuit() override;

private:
	float m_defaultSpeed;
	float m_factor;
	float m_defaultRotSpeed;

	MatrixBuffer m_matrixBuffer;
	ym::UniformBuffer* m_uniformBuffer;
	ym::Camera* m_camera;
	ym::Shader* m_shader;
	glm::mat4 m_world;
	ym::Model m_model;

	ym::Texture* m_texture;
	ym::Sampler m_sampler;
};