#include "stdafx.h"
#include "..\Header\Wand.h"
#include "Export_Function.h"
#include "ParticleMgr.h"
#include "BulletMgr.h"
#include "LightMgr.h"

CWand::CWand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
{
	m_ObjTag = L"Wand";
}

CWand::CWand(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CWeapon(pGraphicDev)
{
	m_vPos = vPos;
	m_ObjTag = L"Wand";
	m_eItemType = ITEM_WEAPON;
	m_eWeaponType = WT_AD;
	m_str = L"<PinkWand>\nType:Weapon\nPinkWand magic";
}

CWand::~CWand()
{
}

HRESULT CWand::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	//m_pTransCom->Set_Pos(6.f, 0.5f, 15.f);
	m_eState = STATE_GROUND;

	//_vec3 vScale;
	//_matrix matWorld;
	//m_pTransCom->Get_WorldMatrix(&matWorld);

	//vScale = m_pTransCom->Get_Scale();
	//m_bdBox.vMin = { m_vPos.x - vScale.x, m_vPos.y - vScale.y, m_vPos.z - vScale.z };
	//m_bdBox.vMax = { m_vPos.x + vScale.x, m_vPos.y + vScale.y, m_vPos.z + vScale.z };

	return S_OK;
}

HRESULT CWand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Wand1Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Wand1Texture", pComponent });
	m_textureTag = L"Proto_Wand1Texture";

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_ShaderSpecular"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ShaderSpecular", pComponent });

	return S_OK;

}

void CWand::CollisionEvent(CGameObject * pObj)
{
	//if (STATE_GROUND == m_eState)
	//{
	//	CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
	//	if (pMonster)
	//		return;

	//	m_eState = STATE_INV;
	//	m_pTransCom->Set_Pos(-1000.f, -1000.f, -1000.f);
	//	m_vPos = { -1000.f, -1000.f, -1000.f };

	//	m_pColliderCom->Set_Free(true);
	//}

}

void CWand::InteractEvent()
{
	if (STATE_GROUND == m_eState)
	{
		m_eState = STATE_INV;
		m_pTransCom->Set_Pos(-1000.f, -1000.f, -1000.f);
		m_vPos = { -1000.f, -1000.f, -1000.f };

		m_pColliderCom->Set_Free(true);
	}
}

void CWand::Charge(const _float & fTimeDelta)
{
	_int frameEnd = m_pTextureCom->Get_FrameEnd();

	if (m_bCharge)
	{
		m_fFrame += frameEnd * fTimeDelta;
		m_fPlusSpeed += 0.3f;
		m_pTransCom->Set_Stop(true);

		if (!m_bParticleCall)
		{
			m_bParticleCall = true;
		}


		if (m_fFrame >= frameEnd)
			m_fFrame = (_float)frameEnd;

		if (5.f < m_fChargeTime)
		{
			m_fFrame = 0.f;
			m_fChargeTime = 0.f;
			m_bCharge = false;
			m_bAttack = true;
			m_pTransCom->Set_Stop(false);
		}

	}

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)	
	{
		m_bClick = true;
		m_fChargeTime += m_fTimeDelta;

		if (m_fChargeTime > 0.3f)	
		{
			m_bClick = false;		
			m_bCharge = true;
			m_pTransCom->Set_Stop(true);

			D3DXCOLOR tColor = { 0.5f, 0.f, 0.5f, 1.f };
			Update_Color(LIGHT_WAND, tColor);
			m_pGraphicDev->LightEnable(LIGHT_WAND, TRUE);

		}
	}
	else
	{
		if (true == m_bClick)	
		{
			m_fFrame += frameEnd * fTimeDelta * 3.f;
			if (m_fFrame >= frameEnd)
			{
				m_fFrame = 0.f;
				m_bClick = false;
				m_bAttack = true;
				m_fChargeTime = 0.f;
				m_pTransCom->Set_Stop(true);
			}
		}
		else if (true == m_bCharge) 
		{
			m_fFrame = 0.f;
			m_bCharge = false;
			m_fChargeTime = 0.f;
			m_bAttack = true;
			m_bParticleCall = false;
			m_pTransCom->Set_Stop(false);
		}
	}
}

