#include "stdafx.h"
#include "PSystem.h"
#include "PtBuffer.h"
#include "Export_Function.h"


CPSystem::CPSystem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_origin({ 0.f,0.f,0.f })
	, m_fEmitRate(0.f)
	, m_fSize(0.f)
	, m_pTextureCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_maxParticles(0)
{
}

CPSystem::CPSystem(const CPSystem & rhs)
	: CGameObject(rhs)
	, m_origin(rhs.m_origin)
	, m_fEmitRate(rhs.m_fEmitRate)
	, m_fSize(rhs.m_fSize)
	, m_maxParticles(rhs.m_maxParticles)
{
}

CPSystem::~CPSystem()
{
}

HRESULT CPSystem::Ready_Object(void)
{
	return S_OK;
}

_int CPSystem::Update_Object(const _float & fTimeDelta)
{
	return CGameObject::Update_Object(fTimeDelta);
}

void CPSystem::LateUpdate_Object(void)
{
}

void CPSystem::Render_Obejct(void)
{
}

void CPSystem::Free(void)
{
	m_particles.clear();

	CGameObject::Free();
}

void CPSystem::reset()
{
	for (auto& iter = m_particles.begin(); iter != m_particles.end(); ++iter)
	{
		resetParticle(&(*iter));
	}
}

void CPSystem::addParticle()
{
	ATTINFO tAttribute;

	resetParticle(&tAttribute);

	m_particles.push_back(tAttribute);
}

void CPSystem::preRender()
{// �������� �ռ� ������ �ʱ� ���� ����
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);

	// ���� ������ ��ü �ؽ�ó ����Ʈ ��������Ʈ�� �ؽ�ó ���ο� �̿��� ����
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// ����Ʈ ũ�⸦ �佺���̽� ������ �ؼ��� ���ΰ�?
	// �佺���̽� ���� : ī�޶� ������ 3D ����Ʈ. ī�޶���� �Ÿ��� ���� ũ�Ⱑ ������.
	// false : ��ũ�������̽� ����
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// �Ÿ�(����Ʈ�� ī�޶� ���� �Ÿ�)�� ���� ��ƼŬ ũ�� ����
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// �ؽ�ó�� ���ĸ� �̿��Ѵ�.
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CPSystem::render()
{
	if (!m_particles.empty())
	{
		m_pTextureCom->Set_Texture(_ulong(m_fFrame));
		m_pBufferCom->Render_Buffer();
	}
}

void CPSystem::postRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

_bool CPSystem::isEmpty()
{
	return m_particles.empty();
}

_bool CPSystem::isDead()
{
	for (auto& pt : m_particles)
	{
		if (true == pt.bIsAlive)
		{
			return false;
		}
	}

	return true;
}

void CPSystem::removeDeadParticles()
{
	for (auto iter = m_particles.begin(); iter != m_particles.end();)
	{
		if (false == iter->bIsAlive)
			m_particles.erase(iter);
		else
			++iter;
	}
}

void CPSystem::removeAllParticles()
{
	m_particles.clear();
}
