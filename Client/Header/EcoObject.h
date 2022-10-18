#pragma once
#include "GameObject.h"
#include "Texture.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CEcoObject : public CGameObject
{
protected:
	explicit CEcoObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEcoObject(const CEcoObject& rhs);
	virtual ~CEcoObject();

public:
	CTexture*	Get_TextureCom() { return m_pTextureCom; }
	const _uint&	Get_CurrentTexture() { return m_iTexture; }
	void			Set_CurrentTexture(_uint iTexture) { m_iTexture = iTexture; }
	void	PlusTexture() { if (m_iTexture < m_pTextureCom->Get_FrameEnd()) m_iTexture++; }
	void	MinusTexture() { if (m_iTexture > 0) m_iTexture--; }
	ECOOBJTYPE		Get_Type() { return m_eType; }

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

	static CEcoObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

protected:
	virtual void		Billboard();

private:
	HRESULT				Add_Component(void);

protected:
	CRcTex*			m_pBufferCom	= nullptr;
	CTransform*		m_pTransCom	= nullptr;
	CTexture*			m_pTextureCom	= nullptr;
	_vec3				m_vPos;
	//_bool				m_bDead			= false;
	_matrix				m_matWorld;
	_int				m_iTransparency	= 255;
	_uint				m_iTexture = 0;
	_bool				m_bClone = false;
	_matrix				m_matOldBill;

public:
	ECOOBJTYPE		m_eType = ECO_END;
};

