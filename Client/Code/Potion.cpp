#include "stdafx.h"
#include "Potion.h"
#include "Export_Function.h"
#include "Player.h"
#include "CullingMgr.h"
#include "StaticCamera.h"
#include "ParticleMgr.h"

CPotion::CPotion(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Potion";
}

CPotion::CPotion(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Potion";
	m_eItemType = ITEM_POTION;
	m_str = L"<Potion>\nType:Potion\nI don't tell you what it is";
}

CPotion::CPotion(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Potion";
	m_iTextureIdx = _eType;
	m_tPotionType = _eType;
}

CPotion::~CPotion()
{
}

HRESULT CPotion::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_tInfo.iHpHeal = 10;
	m_eItemType = ITEM_POTION;

	return S_OK;
}

_int CPotion::Update_Object(const _float & fTimeDelta)
{
	if (m_eState == STATE_INV)
		return 0;


	if (m_bFinished)
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;



	int iResult = CItem::Update_Object(fTimeDelta);

	if (STATE_EQUIP == m_eState)
	{
		Engine::StopSound(SOUND_ITEM);
		Engine::Play_Sound(L"I_Drink.mp3", SOUND_ITEM, 1.f);

		RandomItem(fTimeDelta);
	}
	else if (STATE_GROUND == m_eState)
	{
		m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pTransCom->Set_Y(m_vPos.y - 0.3f);
	}


	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return iResult;
}

void CPotion::LateUpdate_Object(void)
{
	if (m_iCnt == 0 && m_bFinished)
	{
		m_bDead = true;
	}


	if (m_eState != STATE_GROUND)
		return;

	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);



	Billboard();
	CGameObject::LateUpdate_Object();
}

void CPotion::Render_Obejct(void)
{
	if (m_eState != STATE_GROUND)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	////m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	D3DMATERIAL9		tMtrl, tOldMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));
	m_pGraphicDev->GetMaterial(&tMtrl);
	tOldMtrl = tMtrl;

	if (tMtrl.Ambient.r < 0.5f)
	{
		tMtrl.Ambient = { 0.5f, 0.5f, 0.5f, 1.f };
		m_pGraphicDev->SetMaterial(&tMtrl);
	}


	m_pTextureCom->Set_Texture(m_iTextureIdx);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	m_pGraphicDev->SetMaterial(&tOldMtrl);
	////m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

HRESULT CPotion::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	//            Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	//  ؽ     İ ü       Ʈ
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Potion_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Potion_Texture", pComponent });
	m_textureTag = L"Proto_Potion_Texture";

	//               Ʈ
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

CPotion * CPotion::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
{
	CPotion*	pInstance = new CPotion(pGraphicDev, vPos, _eType);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPotion::Free(void)
{
	CGameObject::Free();
}

void CPotion::CollisionEvent(CGameObject * pObj)
{

	//if (STATE_GROUND == m_eState)
	//{
	//	CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
	//	if (pMonster)
	//		return;

	//	m_eState = STATE_INV;
	//	m_pColliderCom->Set_Free(true);
	//}

}

void CPotion::InteractEvent()
{
	if (STATE_GROUND == m_eState)
	{
		//CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
		//if (pMonster)
		//	return;

		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}

void CPotion::RandomItem(const _float& fTimeDelta)
{
	CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	PLAYERINFO pPlayerInfo = pPlayer->Get_Info();

	switch (m_tPotionType)
	{
	case POTION_0: // yellow
	{
		pPlayer->Set_HpFull();
		CStaticCamera* pCam = static_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
		pCam->Wave_Camera(10.f, _vec3(0.f, 0.f, 1.f), 30.f);
		m_bFinished = true;
	}
	break;
	case POTION_1: // red
	{
		m_fDotTime += fTimeDelta;
		if (1.f < m_fDotTime)
		{
			if (m_iDot > m_tInfo.iHpHeal)
			{
				m_bFinished = true;
				return;
			}

			pPlayer->Set_HpPlus();
			m_iDot++;
			m_fDotTime = 0.f;
		}
	}
	break;
	case POTION_2: // green
	{
		m_fDotTime += fTimeDelta;
		if (1.f < m_fDotTime)
		{

			if (m_iDot > m_tInfo.iHpHeal)
			{
				m_bFinished = true;
				return;
			}
			
			pPlayer->Set_HpMinus();
			m_iDot++;
			m_fDotTime = 0.f;
		}
	}
	break;
	case POTION_3: // blue
	{
		pPlayer->Set_Slow();
		m_bFinished = true;
	}
	break;
	case POTION_4: // pink
	{
		pPlayer->Set_Stun();
		m_bFinished = true;
	}
	break;
	}

}