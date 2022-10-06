#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

// 모든 아이템들이 가진 공통적인 특성
class ENGINE_DLL CItem : public CGameObject
{

protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev) {};
	virtual ~CItem() {};

public:
	virtual HRESULT Ready_Object(void) { return S_OK; }
	virtual _int	Update_Object(const _float& fTimeDelta) { return 0; }
	virtual void	LateUpdate_Object(void) {}
	virtual	void	Render_Obejct(void) {}

protected:
	virtual void	Free(void) {};

public:
	void		Set_Equipped() { m_eState = STATE_EQUIP; }
	wstring		Get_TextureTag() { return m_textureTag; }

protected:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransCom = nullptr;


	ITEMSTATE		m_eState = STATE_END;

	wstring			m_ObjTag;
	wstring			m_textureTag;

	// 일반공격/차징공격
	_bool			m_bClick = false;
	_bool			m_bCharge = false;
	_bool			m_bAttack = false;
	_float			m_fChargeTime = 0.f;
	_float			m_fOldChargeTime = 0.f;
	_float			m_fCoolTime = 0.f;

	_bool			m_bParticleCall = false;
};

END
