#pragma once
#include "GameObject.h"

class CCullingMgr : public CBase
{
	DECLARE_SINGLETON(CCullingMgr)

private:
	CCullingMgr();
	~CCullingMgr();

public:
	void	Ready_CullingMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	_int	Update(const _float& fTimeDelta);
	//void	LateUpdate(void);

public:
	_bool	Is_Inside(CGameObject* pObj);

public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_vec3		vertexArr[8];
	D3DXPLANE	planeArr[6];
};

