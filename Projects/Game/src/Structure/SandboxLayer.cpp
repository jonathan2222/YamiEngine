#include "SandboxLayer.h"

#include <glm/gtx/rotate_vector.hpp>
#include "Engine/Core/Graphics/Vertex.h"

void SandboxLayer::onStart()
{
	std::vector<ym::Vertex> vertices =
	{
		{ glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0, 0, 0), },
		{ glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0, 0, 1), },
		{ glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0, 1, 0), },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 1), },

		{ glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1, 0, 0), },
		{ glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1, 0, 1), },
		{ glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1, 1, 0), },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 1, 1), },
	};

	std::vector<unsigned int> indices =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5
	};

	ym::Model::Info modelInfo(ym::Topology::TRIANGLE_LIST, ym::Usage::STATIC);
	ym::AttributeLayout layout;
	layout.push(ym::Format::FLOAT_32_RGB, "POSITION");
	layout.push(ym::Format::FLOAT_32_RGB, "COLOR");
	m_model.setData(&(vertices[0].pos.x), (unsigned int)(sizeof(ym::Vertex) * vertices.size()),
		indices.data(), (unsigned int)indices.size(), layout, modelInfo);

	m_shader = ym::Shader::create();
	m_shader->load("Test", layout);

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

void SandboxLayer::onUpdate(float dt)
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

void SandboxLayer::onRender()
{
	m_matrixBuffer.world = m_world;
	m_matrixBuffer.projection = m_camera->getProj();
	m_matrixBuffer.view = m_camera->getView();
	m_uniformBuffer->updateData(&(m_matrixBuffer.world[0][0]), sizeof(MatrixBuffer), 0);
	m_uniformBuffer->bind(YM_SHADER_TYPE_VERTEX);
	getRenderer()->draw(&m_model, m_shader);
}

void SandboxLayer::onRenderImGui()
{
	// If ImGUI/activate is true than this will result in irregular behaviour of the win32 window's menue buttons!

	static bool show_demo_window = false;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", m_clearColor);		// Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void SandboxLayer::onQuit()
{
	delete m_camera;
	delete m_shader;
	delete m_uniformBuffer;
}
