#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CUserParticle;
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

	// �̸� ������
	HRESULT			Add_GameObject(CLayer* pLayer);
	
		// ��ƼŬ Proto ������Ʈ ����(�ַ� �ؽ�ó)
	HRESULT			Ready_Proto();

	// ��ƼŬ ȣ��
	void			Call_Particle(PTYPE eType);

	// ����� ���� ��ƼŬ
	void			Collect_Particle(_int iIdx);

public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	vector<TCHAR*>			m_vecObjTag;
	vector<CUserParticle*>	m_ParticlePool;

};

