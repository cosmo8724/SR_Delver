#include "stdafx.h"
#include "UserParticle.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "ParticleMgr.h"

USING(Engine)

CUserParticle::CUserParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_origin = { 10.f, 0.f, 10.f };
	m_fSize = 0.9f;

	m_bdBox.vMin = { 0.f, 0.f, 0.f };
	m_bdBox.vMax = { 30.f, 30.f, 30.f };

	m_Attribute.fLifeTime = 2.f;
	m_Attribute.tColor = { 1.f, 1.f, 1.f, 1.f };

	m_maxParticles = 1;
	m_Attribute.vVelocity = { 1.f, 1.f, 1.f };

	for (int i = 0; i < m_maxParticles; ++i)
		addParticle();
}

CUserParticle::~CUserParticle()
{
}

void CUserParticle::Set_Particle_Texture(_int iSelected)
{
	wstring str = L"Proto_Particle";
	wchar_t	num[10];
	_itow_s(iSelected, num, 10);
	str = str + num + L"_Texture";
	m_pTextureCom = static_cast<CTexture*>(Engine::Get_Component(L"Layer_Tool_Environment", L"Particle", str.c_str(), ID_STATIC));
}

void CUserParticle::Set_Texture(PTEXTUREID eTex)
{
	CComponent* pComponent = nullptr;

	wstring str = L"Proto_Particle";
	wchar_t	num[10];
	_itow_s(eTex, num, 10);
	str = str + num + L"_Texture";

	auto iter = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(str.c_str()));

	if (iter != m_mapComponent[ID_STATIC].end())	// �̹� �����ϴ� �ؽ��ĸ��̶�� �ʿ� ���� �ʰ� ����.
		m_pTextureCom = static_cast<CTexture*>(iter->second);
	else
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(str.c_str()));
		NULL_CHECK(m_pTextureCom);
		m_mapComponent[ID_STATIC].insert({ str.c_str(), pComponent });
	}



}

void CUserParticle::Set_Particle(PTYPE _eType)
{
	removeAllParticles();
	m_eType = _eType;
	switch (_eType)
	{
	case PTYPE_SNOW:
		//m_bdBox.vMin = { 0.f, 0.f, 0.f };
		//m_bdBox.vMax = { 10.f, 10.f, 10.f };
		//m_Attribute.vVelocity = { 1.f, 1.f, 1.f };
		m_maxParticles = 5000;
		break;
	case PTYPE_SPOT:
		// ��ƼŬ ��
		m_maxParticles = 1;

		// ���� ��ġ
		if (nullptr == m_pTarget)
			MSG_BOX("��ƼŬ Ÿ�� ���� �Ф�");
		CTransform* pCom = static_cast<CTransform*>(m_pTarget->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		m_origin = pCom->Get_Pos();

		// ��ƼŬ �ݺ����
		m_bFrameRepeat = true;

		// �����ð�
		m_Attribute.fLifeTime = 1.f;

		break;
	}

	for (int i = 0; i < m_maxParticles; ++i)
		addParticle();
	m_bReady = true;
}


void CUserParticle::Play_Particle()
{
	while (isDead() && m_bPlay)
	{
		removeAllParticles();
		m_fFrame = 0.f;

		for (int i = 0; i < m_maxParticles; ++i)
			addParticle();
	}
}

HRESULT CUserParticle::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pBufferCom->Set_ParticleList(&m_particles);
	return CPSystem::Ready_Object();
}

_int CUserParticle::Update_Object(const _float & fTimeDelta)
{
	if (!m_bUse || !m_bReady)
		return 0;

	_float frameEnd = (_float)m_pTextureCom->Get_FrameEnd();
	m_fFrame += frameEnd * fTimeDelta * m_fFrameSpeed;

	if (m_fFrame >= frameEnd)
	{
		if (m_bFrameRepeat)
			m_fFrame = 0.f;
		else
			m_fFrame = frameEnd;
	}

	int iResult = CPSystem::Update_Object(fTimeDelta);
	update(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CUserParticle::LateUpdate_Object(void)
{
	if (!m_bUse || !m_bReady)
		return;

	CGameObject::LateUpdate_Object();
}

void CUserParticle::Render_Obejct(void)
{
	if (!m_bUse || !m_bReady)
		return;

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	_vec3 vMax = m_bdBox.vMax;
	_vec3 vMin = m_bdBox.vMin;


	preRender();
	CPSystem::render();
	postRender();
}

void CUserParticle::resetParticle(ATTINFO * attribute)
{
	switch (m_eType)
	{
	case PTYPE_FIREWORK:
	{
		attribute->bIsAlive = true;
		attribute->vPosition = m_origin;

		_vec3 min = { -1.f,-1.f,-1.f };
		_vec3 max = { 1.f, 1.f, 1.f };

		GetRandomVector(&attribute->vVelocity, &min, &max);

		//��ü�� ����� ���� ����ȭ
		D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);

		attribute->vVelocity *= m_fVelocityMulti;
	}
		break;

	case PTYPE_SNOW:
	{
		attribute->bIsAlive = true;

		// �������� ��ġ������ ���� ������ x, z��ǥ�� ��´�.
		GetRandomVector(&attribute->vPosition,
			&m_bdBox.vMin,
			&m_bdBox.vMax);

		// ���� (y-��ǥ)�� �׻� �������� �ֻ���� �ȴ�.
		attribute->vPosition.y = m_bdBox.vMax.y;

		// �����̴� �Ʒ������� �������� �ణ ������ ���Ѵ�.
		attribute->vVelocity.x = GetRandomFloat(0.0f, m_Attribute.vVelocity.x) * (-3.0f);
		attribute->vVelocity.y = GetRandomFloat(0.0f, m_Attribute.vVelocity.y) * (-10.0f);
		attribute->vVelocity.z = m_Attribute.vVelocity.z;
		attribute->vVelocity *= m_fVelocityMulti;
	}
		break;

	case PTYPE_LASER:
	{
		attribute->bIsAlive = true;

		// change to camera position
		attribute->vPosition = m_origin;
		attribute->vVelocity = 10.f *  m_Attribute.vVelocity;
		attribute->vVelocity = 10.f * _vec3({ 3.2f, 1.f, 1.f });
	}
		break;

	case PTYPE_FOUNTAIN:
	{
		attribute->bIsAlive = true;
		attribute->vPosition = { 10.f, 10.f, 10.f };

		_vec3 min = { -1.f, 0.f, -1.f };
		_vec3 max = m_Attribute.vVelocity;

		GetRandomVector(&attribute->vVelocity, &min, &max);

		//��ü�� ����� ���� ����ȭ
		D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);

		attribute->vVelocity *= 10.f;

	}
		break;

	case PTYPE_SPOT:
	{
		attribute->bIsAlive = true;
		attribute->vPosition = m_origin;
	}
		break;
	}

	// ����
	if (m_bRand)
	{
		attribute->tColor = D3DXCOLOR(
			GetRandomFloat(0.f, m_Attribute.tColor.r),
			GetRandomFloat(0.f, m_Attribute.tColor.g),
			GetRandomFloat(0.f, m_Attribute.tColor.b),
			m_Attribute.tColor.a);
	}
	else
	{
		attribute->tColor = m_Attribute.tColor;
	}
	attribute->fAge = 0.f;
	attribute->fLifeTime = m_Attribute.fLifeTime; // 2�� ������ ������ ������.
}

