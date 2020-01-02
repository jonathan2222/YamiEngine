#include "SandboxLayer2.h"

#include <glm/gtx/rotate_vector.hpp>
#include "Engine/Core/Graphics/Vertex.h"
#include "Engine/Core/Graphics/ResourceManager.h"

struct MyVertex
{
	glm::vec3 pos;
	glm::vec2 uv;
};

void SandboxLayer2::onStart()
{
	std::vector<MyVertex> vertices =
	{
		{ glm::vec3(-0.5f,-0.5f, 0.0f), glm::vec2(0, 0), },
		{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0, 1), },
		{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1, 0), }
	};

	std::vector<unsigned int> indices = { 0, 1, 2 };

	ym::Model::Info modelInfo(ym::Topology::TRIANGLE_LIST, ym::Usage::STATIC);
	ym::AttributeLayout layout;
	layout.push(ym::Format::FLOAT_32_RGB, "POSITION");
	layout.push(ym::Format::FLOAT_32_RG, "TEXCOORD");
	m_model.setData(&(vertices[0].pos.x), (unsigned int)(sizeof(MyVertex) * vertices.size()),
		indices.data(), (unsigned int)indices.size(), layout, modelInfo);

	m_shader = ym::Shader::create();
	m_shader->load("Tests/texture", layout);

	ym::ResourceManager::Image* img = ym::ResourceManager::get().loadImage("test.png", 4);

	m_texture = ym::Texture::create();
	m_sampler.filter = ym::Sampler::Filter::MIN_NEAREST_MAG_NEAREST_MIP_LINEAR;
	m_sampler.addressMode = ym::Sampler::AddressMode::REPEAT;
	// TODO: NEED A DIFFERENT FORMAT FOR INTERNAL USE FOR DIRECTX11!!!!
	m_texture->setData(img, ym::Format::UINT_8_RGBA, m_sampler, ym::Usage::STATIC);

	ym::ResourceManager::get().freeImage(img);
	img = nullptr;

	m_camera = new ym::Camera({ 0.0f, 0.0f, 2.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, 0.01f, 100.0f, 3.1415f / 4);
	m_camera->updateView();
	m_camera->updateProj();
	m_world = glm::mat4(1.0f);
	m_world[3][3] = 1.0f;

	m_uniformBuffer = ym::UniformBuffer::create();
	m_matrixBuffer.world = glm::transpose(m_world);
	m_matrixBuffer.projection = glm::transpose(m_camera->getProj());
	m_matrixBuffer.view = glm::transpose(m_camera->getView());
	m_uniformBuffer->setShader(*m_shader, 0, 0);
	m_uniformBuffer->setData(&(m_matrixBuffer.world[0][0]), sizeof(MatrixBuffer));

	m_defaultSpeed = 2.0f;
	m_factor = 2.f;
	m_defaultRotSpeed = 3.1415f / 180.f * 90.0f;
}

void SandboxLayer2::onUpdate(float dt)
{
	if (ym::Input::get()->isKeyPressed(ym::Key::ESCAPE))
		getDisplay()->close();

	float speed = m_defaultSpeed * dt;
	if (ym::Input::get()->isKeyPressed(ym::Key::LEFT_SHIFT))
	{
		speed = m_defaultSpeed * m_factor * dt;
	}
	float rotSpeed = m_defaultRotSpeed * dt;

	if (ym::Input::get()->isKeyPressed(ym::Key::A))
	{
		glm::vec3 right = m_camera->getRight();
		m_camera->setPosition(m_camera->getPos() - right * speed);
	}
	if (ym::Input::get()->isKeyPressed(ym::Key::D))
	{
		glm::vec3 right = m_camera->getRight();
		m_camera->setPosition(m_camera->getPos() + right * speed);
	}
	if (ym::Input::get()->isKeyPressed(ym::Key::W))
	{
		glm::vec3 dir = m_camera->getDir();
		m_camera->setPosition(m_camera->getPos() + dir * speed);
	}
	if (ym::Input::get()->isKeyPressed(ym::Key::S))
	{
		glm::vec3 dir = m_camera->getDir();
		m_camera->setPosition(m_camera->getPos() - dir * speed);
	}

	if (ym::Input::get()->isKeyPressed(ym::Key::LEFT))
	{
		glm::vec3 up = m_camera->getUp();
		glm::vec3 right = m_camera->getRight();
		const glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
		right = glm::rotate(right, rotSpeed, globalUp);
		if (glm::length(up - globalUp) > 0.0001f)
			up = glm::rotate(up, rotSpeed, globalUp);
		m_camera->setOrientaion(up, right);
	}
	if (ym::Input::get()->isKeyPressed(ym::Key::RIGHT))
	{
		glm::vec3 up = m_camera->getUp();
		glm::vec3 right = m_camera->getRight();
		const glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
		right = glm::rotate(right, -rotSpeed, globalUp);
		if (glm::length(up - globalUp) > 0.0001f)
			up = glm::rotate(up, -rotSpeed, globalUp);
		m_camera->setOrientaion(up, right);
	}
	if (ym::Input::get()->isKeyPressed(ym::Key::UP))
	{
		glm::vec3 up = m_camera->getUp();
		glm::vec3 right = m_camera->getRight();
		up = glm::rotate(up, rotSpeed, right);
		m_camera->setOrientaion(up, right);
	}
	if (ym::Input::get()->isKeyPressed(ym::Key::DOWN))
	{
		glm::vec3 up = m_camera->getUp();
		glm::vec3 right = m_camera->getRight();
		up = glm::rotate(up, -rotSpeed, right);
		m_camera->setOrientaion(up, right);
	}

	if (ym::Input::get()->isMBPressed(ym::MB::LEFT))
	{
		glm::vec2 pos = ym::Input::get()->getMousePos();
		YM_LOG_INFO("Mouse pos: {}, {}", pos.x, pos.y);
	}
}

void SandboxLayer2::onRender()
{
	m_matrixBuffer.world = m_world;
	m_matrixBuffer.projection = m_camera->getProj();
	m_matrixBuffer.view = m_camera->getView();
	m_uniformBuffer->updateData(&(m_matrixBuffer.world[0][0]), sizeof(MatrixBuffer), 0);
	m_uniformBuffer->bind(YM_SHADER_TYPE_VERTEX);

	m_shader->bind();
	m_shader->setTexture("tex", m_texture, m_sampler, 0);

	getRenderer()->draw(&m_model, m_shader);
}

void SandboxLayer2::onRenderImGui()
{
}

void SandboxLayer2::onQuit()
{
	delete m_camera;
	delete m_shader;
	delete m_uniformBuffer;
	delete m_texture;
}
