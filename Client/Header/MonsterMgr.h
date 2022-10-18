#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CMonster;
END

class CMonsterMgr : public CBase
{
	DECLARE_SINGLETON(CMonsterMgr)

private:
	CMonsterMgr();
	~CMonsterMgr();

public:
	void					Ready_MonsterMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT					Ready_Proto();

	HRESULT					Add_GameObject(CLayer* pLayer);

public:
	inline virtual void		Free(void);

public:
	vector<CGameObject*>*	Get_Monster() { return &m_vecMonster; }


private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	vector<CGameObject*>	m_vecMonster;
};

