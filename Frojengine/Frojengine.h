#pragma once

#include <tchar.h>
#include <list>
#include <vector>
#include <unordered_map>

using namespace std;

#include <Windows.h>



///////////////////////////////////////////
//
//  Frojer Engine System Header
//
///////////////////////////////////////////

#include "FJDefine.h"
#include "FJSystemEngine.h"
#include "FJRenderingEngine.h"
#include "Device.h"
#include "FileLoader.h"
#include "Debug.h"

///////////////////////////////////////////
//
//  Frojer Engine Object Header
//
///////////////////////////////////////////

#include "IObject.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Scene.h"



///////////////////////////////////////////
//
//  Frojer Engine Object Manager Header
//
///////////////////////////////////////////

#include "SceneManager.h"



///////////////////////////////////////////
//
//  Frojer Engine Component Header
//
///////////////////////////////////////////
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"