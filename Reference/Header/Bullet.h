#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTransform;
class CTexture;

class ENGINE_DLL CBullet : public  CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_Object(void) { return S_OK; }
	virtual _int	Update_Object(const _float& fTimeDelta) { return 0; }
	virtual void	LateUpdate_Object(void) {}
	virtual	void	Render_Obejct(void) {}

protected:
	virtual void	Reset() {};  //Ǯ�� ���ư� ������Ʈ�� �����ϱ� ���� ���� ������ �����Ѵ�.
	virtual void	Free(void);

public:
	void			Set_Fire(_bool _bFire) { m_bFire = _bFire; m_bDead = false; }
	_bool			Is_Fired() { return m_bFire; }
	void			Set_Index(_int iIdx) { m_iIndex = iIdx; }
	_int			Get_BulletAttack() { return m_tInfo.iAttack; }

protected:
	MONTSERINFO		m_tInfo;

	//CCubeTex*		m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	_bool			m_bFire = false;
	//_bool			m_bDead = false;
	_float			m_fLifeTime = 0.f;

	_int			m_iIndex = -1;// �Ŵ��� �ε��� ȸ����
};
END

