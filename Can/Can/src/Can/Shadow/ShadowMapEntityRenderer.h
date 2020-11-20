#pragma once
#include "Can/Renderer/Shader.h"
#include <glm/glm.hpp>
namespace Can
{
	class ShadowMapEntityRenderer
	{
	public:
		ShadowMapEntityRenderer(const Ref<Shader>& shader, const glm::mat4& pv);

		void Render();

	private:
		glm::mat4 ligthSpaceMatrix;
		Ref<Shader> shader;
	};
}