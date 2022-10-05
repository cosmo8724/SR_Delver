#include "stdafx.h"
#include "UserParticle.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"

USING(Engine)

CUserParticle::CUserParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_origin = { 10.f, 0.f, 10.f };
	m_fSize = 0.9f;

	m_bdBox.vMin = { 0.f, 0.f, 0.f };
	m_bdBox.vMax = { 10.f, 10.f, 10.f };

	m_Attribute.fLifeTime = 2.f;
	m_Attribute.tColor = { 1.f, 1.f, 1.f, 1.f };

	m_maxParticles = 5000;
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
	CGameObject::LateUpdate_Object();
}

void CUserParticle::Render_Obejct(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

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

		//구체를 만들기 위한 정규화
		D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);

		attribute->vVelocity *= m_fVelocityMulti;
	}
		break;

	case PTYPE_SNOW:
	{
		attribute->bIsAlive = true;

		// 눈송이의 위치지정을 위해 임의의 x, z좌표를 얻는다.
		GetRandomVector(&attribute->vPosition,
			&m_bdBox.vMin,
			&m_bdBox.vMax);

		// 높이 (y-좌표)는 항상 경계상자의 최상단이 된다.
		attribute->vPosition.y = m_bdBox.vMax.y;

		// 눈송이는 아래쪽으로 떨어지며 약간 뒤쪽을 향한다.
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

		//구체를 만들기 위한 정규화
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

	// 공통
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
	attribute->fLifeTime = m_Attribute.fLifeTime; // 2초 동안의 수명을 가진다.
}

void CUserParticle::update(_float fTimeDelta)
{
	switch (m_eType)
	{
	case PTYPE_FIREWORK:
	{
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			// 생존한 파티클만 갱신
			if (iter->bIsAlive)
			{
				iter->vPosition += iter->vVelocity * fTimeDelta;
				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // 죽인다.
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

			// 포인트가 경계를 벗어났는가?
			if (m_bdBox.isPointInside(iter->vPosition) == false)
			{
				// 경계를 벗어난 파티클은 재활용한다.
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
			// 생존한 파티클만 갱신
			if (iter->bIsAlive)
			{
				iter->vVelocity.y -= GetRandomFloat(0.f, 1.4f);
				iter->vPosition += (iter->vVelocity * fTimeDelta);
				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // 죽인다.
					iter->bIsAlive = false;
			}
		}
		break;

	case PTYPE_SPOT:
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			//iter->vPosition += iter->vVelocity * fTimeDelta;

			// 포인트가 경계를 벗어났는가?
			if (m_bdBox.isPointInside(iter->vPosition) == false)
			{
				// 경계를 벗어난 파티클은 재활용한다.
				resetParticle(&(*iter));
			}
		}
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

	//// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CPtBuffer*>(Clone_Proto(L"Proto_PtBufferCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PtBufferCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	// 모든 파티클 용 텍스쳐 컴객체를 가지고 있도록 하는건??
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Particle0_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Particle0_Texture", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Particle1_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Particle1_Texture", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Particle2_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Particle2_Texture", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Particle3_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Particle3_Texture", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Particle4_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Particle4_Texture", pComponent });

	return S_OK;
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
