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
	for (Layer*& layer : m_layers)
		layer->onUpdate(dt);
}

void ym::LayerManager::onRender()
{
	for (Layer*& layer : m_layers)
		layer->onRender();
}

void ym::LayerManager::onRenderImGui()
{
	for (Layer*& layer : m_layers)
		layer->onRenderImGui();
}

void ym::LayerManager::onQuit()
{
	for (Layer*& layer : m_layers)
		layer->onQuit();
}
