#pragma once
#include "Engine_Include.h"

class CMapTool
{
public:
	CMapTool(LPDIRECT3DDEVICE9 pGraphicDev);
	~CMapTool();

public:
	HRESULT		MapTool_Window(const _float& fTimeDelta);
	HRESULT		BlockMapTool_Window(const _float& fTimeDelta);
	void		Get_MapInfo(_int* Width, _int* Depth, _int* Interval) { *Width = m_iWidth; *Depth = m_iDepth; *Interval = m_iInterval; }

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	_int	m_iWidth = 50;
	_int	m_iDepth = 50;
	_int	m_iInterval = 1;
	_int	m_iBlockCnt = 0;

	vector<TCHAR*>	m_vecObjTags;
};

