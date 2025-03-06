#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>

#include <memory>
#include <functional>
#include <algorithm>

#include <array>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <queue>
#include <stack>

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <conio.h>
#include <optional>

#include "d3dx12.h"
#include <d3d12.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirecTXTex.h>
#include <DirecTXTex.inl>
#include <dxgi1_4.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>