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

	void	Update_Light(const _uint& iIndex, const D3DLIGHT9* pLightInfo);

	void	Update_Pos(const _uint& iIndex, _vec3 vPos);

private:
	list<CLight*>				m_LightList;

private:
	virtual void	Free(void);
};

END
