#pragma once
#include "Scene.h"
#include "Engine_Include.h"
#include "Loading.h"

class CLoading_Scene : public CScene
{
private:
	explicit CLoading_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading_Scene();

public:
	HRESULT		Ready_Scene(LOADINGID eID);
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT		Ready_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Proto();

public:
	static CLoading_Scene* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual	void	Free();

private:
	CLoading*		m_pLoading = nullptr;
	CScene*		m_pScene = nullptr;
	LOADINGID	m_eID;
};

