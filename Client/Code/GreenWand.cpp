#include "stdafx.h"
#include "..\Header\GreenWand.h"
#include "Export_Function.h"
#include "ParticleMgr.h"
#include "BulletMgr.h"

CGreenWand::CGreenWand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
{
	m_ObjTag = L"GreenWand";
}

CGreenWand::CGreenWand(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CWeapon(pGraphicDev)
{
	m_vPos = vPos;
	m_ObjTag = L"GreenWand";
	m_eItemType = ITEM_WEAPON;
	m_eWeaponType = WT_AD;

}

CGreenWand::~CGreenWand()
{
}

HRESULT CGreenWand::Ready_Object(void)
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

	//D3DLIGHT9	tLightInfo;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	//tLightInfo.Type = D3DLIGHT_POINT;
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Position = _vec3(5.f, 3.f, 9.f );
	//tLightInfo.

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);

	return S_OK;
}

HRESULT CGreenWand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_GreenWand1Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_GreenWand1Texture", pComponent });
	m_textureTag = L"Proto_GreenWand1Texture";

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

void CGreenWand::CollisionEvent(CGameObject * pObj)
{
	if (STATE_GROUND == m_eState)
	{
		m_eState = STATE_INV;
		m_pTransCom->Set_Pos(-1000.f, -1000.f, -1000.f);
		m_vPos = { -1000.f, -1000.f, -1000.f };

		m_pColliderCom->Set_Free(true);
		//_vec3 vScale;
		//_matrix matWorld;
		//m_pTransCom->Get_WorldMatrix(&matWorld);

		//vScale = m_pTransCom->Get_Scale();
		//m_bdBox.vMin = { m_vPos.x - vScale.x, m_vPos.y - vScale.y, m_vPos.z - vScale.z };
		//m_bdBox.vMax = { m_vPos.x + vScale.x, m_vPos.y + vScale.y, m_vPos.z + vScale.z };
	}

}

void CGreenWand::Charge(const _float & fTimeDelta)
{
	_int frameEnd = m_pTextureCom->Get_FrameEnd();

	if (m_bCharge)
	{
		m_fFrame += frameEnd * fTimeDelta;
		m_fPlusSpeed += 0.3f;

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
			}
		}
		else if (true == m_bCharge)  
		{
			m_fFrame = 0.f;
			m_bCharge = false;
			m_fChargeTime = 0.f;
			m_bAttack = true;
			m_bParticleCall = false;
		}
	}
}

void CGreenWand::Attack(const _float & fTimeDelta)
{
	if (true == m_bAttack)
	{
		CBulletMgr::GetInstance()->Pre_Setting(BULLET_GREENWAND, 0.f);
		CBulletMgr::GetInstance()->Fire(BULLET_GREENWAND);

		CBulletMgr::GetInstance()->Pre_Setting(BULLET_GREENWAND, 20.f);
		CBulletMgr::GetInstance()->Fire(BULLET_GREENWAND);

		CBulletMgr::GetInstance()->Pre_Setting(BULLET_GREENWAND, -20.f);
		CBulletMgr::GetInstance()->Fire(BULLET_GREENWAND);

		m_bAttack = false;
		m_fPlusSpeed = 0.f;
	}
}

_int CGreenWand::Update_Object(const _float & fTimeDelta)
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
		break;
	}

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fTimeDelta = fTimeDelta;

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());


	// vLook 세팅




	return iResult;
}

void CGreenWand::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();

	if (STATE_INV == m_eState)
		return;
}

void CGreenWand::Render_Obejct(void)
{
	if (m_eState == STATE_INV)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


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

#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}

CGreenWand * CGreenWand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CGreenWand*		pInstance = new CGreenWand(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenWand::Free(void)
{
	CGameObject::Free();
}

void CGreenWand::LevelUp()
{
}


