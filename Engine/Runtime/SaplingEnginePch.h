#pragma once

/*
 * Windows
 */
#include <Windows.h>
#include <windowsx.h>
#include <wrl.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/*
 * crt
 */
#include <crtdbg.h>

#if defined(DEBUG) || defined(_DEBUG)
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

/*
* direct
*/
#include "Application/DirectX12/D3D12Header.h"
#include "Application/DirectX12/D3D12UploadBuffer.h"

/*
 * math
 */
#include "Math/Color.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "Utility/Log.h"
#include "Utility/Singleton.h"
#include "Utility/Time.h"

/*
 * XML
 */
#include "Utility/RapidXml/rapidxml.hpp"
#include "Utility/RapidXml/rapidxml_utils.hpp"
#include "Utility/RapidXml/rapidxml_print.hpp"

using SaplingEngine::Math::Color;
using SaplingEngine::Math::Matrix4x4;
using SaplingEngine::Math::Quaternion;
using SaplingEngine::Math::Vector2;
using SaplingEngine::Math::Vector3;
using SaplingEngine::Math::Vector4;