void CWand::Attack(const _float & fTimeDelta)
{
	if (true == m_bAttack)
	{
		Engine::StopSound(SOUND_EFFECT);
		Engine::Play_Sound(L"I_Wand.mp3", SOUND_EFFECT, 1.f);

		CBulletMgr::GetInstance()->Pre_Setting(BULLET_WAND, m_fPlusSpeed);
		CBulletMgr::GetInstance()->Fire(BULLET_WAND);
		//CParticleMgr::GetInstance()->Set_Info(this);
		//CParticleMgr::GetInstance()->Call_Particle(PTYPE_LASER, TEXTURE_0);
		m_bAttack = false;
		m_fPlusSpeed = 0.f;

		m_pGraphicDev->LightEnable(LIGHT_WAND, FALSE);
		m_pTransCom->Set_Stop(false);
	}
}

_int CWand::Update_Object(const _float & fTimeDelta)
{
	if (STATE_INV == m_eState)
		return 0;

	int iResult = CWeapon::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		m_pCamera = dynamic_cast<CCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
		if (nullptr == m_pCamera)
			return 0;
		else
		{
			m_pCenter = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
			if (nullptr == m_pCenter)
				return 0;
			else m_bReady = true;
		}
	}



	_vec3* pPlayerInfo = m_pCenter->Get_InfoAll();

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	switch (m_eState)
	{
	case STATE_GROUND:
		m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pTransCom->Revolution(pPlayerInfo, matView, 0.f, m_fTimeDelta, STATE_GROUND);
		//m_pTransCom->Move_Pos(&_vec3({ 0.005f, 0.005f, 0.005f }));
		break;
	case STATE_EQUIP:
		if (!(Engine::Get_DIKeyState(DIK_TAB) & 0x80))
		{
			Charge(fTimeDelta);
			Attack(fTimeDelta);
		}

		m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);
		//m_pTransCom->Revolution(pPlayerInfo, matView, 45.f, m_fTimeDelta, STATE_EQUIP);

		m_pTransCom->Item_Motion(m_pGraphicDev, *m_pCenter->Get_WorldMatrixPointer());

		
		// Light
		_vec3 vPos, vRight, vLook, vUp;
		m_pCenter->Get_Info(INFO_POS, &vPos);
		m_pCenter->Get_Info(INFO_RIGHT, &vRight);
		m_pCenter->Get_Info(INFO_LOOK, &vLook);
		m_pCenter->Get_Info(INFO_UP, &vUp);
		_vec3 vTrans = vPos + 0.25f * vRight + 0.7f * vLook + 0.2f * vUp;
		Update_Pos(LIGHT_WAND, vTrans);

		break;
	}

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fTimeDelta = fTimeDelta;

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());


	// vLook 세팅




	return iResult;
}

void CWand::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();

	//if (STATE_INV == m_eState)
	//	return;
}

void CWand::Render_Obejct(void)
{
	if (m_eState == STATE_INV)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f); // 원색
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f); // 환경반사
	
	// 수행 x
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);




	// Shader Test
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//_matrix		IMatrix = *D3DXMatrixIdentity(&IMatrix);
	//_matrix matWorld, matView, matProj, matViewInv;

	//matView = matProj = IMatrix;


	//m_pTransCom->Get_WorldMatrix(&matWorld);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	//D3DXMatrixInverse(&matViewInv, 0, &matView);
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldCameraPosition",
	//				_vec4(matViewInv._41, matViewInv._42, matViewInv._43, 1.f)
	//	, sizeof(_vec4))))
	//	return;


	//D3DXMatrixTranspose(&matWorld, &matWorld);
	//D3DXMatrixTranspose(&matView, &matView);
	//D3DXMatrixTranspose(&matProj, &matProj);

	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
	//	return;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
	//	return;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_matrix))))
	//	return;

	//m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);


	//m_pShaderCom->Begin_Shader(0); // pass 0

	//m_pBufferCom->Render_Buffer();

	//m_pShaderCom->End_Shader();

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);



#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}

CWand * CWand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CWand*		pInstance = new CWand(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWand::Free(void)
{
	CGameObject::Free();
}

void CWand::LevelUp()
{
}



