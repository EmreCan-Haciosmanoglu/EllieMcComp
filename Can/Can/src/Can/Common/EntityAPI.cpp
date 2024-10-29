#include "canpch.h"
//#include "Can/Common/CommonHeaders.h"
//#include "Can/Common/Id.h"
//#include "Can/Components/Entity.h"
//#include "Can/Components/Transform.h"
//#include "Can/Components/Script.h"
//
//using namespace Can;
//
//namespace
//{
//	struct transform_component
//	{
//		f32 position[3];
//		f32 rotation[3];
//		f32 scale[3];
//
//		transform::init_info to_init_info()
//		{
//			using namespace DirectX;
//			transform::init_info info{};
//			memcpy(info.position, position, sizeof(position));
//			memcpy(info.scale, scale, sizeof(scale));
//			XMFLOAT3A rot{ rotation };
//			XMVECTOR quat{ XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3A(&rot)) };
//			XMFLOAT4A rot_quat{};
//			XMStoreFloat4A(&rot_quat, quat);
//			memcpy(info.rotation, &rot_quat.x, sizeof(info.rotation);
//			return info;
//		}
//	};
//
//	struct script_component
//	{
//		script::detail::script_creator script_creator;
//
//		script::init_info to_init_info()
//		{
//			script::init_info info{};
//			info.script_creator = script_creator;
//			return info;
//		}
//	};
//}