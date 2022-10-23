#pragma once
#include "Base.h"
#include "Light.h"

BEGIN(Engine)
class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr(void);
	virtual ~CLightMgr();

public:
	HRESULT	Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
		const D3DLIGHT9* pLightInfo, const _uint& iIndex);

	void	Update_Light(const _uint& iIndex, const D3DLIGHT9* pLightInfo) { m_LightList[iIndex]->Update_Info(pLightInfo); }
	void	Update_Pos(const _uint& iIndex, _vec3 vPos) { m_LightList[iIndex]->Set_Pos(vPos); }
	void	Update_Color(const _uint& iIndex, D3DXCOLOR tColor) { m_LightList[iIndex]->Set_Coloor(tColor); }

private:
	vector<CLight*>				m_LightList;

private:
	virtual void	Free(void);
};

END
