#pragma once

namespace Can
{
	class ShadowFrameBuffer
	{
	public:
		ShadowFrameBuffer(int width, int height);
		~ShadowFrameBuffer();
	public:
		void Init();

		void Bind();
		void Unbind();

		inline unsigned int GetShadowMap() { return shadowMap; }
	private:
		const int width;
		const int height;
		unsigned int fbo;
		unsigned int shadowMap;
	};
}