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

	// ��ƼŬ ȣ��
	void			Call_Particle(PTYPE eType, PTEXTUREID eTex);

	// ����� ���� ��ƼŬ ȸ��
	void			Collect_Particle(_int iIdx);

	// ȣ�� �� ��ƼŬ ���� ����
	void			Set_Info(CGameObject* pObj);


public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	_int					m_maxParticle;
	vector<TCHAR*>			m_vecObjTag;
	vector<CGameObject*>	m_ParticlePool;

	queue<_int>				m_IdxQue;


	ATTRIBUTE		m_pInfo;
	CGameObject*	m_pTarget;

};

