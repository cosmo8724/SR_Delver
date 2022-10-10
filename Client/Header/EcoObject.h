#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CEcoObject : public CGameObject
{
private:
	explicit CEcoObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEcoObject();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);


public:
	static CEcoObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

private:
	CRcTex*				m_pBufferCom	= nullptr;
	CTransform*			m_pTransCom	= nullptr;
	CTexture*				m_pTextureCom	= nullptr;
};

