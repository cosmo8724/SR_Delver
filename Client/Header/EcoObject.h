#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CEcoObject : public CGameObject
{
protected:
	explicit CEcoObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEcoObject();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Free(void);

protected:
	virtual void		Billboard();

protected:
	CRcTex*				m_pBufferCom	= nullptr;
	CTransform*			m_pTransCom		= nullptr;
	CTexture*			m_pTextureCom	= nullptr;
	_vec3				m_vPos;
	//_bool				m_bDead			= false;
	_matrix				m_matWorld;
	_int				m_iTransparency	= 255;
};

