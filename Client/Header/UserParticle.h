#pragma once
#include "PSystem.h"
class CUserParticle : public CPSystem
{
private:
	explicit CUserParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUserParticle();

public:
	// �ʼ��� �����Ǿ�� �ϴ� �׸��
	inline void		Set_ParticleCnt(_int _iCnt) { m_maxParticles = _iCnt; }
	inline void		Set_Settings()
	{
		removeAllParticles();

		for (int i = 0; i < m_maxParticles; ++i)
			addParticle();
	}
	inline  void	Set_PointSize(_float _fSize) { m_fSize = _fSize; }
	inline	void	Set_bdBox(_vec3 vMin, _vec3 vMax)	{ m_bdBox.vMin = vMin; m_bdBox.vMax = vMax; }
	inline	void	Set_bdBoxMin(_vec3 vMin) { m_bdBox.vMin = vMin; }
	inline	void	Set_bdBoxMax(_vec3 vMax) { m_bdBox.vMax = vMax; }
	inline	void	Set_Play(_bool bPlay) { m_bPlay = bPlay;   if (m_eType == PTYPE_SNOW) removeAllParticles(); }
	inline	void	Set_Velocity(_float fVel) { m_fVelocityMulti = fVel; }
	inline  void	Set_LifeTime(_float fLife) { m_Attribute.fLifeTime = fLife; }
	inline  void	Set_Accel(_vec3 vAcc) { m_Attribute.vAcceleration = vAcc; }
	inline	void	Set_Color(D3DCOLOR color) { m_Attribute.tColor = color; }
	inline	void	Set_FrameSpeed(_float fSpeed) { m_fFrameSpeed = fSpeed; }
	inline  void	Set_Type(PTYPE eType) { m_eType = eType; }
	inline  void	Is_ColorRand(_bool bRand) { m_bRand = bRand; }
	inline	void	Is_FrameRepeat(_bool bRepeat) { m_bFrameRepeat = bRepeat; }
	
	void			Set_Particle_Texture(_int iSelected);
	void			Play_Particle();

public:
	void			Set_Index(_int iIdx) { m_iIndex = iIdx; }
	void			Set_Use(_bool bUse) { m_bUse = bUse; }
	void			Set_Particle(PTYPE _eType);	// ��ƼŬ�� Ÿ�Կ� ���� �ʿ��� ��� ����
	void			Set_Texture(PTEXTUREID eTex);
	void			Set_Target(CGameObject* pObj) { m_pTarget = pObj; }
	void			Set_Information(_bool _bUse, _int _iIdx, CGameObject* _pObj, ATTRIBUTE _att, PINFO _pInfo
									, _float _fFrameSpeed, _bool _bFrameRepeat, _bool _bRand); 
	void			ReUse();	// ��ƼŬ�� ������ Ǯ�� ���ư��� ���½�����.

public:
	virtual HRESULT			Ready_Object(void);
	virtual _int			Update_Object(const _float& fTimeDelta);
	virtual void			LateUpdate_Object(void);
	virtual void			Render_Obejct(void);

public:
	virtual void			resetParticle(ATTINFO* attribute);
	virtual void			update(_float fTimeDelta);
	virtual void			preRender();
	virtual void			postRender();

private:
	HRESULT				Add_Component(void);

public:
	static CUserParticle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free(void) override;

private:
	_bool		m_bPlay = true;
	_bool		m_bRand = false;
	_bool		m_bFrameRepeat = false;
	_float		m_fFrameSpeed = 1.f;
	PTYPE		m_eType = PTYPE_END;
	_float		m_fVelocityMulti = 1.f;

	// Ǯ ������
	_int		m_iIndex = -1;
	_bool		m_bUse = false;
	_bool		m_bReady = false;

	// �߰��� ����
	CGameObject*	m_pTarget = nullptr;



};

