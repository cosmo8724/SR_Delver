#pragma once
#include "GameObject.h"

BEGIN(Engine)
//class CRcTex;
class CTransform;
class CTexture;
class CShader;
class CTerrainTex;
END

class CWater : public CGameObject
{
private:
	explicit CWater(LPDIRECT3DDEVICE9 pGraphicDev, WATERTYPE eType);
	virtual ~CWater();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CTerrainTex*		m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	_vec3				m_vDirection;
	_float				m_fTime = 0.f;
	WATERTYPE			m_eType = WATER_END;

public:
	static CWater*		Create(LPDIRECT3DDEVICE9 pGraphicDev, WATERTYPE eType);
	virtual void		Free(void);

};

