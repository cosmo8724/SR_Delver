#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CNPC;
END

class CNPCMgr : public CBase
{
	DECLARE_SINGLETON(CNPCMgr)

private:
	CNPCMgr();
	~CNPCMgr();

public:
	void					Ready_NPC(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT					Ready_Proto();

	HRESULT					Add_GameObject(CLayer* pLayer, LOADINGID eID);

public:
	inline virtual void		Free(void);

public:
	vector<CGameObject*>*	Get_NPC() { return &m_vecNPC; }

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	vector<CGameObject*>	m_vecNPC;
};

