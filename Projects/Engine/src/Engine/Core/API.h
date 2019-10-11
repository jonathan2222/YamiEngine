#pragma once

namespace ym
{
	class API
	{
	public:
		static API* get();

		virtual void init() = 0;
		virtual void destroy() = 0;
	};
}
