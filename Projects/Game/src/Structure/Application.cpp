#include "Application.h"

#include <Engine/Core/Display.h>

// Only for test, should be moved.
#ifdef YAMI_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
#include <gl/GL.h>

#include "Engine/Core/Input/Input.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Input/Config.h"
#include "Engine/Core/Graphics/Renderer.h"
#include "Engine/Core/Graphics/Model.h"
#include "Engine/Core/Camera.h"

#include <glm/gtx/rotate_vector.hpp>

Application::Application(ym::DisplayDesc& displayDescriptor) : IApp(displayDescriptor)
{
}

Application::~Application()
{
	delete m_display;
}

void Application::processArguments(int argc, char* argv[])
{	
}

void Application::run()
{
	ym::Model* model = ym::Model::create();
	/*std::vector<ym::Vertex> vertices = 
	{ 
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)}, 
		{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}
	};
	std::vector<unsigned int> indices = {0, 1, 2};*/
	std::vector<ym::Vertex> vertices =
	{
		{ glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0, 0, 0), },
		{ glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0, 0, 1), },
		{ glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0, 1, 0), },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 1), },

		{ glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(1, 0, 0), },
		{ glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(1, 0, 1), },
		{ glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(1, 1, 0), },
		{ glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(1, 1, 1), },
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
	model->setData(vertices, indices);

	m_renderer->initShader(L"./Resources/Shader.vs", L"./Resources/Shader.ps");

	ym::Camera camera({0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, 0.01f, 100.0f, 3.1415f / 4);
	camera.updateView();
	camera.updateProj();
	glm::mat4 world(1.0f);
	world[3][3] = 1.0f;

	float defaultSpeed = 0.005f;
	float factor = 2.f;
	float rotSpeed = 3.1415f / 180.f * 0.2f;

	while (!m_display->shouldClose())
	{
		m_display->pollEvents();

		if (ym::Input::get()->isKeyPressed(ym::Key::ESCAPE))
			m_display->close();

		float speed = defaultSpeed;
		if (ym::Input::get()->isKeyPressed(ym::Key::LEFT_SHIFT))
		{
			speed = defaultSpeed*factor;
		}

		if (ym::Input::get()->isKeyPressed(ym::Key::A))
		{
			glm::vec3 right = camera.getRight();
			camera.setPosition(camera.getPos() - right*speed);
		}
		if (ym::Input::get()->isKeyPressed(ym::Key::D))
		{
			glm::vec3 right = camera.getRight();
			camera.setPosition(camera.getPos() + right * speed);
		}
		if (ym::Input::get()->isKeyPressed(ym::Key::W))
		{
			glm::vec3 dir = camera.getDir();
			camera.setPosition(camera.getPos() + dir * speed);
		}
		if (ym::Input::get()->isKeyPressed(ym::Key::S))
		{
			glm::vec3 dir = camera.getDir();
			camera.setPosition(camera.getPos() - dir * speed);
		}

		if (ym::Input::get()->isKeyPressed(ym::Key::LEFT))
		{
			glm::vec3 up = camera.getUp();
			glm::vec3 right = camera.getRight();
			const glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
			right = glm::rotate(right, rotSpeed, globalUp);
			if(glm::length(up-globalUp) > 0.0001f)
				up = glm::rotate(up, rotSpeed, globalUp);
			camera.setOrientaion(up, right);
		}
		if (ym::Input::get()->isKeyPressed(ym::Key::RIGHT))
		{
			glm::vec3 up = camera.getUp();
			glm::vec3 right = camera.getRight();
			const glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
			right = glm::rotate(right, -rotSpeed, globalUp);
			if (glm::length(up - globalUp) > 0.0001f)
				up = glm::rotate(up, -rotSpeed, globalUp);
			camera.setOrientaion(up, right);
		}
		if (ym::Input::get()->isKeyPressed(ym::Key::UP))
		{
			glm::vec3 up = camera.getUp();
			glm::vec3 right = camera.getRight();
			up = glm::rotate(up, rotSpeed, right);
			camera.setOrientaion(up, right);
		}
		if (ym::Input::get()->isKeyPressed(ym::Key::DOWN))
		{
			glm::vec3 up = camera.getUp();
			glm::vec3 right = camera.getRight();
			up = glm::rotate(up, -rotSpeed, right);
			camera.setOrientaion(up, right);
		}

		if (ym::Input::get()->isMBPressed(ym::MB::LEFT))
		{
			glm::vec2 pos = ym::Input::get()->getMousePos();
			YM_LOG_INFO("Mouse pos: {}, {}", pos.x, pos.y);
		}
		
		m_renderer->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

		model->bind();
		m_renderer->bindShader(world, camera.getView(), camera.getProj());
		model->draw();

		m_renderer->endScene();
	}

	model->destroy();
	delete model;
}
