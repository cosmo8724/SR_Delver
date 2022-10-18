#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
END

// ��ƼŬ�� ������ƮǮó�� �̸� �����Ͽ� ����
class CParticleMgr : public CBase
{
	DECLARE_SINGLETON(CParticleMgr)
private:
	CParticleMgr();
	~CParticleMgr();

public:
	void			Ready_ParticleMgr(LPDIRECT3DDEVICE9 pGraphicDev);

	// ��ƼŬ Proto ������Ʈ ����(�ַ� �ؽ�ó)
	HRESULT			Ready_Proto();

	// �̸� ������Ʈ�� �����ص�.
	HRESULT			Add_GameObject(CLayer* pLayer);

	// ��ƼŬ ȣ�� (changed)
	void			Call_Particle(PTYPE eType, PTEXTUREID eTex);

	// ����� ���� ��ƼŬ ȸ�� (dispose)
	void			Collect_Particle(_int iIdx);

	// ȣ�� �� ��ƼŬ ���� ����
	void			Set_Info(CGameObject* pObj
		, _int		_maxParticles						// �ִ� ��ƼŬ ��
		, _float	_fSize								// ��� ��ƼŬ�� ũ��
		, _vec3		_vVelocity = { 1.f,1.f,1.f }		// �ӵ�(Spot Type�� ��� vRight, vUp, vLook ���κ����� �Ÿ�)
		, _float	_fLifeTime = 1.f					// ��ƼŬ �Ҹ���� �����Ǵ� �ð�
		, D3DXCOLOR _tColor = { 1.f, 1.f, 1.f, 1.f }		// ����
		, _float	_fFrameSpeed = 1.f					// ������ �ӵ�
		, _bool		_bFrameRepeat = false				// ������ �ݺ���� ����
		, _bool		_bRand = false						// �� ���� ����
		);


	void		Add_Info_Spot(_bool bFrameMove=false, _bool bRandFrame=false)
	{
		m_bFrameMove = bFrameMove;
		m_bFrameRand = bRandFrame;
	}


	void			Add_Info_Circling(
		_bool		_bFrameMove = true
		, _float		_fFrame = 0
		, _float		_fDist = 1.f
		, _float		_fAngleSpeed= 1.f
		)
	{
		m_bFrameMove = _bFrameMove;
		m_fFrame = _fFrame;
		m_fDist = _fDist;
		m_fAngleSpeed = _fAngleSpeed;
	}


	// �ٿ�� �ڽ� ���� ( �ʿ��� ��� )
	//void			Set_BoundingBox(BDBOX _bdBox);


public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	_int					m_maxParticle;
	vector<TCHAR*>			m_vecObjTag;		// names..
	vector<CGameObject*>	m_ParticlePool;

	queue<_int>				m_IdxQue;


	ATTRIBUTE		m_tAttribute;
	PINFO			m_tPInfo;
	CGameObject*	m_pTarget;

	_float			m_fFrameSpeed = 1.f;
	_bool			m_bFrameRepeat = false;
	_bool			m_bRand = false;

	_bool			m_bFrameRand = false;
	_bool			m_bFrameMove = true;
	_float			m_fFrame = 0.f;
	_float			m_fDist = 1.f;
	_float			m_fAngleSpeed = 1.f;
};

