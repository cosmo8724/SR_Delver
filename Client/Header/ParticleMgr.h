#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
END

// 파티클을 오브젝트풀처럼 미리 생성하여 관리
class CParticleMgr : public CBase
{
	DECLARE_SINGLETON(CParticleMgr)
private:
	CParticleMgr();
	~CParticleMgr();

public:
	void			Ready_ParticleMgr(LPDIRECT3DDEVICE9 pGraphicDev);

	// 파티클 Proto 컴포넌트 생성(주로 텍스처)
	HRESULT			Ready_Proto();

	// 미리 오브젝트를 생성해둠.
	HRESULT			Add_GameObject(CLayer* pLayer);

	// 파티클 호출 (changed)
	void			Call_Particle(PTYPE eType, PTEXTUREID eTex);

	// 사용이 끝난 파티클 회수 (dispose)
	void			Collect_Particle(_int iIdx);

	// 호출 전 파티클 정보 설정
	void			Set_Info(CGameObject* pObj
		, _int		_maxParticles						// 최대 파티클 수
		, _float	_fSize								// 모든 파티클의 크기
		, _vec3		_vVelocity = { 1.f,1.f,1.f }		// 속도(Spot Type의 경우 vRight, vUp, vLook 으로부터의 거리)
		, _float	_fLifeTime = 1.f					// 파티클 소멸까지 유지되는 시간
		, D3DXCOLOR _tColor = { 1.f, 1.f, 1.f, 1.f }		// 색깔
		, _float	_fFrameSpeed = 1.f					// 프레임 속도
		, _bool		_bFrameRepeat = false				// 프레임 반복재생 여부
		, _bool		_bRand = false						// 색 랜덤 여부
		);

	// Set Info for Spot Type ( test )
	//void			Set_Info_Spot(
	//								CGameObject*	pObj			= nullptr
	//								, _vec3			vDist			= { 1.f, 1.f, 1.f }	// distance from the object from (vRight, vUp, vLook)
	//								, _int			iParticleCnt	= 1.f
	//								, _float		fSize			= 1.f
	//								, _float		fLifeTime		= 1.f
	//								, D3DXCOLOR		tColor			= { 1.f, 1.f, 1.f, 1.f }
	//								, _float		fFrameSpeed		= 1.f
	//								, _bool			bFrameRepeat	= false
	//								, _bool			bRandColor		= false
	//);






	// 바운딩 박스 세팅 ( 필요한 경우 )
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

};

