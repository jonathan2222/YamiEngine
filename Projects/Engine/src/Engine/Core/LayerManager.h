#pragma once

#include <vector>

namespace ym
{
	class API;
	class IApp;
	class Layer;
	class LayerManager
	{
	public:
		LayerManager() = default;
		static LayerManager* get();

		void setApp(IApp* app);

		void push(Layer* layer);
		void pop();

		void onStart();
		void onUpdate(float dt);
		void onRender();
		void onRenderImGui();
		void onQuit();

	private:
		virtual ~LayerManager();

		std::vector<Layer*> m_layers;
		IApp* m_app;
	};
}