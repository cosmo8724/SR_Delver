#pragma once

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight :	public CBase
{
public:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight();
	
public:
	HRESULT	Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	_uint	Get_Index() { return m_iIndex; }
	void	Set_Pos(_vec3 vPos) { m_tLightInfo.Position = vPos; m_pGraphicDev->SetLight(m_iIndex, &m_tLightInfo);}
	void	Set_Coloor(D3DXCOLOR tColor) { 
		m_tLightInfo.Diffuse = tColor;
		m_tLightInfo.Ambient = tColor;
		m_pGraphicDev->SetLight(m_iIndex, &m_tLightInfo); }

	void	Update_Info(const D3DLIGHT9* pLightInfo)
	{
		m_pGraphicDev->SetLight(m_iIndex, pLightInfo);
	}
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	D3DLIGHT9				m_tLightInfo;
	_uint					m_iIndex;

public:
	static CLight*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
						const D3DLIGHT9* pLightInfo, 
						const _uint& iIndex);

private:
	virtual void	Free(void);
};

END
