#pragma once
#include "GameObject.h"



namespace Engine
{
	class CTransform;
	class CTerrainTex;
	class CTexture;
}

class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();


public:
	void	Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }
	void	Set_TextureIndex(_int iIndex) { m_iTextureIndex = iIndex; }

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	HRESULT				SetUp_Material(void);

private:
	CTerrainTex*		m_pBufferCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	
	_vec3				m_vDirection;
	_bool				m_bWireFrame = false;
	_int				m_iTextureIndex = 0;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};