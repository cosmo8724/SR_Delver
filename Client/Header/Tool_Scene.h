#pragma once
#include "Engine_Include.h"
#include "Scene.h"

class CTool_Scene : public CScene
{
private:
	explicit CTool_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTool_Scene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT		Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT		Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT		Ready_Light();

public:
	static CTool_Scene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

