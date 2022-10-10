#pragma once
#include "Engine_Include.h"

class CObjectTool
{
public:
	CObjectTool(LPDIRECT3DDEVICE9 pGraphicDev);
	~CObjectTool();

public:
	HRESULT		ObjectTool_Window(const _float& fTimeDelta);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	vector<TCHAR*>			m_vecObjTags;
};

