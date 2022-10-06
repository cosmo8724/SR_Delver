#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CUserParticle;
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

	// 미리 오브젝
	HRESULT			Add_GameObject(CLayer* pLayer);
	
		// 파티클 Proto 컴포넌트 생성(주로 텍스처)
	HRESULT			Ready_Proto();

	// 파티클 호출
	void			Call_Particle(PTYPE eType);

	// 사용이 끝난 파티클
	void			Collect_Particle(_int iIdx);

public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	vector<TCHAR*>			m_vecObjTag;
	vector<CUserParticle*>	m_ParticlePool;

};

