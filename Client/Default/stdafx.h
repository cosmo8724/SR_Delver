// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3d9.h>
#include <d3dx9.h>

#include "../Default/ImGui/imgui.h"
#include "../Default/ImGui/imgui_impl_dx9.h"
#include "../Default/ImGui/imgui_impl_win32.h"

#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <process.h>
#include <fstream>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern LPDIRECT3DDEVICE9	g_GraphicDev;
extern bool			g_bIsTool;
extern bool			g_bIsRenderInstancing;
extern bool			g_bOpenAtOnce;
extern D3DXVECTOR3	g_vPlayerPos;
extern float		g_fAmbient;
extern int			g_iMoney;
extern bool			g_bBoss;
extern bool			g_bShopOpen;
extern bool			g_bInvOpen;

using namespace std;
