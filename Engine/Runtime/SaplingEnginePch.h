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
#include <functional>
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
 * math
 */
#include "Math/Math.h"
#include "Math/Color.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "Utility/Exception.h"
#include "Utility/Log.h"
#include "Utility/Singleton.h"
#include "Utility/Time.h"
#include "Utility/Utility.h"

using SaplingEngine::Math::Color;
using SaplingEngine::Math::Matrix4x4;
using SaplingEngine::Math::Quaternion;
using SaplingEngine::Math::Vector2;
using SaplingEngine::Math::Vector3;
using SaplingEngine::Math::Vector4;