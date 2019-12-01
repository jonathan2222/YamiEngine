#pragma once

#include <string>

namespace ym
{
	class API
	{
	public:
		static API* get();

		virtual void init() = 0;
		virtual void destroy() = 0;

		struct VideoCardInfo
		{
			std::string name;
			int videoMemory;
			int systemMemory;
			int sharedSystemMemory;
		};

		VideoCardInfo& getVideoCardInfo();

	private:
		VideoCardInfo m_videoCardinfo;
	};
}