void CUserParticle::update(_float fTimeDelta)
{
	switch (m_eType)
	{
	case PTYPE_FIREWORK:
	{
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			// ������ ��ƼŬ�� ����
			if (iter->bIsAlive)
			{
				iter->vPosition += iter->vVelocity * fTimeDelta;
				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // ���δ�.
					iter->bIsAlive = false;
			}
		}
	}
		break;

	case PTYPE_SNOW:
	{
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			iter->vPosition += iter->vVelocity * fTimeDelta;

			// ����Ʈ�� ��踦 ����°�?
			if (m_bdBox.isPointInside(iter->vPosition) == false)
			{
				// ��踦 ��� ��ƼŬ�� ��Ȱ���Ѵ�.
				resetParticle(&(*iter));
			}
		}

	}
		break;

	case PTYPE_LASER:
	{
		std::list<ATTINFO>::iterator i;

		for (i = m_particles.begin(); i != m_particles.end(); i++)
		{
			i->vPosition += i->vVelocity * fTimeDelta;

			i->fAge += fTimeDelta;

			if (i->fAge > i->fLifeTime) // kill 
				i->bIsAlive = false;
		}
	}
		break;

	case PTYPE_FOUNTAIN:
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			// ������ ��ƼŬ�� ����
			if (iter->bIsAlive)
			{
				iter->vVelocity.y -= GetRandomFloat(0.f, 1.4f);
				iter->vPosition += (iter->vVelocity * fTimeDelta);
				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // ���δ�.
					iter->bIsAlive = false;
			}
		}
		break;

	case PTYPE_SPOT:
		CTransform* pCom = static_cast<CTransform*>(m_pTarget->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		auto iter = m_particles.begin();
		iter->fAge += fTimeDelta;
		iter->vPosition = pCom->Get_Pos();
		if (iter->fAge > iter->fLifeTime)
		{
			CParticleMgr::GetInstance()->Collect_Particle(m_iIndex);
			ReUse();
		}


		//for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		//{
		//	//iter->vPosition += iter->vVelocity * fTimeDelta;
		//	CTransform* pCom = static_cast<CTransform*>(m_pTarget->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		//	m_origin = pCom->Get_Pos();

		//	// ����Ʈ�� ��踦 ����°�?
		//	if (m_bdBox.isPointInside(iter->vPosition) == false)
		//	{
		//		// ��踦 ��� ��ƼŬ�� ��Ȱ���Ѵ�.
		//		//resetParticle(&(*iter));

		//	}
		//}
		break;
	}
}

void CUserParticle::preRender()
{
	CPSystem::preRender();

	switch (m_eType)
	{
	case PTYPE_FIREWORK:
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
		break;
	}
}

void CUserParticle::postRender()
{
	CPSystem::postRender();

	switch (m_eType)
	{
	case PTYPE_FIREWORK:
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
		break;
	}
}

HRESULT CUserParticle::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	//// ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CPtBuffer*>(Engine::Clone_Proto(L"Proto_PtBufferCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PtBufferCom", pComponent });

	// �ؽ��� �İ�ü ������Ʈ
	// ��� ��ƼŬ �� �ؽ��� �İ�ü�� ������ �ֵ��� �ϴ°�??
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Particle0_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Particle0_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Particle1_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Particle1_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Particle2_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Particle2_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Particle3_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Particle3_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Particle4_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Particle4_Texture", pComponent });

	return S_OK;
}
void CUserParticle::ReUse()
{
	m_iIndex = -1;
	m_bUse = false;
	m_bReady = false;
	m_pTarget = nullptr;

	removeAllParticles();
}

CUserParticle * CUserParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUserParticle*	pInstance = new CUserParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUserParticle::Free(void)
{
	CPSystem::Free();
}
