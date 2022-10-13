#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CUI;
END

class CUIMgr : public CBase
{
	DECLARE_SINGLETON(CUIMgr)

private:
	CUIMgr();
	~CUIMgr();

public:
	void					Ready_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT					Ready_Proto();

	HRESULT					Add_GameObject(CLayer* pLayer);

public:
	inline virtual void		Free(void);

public:
	vector<CGameObject*>*	Get_UI() { return &m_vecUI; }

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	vector<CGameObject*>	m_vecUI;
};

