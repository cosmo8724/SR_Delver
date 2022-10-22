#pragma once
#include "Engine_Include.h"

class CInfoTool
{
public:
	CInfoTool(LPDIRECT3DDEVICE9 pGraphicDev);
	~CInfoTool();

public:
	HRESULT		InfoTool_Window(const _float& fTimeDelta);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	vector<TCHAR*>			m_vecObjTags;
	
	_uint		m_iGreenSlimeCnt = 0;
	_uint		m_iPinkSlimeCnt = 0;
	_uint		m_iFistCnt = 0;
	_uint		m_iBlueBatCnt = 0;
	_uint		m_iBrownBatCnt = 0;
	_uint		m_iStickCnt = 0;
	_uint		m_iLeafCnt = 0;
	_uint		m_iSkeletonGhostCnt = 0;
	_uint		m_iGreenSpiderCnt = 0;
	_uint		m_iMimicCnt = 0;
	_uint		m_iOctopusCnt = 0;
	_uint		m_iSongBossCnt = 0;
};