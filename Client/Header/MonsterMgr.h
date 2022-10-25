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

	HRESULT					Add_GameObject(CLayer* pLayer, const _tchar* szFilePath = L"");

	HRESULT					Add_GameObject(const _tchar* objTag, _vec3 vPos);

public:
	inline virtual void		Free(void);

public:
	vector<CGameObject*>*	Get_Monster() { return &m_vecMonster; }
	void					Add_Monster(CGameObject* pMonster) { m_vecMonster.push_back(pMonster); }

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	vector<CGameObject*>	m_vecMonster;
	vector<TCHAR*>			m_vecObjTags;
};

