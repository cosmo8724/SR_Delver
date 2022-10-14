#include "stdafx.h"
#include "ParticleMgr.h"
#include "UserParticle.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CParticleMgr)
CParticleMgr::CParticleMgr()
{
	m_maxParticle = 50;

	for (int i = 0; i < m_maxParticle; ++i)
		m_IdxQue.push(i);
}


CParticleMgr::~CParticleMgr()
{
	Free();
}

void CParticleMgr::Ready_ParticleMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

HRESULT CParticleMgr::Ready_Proto()
{
	// Particle 버퍼컴포넌트
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PtBufferCom", CPtBuffer::Create(m_pGraphicDev)), E_FAIL);


	// Particle 원본 텍스쳐
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle0_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle0/particle0_%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle1_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle1/particle1_%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle2_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle2/particle2_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle3_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle3/particle3_%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle4_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle4/particle4_%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle5_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle5/particle5_0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle6_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle6/particle6_%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle7_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle7/particle7_%d.png", TEX_NORMAL, 4)), E_FAIL);


	return S_OK;
}

HRESULT CParticleMgr::Add_GameObject(CLayer * pLayer)
{
	for (int i = 0; i < m_maxParticle; ++i)
	{
		TCHAR*	szObjTag = new TCHAR[MAX_PATH];
		wsprintf(szObjTag, L"UserParticle");
		_tcscat_s(szObjTag, MAX_PATH, L"%d");
		wsprintf(szObjTag, szObjTag, i);

		CGameObject* pGameObject = CUserParticle::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecObjTag.push_back(szObjTag);
		m_ParticlePool.push_back(pGameObject);
	}

	return S_OK;
}



void CParticleMgr::Call_Particle(PTYPE eType, PTEXTUREID eTex)
{
	_int iIdx = -1;
	if (!m_IdxQue.empty())
	{
		iIdx = m_IdxQue.front();
		//static_cast<CUserParticle*>(m_ParticlePool[iIdx])->Set_Use(true);
		//static_cast<CUserParticle*>(m_ParticlePool[iIdx])->Set_Index(iIdx);
		static_cast<CUserParticle*>(m_ParticlePool[iIdx])->ReUse();

		static_cast<CUserParticle*>(m_ParticlePool[iIdx])->Set_Texture(eTex);
		//static_cast<CUserParticle*>(m_ParticlePool[iIdx])->Set_Target(m_pTarget);
		static_cast<CUserParticle*>(m_ParticlePool[iIdx])->Set_Information(
			true, iIdx, m_pTarget, m_tAttribute, m_tPInfo, m_fFrameSpeed, m_bFrameRepeat, m_bRand);
		static_cast<CUserParticle*>(m_ParticlePool[iIdx])->Set_Particle(eType);
		m_IdxQue.pop();
	}
}

void CParticleMgr::Collect_Particle(_int iIdx)
{
	m_IdxQue.push(iIdx);
}

void CParticleMgr::Set_Info(CGameObject* pObj
	, _int		_maxParticles				// 최대 파티클 수
	, _float	_fSize						// 모든 파티클의 크기
	, _vec3		_vVelocity					// 속도
	, _float	_fLifeTime					// 파티클 소멸까지 유지되는 시간
	, D3DXCOLOR _tColor						// 색깔
	, _float	_fFrameSpeed 				// 프레임 속도
	, _bool		_bFrameRepeat				// 프레임 반복재생 여부
	, _bool		_bRand)						// 색 랜덤 여부
{
	m_pTarget = pObj;
	
	m_tPInfo.iMaxParticles = _maxParticles;
	m_tPInfo.fSize = _fSize;

	m_tAttribute._lifeTime	= _fLifeTime;
	m_tAttribute._color		= _tColor;
	m_tAttribute._velocity	= _vVelocity;

	m_fFrameSpeed			= _fFrameSpeed;
	m_bFrameRepeat			= _bFrameRepeat;
	m_bRand					= _bRand;

}

inline void CParticleMgr::Free(void)
{
	for (auto& tag : m_vecObjTag)
	{
		delete[] tag;
		tag = nullptr;
	}
	m_vecObjTag.clear();
	m_vecObjTag.swap(vector<TCHAR*>());

	m_ParticlePool.clear();
	m_ParticlePool.swap(vector<CGameObject*>());

	Safe_Release(m_pGraphicDev);

}
