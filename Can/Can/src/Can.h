#pragma once
#include "canpch.h"

#include "Can/Application.h"
#include "Can/Instrumentor.h"
#include "Can/Input.h"
#include "Can/KeyCodes.h"
#include "Can/MouseCodes.h"
#include "Can/Random.h"
#include "Can/AssetLoader.h"
#include "Can/Unordered_Array.h"

#include "Can/Core/TimeStep.h"

#include "Can/Layers/Layer.h"
#include "Can/Layers/ImGuiLayer.h"

#include "Can/Renderer/Prefab.h"
#include "Can/Renderer/Object.h"
#include "Can/Renderer/Buffer.h"
#include "Can/Renderer/Framebuffer.h"
#include "Can/Renderer/Shader.h"
#include "Can/Renderer/Texture.h"
#include "Can/Renderer/Renderer.h"
#include "Can/Renderer/Renderer2D.h"
#include "Can/Renderer/Renderer3D.h"

#include "Can/Camera/Camera.h"
#include "Can/Camera/Orthographic_Camera_Controller.h"

#include "Can/Scene/Panel.h"
#include "Can/Scene/Scene.h"
#include "Can/Scene/Button.h"
#include "Can/Scene/ScrollBar.h"
#include "Can/Scene/ScrollView.h"
#include "Can/Scene/Components/ParentComponent.h"
#include "Can/Scene/Components/TransformComponent.h"
#include "Can/Scene/Components/SpriteRendererComponent.h"
#include "Can/Scene/Components/DisabledComponent.h"
#include "Can/Scene/Components/HiddenComponent.h"
#include "Can/Scene/Components/IgnoreCollisionComponent.h"
#include "Can/Scene/Components/IgnoreScrollingComponent.h"
#include "Can/Scene/Components/ChildrenComponent.h"
#include "Can/Scene/Components/OnClickCallbackComponent.h"
#include "Can/Scene/Components/OnDragCallbackComponent.h"
#include "Can/Scene/Components/OnChangeCallbackComponent.h"


#include "Platform/OpenGl/OpenGLBuffer.h"
#include "Platform/OpenGl/OpenGLTexture.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
