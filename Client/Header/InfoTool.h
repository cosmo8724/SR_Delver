#pragma once
#include "Engine_Include.h"

class CInfoTool
{
public:
	CInfoTool(LPDIRECT3DDEVICE9 pDevice);
	~CInfoTool();

public:
	HRESULT		InfoTool_Window(const _float& fTimeDelta);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;

	_int						m_iType = 0;
	CINFO						m_tCInfo;
	
};