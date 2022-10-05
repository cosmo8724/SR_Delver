#pragma once
#include "Engine_Include.h"
#include "UserParticle.h"

class CParticleTool
{
public:
	CParticleTool(LPDIRECT3DDEVICE9 pDevice);
	~CParticleTool();


public:
	void	ParticleTool_Window(const _float& fTimeDelta);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	_int						m_iType = 0;
	ATTINFO					m_tAttribute;
	PINFO						m_tInfo;

public:
	CUserParticle*			m_pParticle;
};