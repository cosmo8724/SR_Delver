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

	_uint		m_iStoneCnt = 0;
	_uint		m_iGrassCnt = 0;
	_uint		m_iTreeCnt = 0;
	_uint		m_iJarCnt = 0;
	_uint		m_iJamCnt = 0;
	_uint		m_iLadderCnt = 0;
};

