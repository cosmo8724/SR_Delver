#pragma once
#include "Scene.h"

class CIntro :	public CScene
{
private:
	explicit CIntro(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CIntro();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);

public:
	static CIntro*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);

private:
	vector<TCHAR*>	m_vecObjTags;
};

