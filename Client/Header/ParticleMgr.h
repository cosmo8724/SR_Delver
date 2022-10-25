#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
END

class CParticleMgr : public CBase
{
	DECLARE_SINGLETON(CParticleMgr)
private:
	CParticleMgr();
	~CParticleMgr();

public:
	void			Ready_ParticleMgr(LPDIRECT3DDEVICE9 pGraphicDev);

	HRESULT			Ready_Proto();

	HRESULT			Add_GameObject(CLayer* pLayer);

	void			Call_Particle(PTYPE eType, PTEXTUREID eTex);

	void			Collect_Particle(_int iIdx);

	void			Set_Info(CGameObject* pObj
		, _int		_maxParticles						
		, _float	_fSize								
		, _vec3		_vVelocity = { 1.f,1.f,1.f }		
		, _float	_fLifeTime = 1.f					
		, D3DXCOLOR _tColor = { 1.f, 1.f, 1.f, 1.f }	
		, _float	_fFrameSpeed = 1.f					
		, _bool		_bFrameRepeat = false				
		, _bool		_bRand = false						
		);


	void		Add_Info_Spot(_bool bFrameMove=false, _bool bRandFrame=false)
	{
		m_bFrameMove = bFrameMove;
		m_bFrameRand = bRandFrame;
	}


	void			Add_Info_Circling(
		_bool			_bFrameMove = true
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

	void			Add_Info_Snow(BDBOX _bdBox)
	{
		m_tPInfo.tBdBox = _bdBox;
	}


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

