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
	m_eTex = eTex;
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

	if (nullptr == m_pTarget)
		MSG_BOX("��ƼŬ Ÿ�� ���� �Ф�");
	CTransform* pCom = static_cast<CTransform*>(m_pTarget->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	m_eType = _eType;

	m_pTransCom = pCom;
	m_origin = pCom->Get_Pos();

	for (int i = 0; i < m_maxParticles; ++i)
		addParticle();

	m_pBufferCom->Set_ParticleList(&m_particles);
	//m_bReady = true;



	m_bUse = true;

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
	//if (!m_bUse || !m_bReady)
	//	return 0;
	if (!m_bUse)
		return 0;
	else
		m_bReady = true;// component������ ready���� �̷����� �����Ƿ� ������ ��Ʈ�� �ϱ� ����

	if (isDead())
	{
		//CParticleMgr::GetInstance()->Collect_Particle(m_iIndex);
		//ReUse();
		return OBJ_TEST;

	}


	if (m_bFrameMove)
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
	}
	else
	{
		if (m_bFrameRand)
		{
			if (!m_bFrameSet)
			{
				_float frameEnd = (_float)m_pTextureCom->Get_FrameEnd();
				m_fFrame = rand() % (_int)(frameEnd + 1);
				m_bFrameSet = true;
			}
		}
		else
			m_fFrame = m_fFixFrame;
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
		attribute->vPosition = m_origin;

		_vec3 min = { -1.f, -1.f, -1.f };
		_vec3 max = { 1.f, 1.f, 1.f };

		GetRandomVector(&attribute->vVelocity, &min, &max);

		//��ü�� ����� ���� ����ȭ
		D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);

		attribute->vVelocity *= 10.f;

	}
	break;

	case PTYPE_SPOT:
	{
		attribute->bIsAlive = true;

	}
	break;

	case PTYPE_REMAIN:
		attribute->bIsAlive = true;
		attribute->vPosition = m_origin;

		break;

	case PTYPE_TRACER:
	{
		attribute->bIsAlive = true;
		attribute->vPosition =
		{
			m_origin.x + GetRandomFloat(-0.3f, 0.3f),
			m_origin.y + GetRandomFloat(-0.3f, 0.3f),
			m_origin.z + GetRandomFloat(-0.3f, 0.3f)
		};
	}
	break;
	case PTYPE_CIRCLING:
	{
		attribute->bIsAlive = true;

		for (int i = 0; i < m_maxParticles; ++i)
		{
			m_fStartAngles[i] = 360.f / (m_maxParticles-1)*i;
		}
	}
	break;
	case PTYPE_MOOD:
	{
		attribute->bIsAlive = true;
	}
	break;

	}

	//����
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
		m_fSize -= 0.005f;
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
	{
		m_fSize -= 0.05f;
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			// ������ ��ƼŬ�� ����
			if (iter->bIsAlive)
			{
				
				iter->vVelocity.y -= GetRandomFloat(0.f, 1.f);
				iter->vPosition += (0.3f * iter->vVelocity * fTimeDelta);
				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // ���δ�.
					iter->bIsAlive = false;
			}
		}
		//for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		//{
		//	// ������ ��ƼŬ�� ����
		//	if (iter->bIsAlive)
		//	{
		//		iter->vVelocity.y -= GetRandomFloat(0.f, 1.4f);
		//		iter->vPosition += (iter->vVelocity * fTimeDelta);
		//		iter->fAge += fTimeDelta;
		//		if (iter->fAge > iter->fLifeTime) // ���δ�.
		//			iter->bIsAlive = false;
		//	}
		//}
	}
	break;

	case PTYPE_SPOT:
	{
		_vec3 vRight, vUp, vLook, vPos;
		m_pTransCom->Get_Info(INFO_RIGHT, &vRight);
		m_pTransCom->Get_Info(INFO_UP, &vUp);
		m_pTransCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			if (iter->bIsAlive)
			{
				iter->vPosition = vPos + m_Attribute.vVelocity.x * vRight
					+ m_Attribute.vVelocity.y * vUp
					+ m_Attribute.vVelocity.z * vLook;

				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // ���δ�.
					iter->bIsAlive = false;
			}

		}

	}
	break;
	case PTYPE_REMAIN: // �ܻ� ��ƼŬ

	{
		CTransform* pCom = static_cast<CTransform*>(m_pTarget->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		_float index = 1.f;
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			// ������ ��ƼŬ�� ����
			if (iter->bIsAlive)
			{
				_vec3 vDir = pCom->Get_Pos() - iter->vPosition;
				D3DXVec3Normalize(&vDir, &vDir);

				_float fSpeed = 30.f - index*0.5f;
				index++;

				iter->vPosition += (fSpeed* fTimeDelta* vDir);
				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // ���δ�.
				{
					iter->bIsAlive = false;
				}
			}
		}
	}
	break;


	case PTYPE_TRACER:
	{
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			// ������ ��ƼŬ�� ����
			if (iter->bIsAlive)
			{
				//iter->vPosition = pCom->Get_Pos();
				iter->fAge += fTimeDelta;

				if (iter->fAge > iter->fLifeTime) // ���δ�.
				{
					iter->bIsAlive = false;
				}
			}
		}

	}
	break;

	case PTYPE_CIRCLING:
	{
		_vec3 vRight, vUp, vLook, vPos;
		m_pTransCom->Get_Info(INFO_RIGHT, &vRight);
		m_pTransCom->Get_Info(INFO_UP, &vUp);
		m_pTransCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vLook, &vLook);

		_matrix matRot, matTrans, matPos, matWorld;

		int i = 0;
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			if (iter->bIsAlive)
			{

				m_pTransCom->Get_Info(INFO_POS, &vPos);

				m_fStartAngles[i] = _float((_int)(m_fStartAngles[i] + m_fAngleSpeed) % 360);
				D3DXMatrixRotationAxis(&matRot, &vLook, D3DXToRadian(m_fStartAngles[i]));
				i++;
				m_fDist -= 0.005f;
				if (m_fDist <= 0)
					m_fDist = 0;

				m_fSize -= 0.0005f;
				D3DXMatrixTranslation(&matTrans, m_fDist * vUp.x, m_fDist * vUp.y, m_fDist * vUp.z);
				matWorld = matTrans * matRot;
				_vec3 vTemp = { 0.f, 0.f,0.f };
				D3DXVec3TransformCoord(&vTemp, &vTemp, &matWorld);

				vPos = vPos + m_Attribute.vVelocity.x * vRight
							+ m_Attribute.vVelocity.y * vUp
							+ m_Attribute.vVelocity.z * vLook;

				iter->vPosition = vPos + vTemp;

				iter->fAge += fTimeDelta;
				if (iter->fAge > iter->fLifeTime) // ���δ�.
					iter->bIsAlive = false;
			}

		}
	}
	break;
	case PTYPE_MOOD:
	{
		
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

	//// ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CPtBuffer*>(Engine::Clone_Proto(L"Proto_PtBufferCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PtBufferCom", pComponent });

	// �ؽ��� �İ�ü ������Ʈ
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Particle5_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Particle5_Texture", pComponent });




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
void CUserParticle::Set_Information(_bool _bUse, _int _iIdx, CGameObject * _pObj, ATTRIBUTE _att, PINFO _pInfo
	, _float _fFrameSpeed, _bool _bFrameRepeat, _bool _bRand)
{
	//m_bUse		= _bUse;
	m_iIndex = _iIdx;
	m_pTarget = _pObj;

	m_Attribute.fLifeTime = _att._lifeTime;
	m_Attribute.tColor = _att._color;
	m_Attribute.vVelocity = _att._velocity;

	m_maxParticles = _pInfo.iMaxParticles;
	m_fSize = _pInfo.fSize;

	m_fFrameSpeed = _fFrameSpeed;
	m_bFrameRepeat = _bFrameRepeat;
	m_bRand = _bRand;

}

void CUserParticle::ReUse()
{
	m_iIndex = -1;
	m_bUse = false;
	m_bReady = false;
	m_pTarget = nullptr;

	m_fFrame = 0.f;
	m_bFrameRepeat = false;
	m_fFrameSpeed = 1.f;
	m_fSize = 1.f;

	//memset(&m_Attribute, 0, sizeof(ATTINFO));
	m_pBufferCom->Set_ParticleList(nullptr);

	//for (_uint i = 0; i < ID_END; ++i)
	//{
	//	for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
	//	m_mapComponent[i].clear();
	//}

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
