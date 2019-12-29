#include "stdafx.h"
#include "LayerManager.h"

#include "Layer.h"
#include "IApp.h"

ym::LayerManager* ym::LayerManager::get()
{
	static LayerManager manager;
	return &manager;
}

void ym::LayerManager::setApp(IApp* app)
{
	m_app = app;
}

ym::LayerManager::~LayerManager()
{
	for (Layer*& layer : m_layers)
		delete layer;
	m_layers.clear();
}

void ym::LayerManager::push(Layer* layer)
{
	layer->init(m_app->m_api, m_app->m_input, m_app->m_display, m_app->m_renderer);
	m_layers.push_back(layer);
}

void ym::LayerManager::pop()
{
	Layer* layer = m_layers.back();
	layer->onQuit();
	delete layer;
	m_layers.pop_back();
}

void ym::LayerManager::onStart()
{
	for (Layer*& layer : m_layers)
		layer->onStart();
}

void ym::LayerManager::onUpdate(float dt)
{
	// Update the active layer
	m_layers.back()->onUpdate(dt);
}

void ym::LayerManager::onRender()
{
	// Render the active layer
	m_layers.back()->onRender();
}

void ym::LayerManager::onRenderImGui()
{
	// Render imGui on the active layer
	m_layers.back()->onRenderImGui();
}

void ym::LayerManager::onQuit()
{
	for (Layer*& layer : m_layers)
		layer->onQuit();
}
