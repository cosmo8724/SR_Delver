#include "stdafx.h"
#include "Gold.h"

#include "Export_Function.h"
#include "Player.h"
#include "CullingMgr.h"

CGold::CGold(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Gold";
}

CGold::CGold(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Gold";
	m_eItemType = ITEM_GOLD;
	m_str = L"<Gold>\nType:Item\nMy money is precious!!! ";

}

CGold::CGold(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Gold";
	m_iTextureIdx = _eType;
}

CGold::~CGold()
{
}

HRESULT CGold::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_eItemType = ITEM_GOLD;
	
	m_vPopAngle =
	{	
		// high - low,  + low
		((rand() % 10000) * 0.0001f* (1.f + 1.f)) - 1.f,
		((rand() % 10000) * 0.0001f* (1.f - 0.f)) + 0.f,
		((rand() % 10000) * 0.0001f* (1.f + 1.f)) - 1.f
	};
	D3DXVec3Normalize(&m_vPopAngle, &m_vPopAngle);

	return S_OK;
}

_int CGold::Update_Object(const _float & fTimeDelta)
{
	if (m_eState == STATE_INV)
		return 0;

	if (m_bDead)
		return OBJ_DEAD;

	int iResult = CItem::Update_Object(fTimeDelta);

	switch (m_eState)
	{
	case STATE_GROUND:
		m_pColliderCom->Set_Free(false);
		m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pTransCom->Set_Y(m_vPos.y - 0.5f);
		break;
	case STATE_POP:
		m_pColliderCom->Set_Free(true);
		m_vPopAngle.y -= fTimeDelta;
		if (m_pTransCom->Get_Pos().y >= m_vPos.y)
		{
			m_pTransCom->Move_Pos(&_vec3(m_vPopAngle.x * 0.04f, m_vPopAngle.y * 0.3f ,m_vPopAngle.z * 0.04f));
		}
		else
			m_eState = STATE_GROUND;

		break;
	}



	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return iResult;
}

void CGold::LateUpdate_Object(void)
{
	if (m_eState == STATE_INV)
		return;

	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	Billboard();
	CGameObject::LateUpdate_Object();
}

void CGold::Render_Obejct(void)
{
	if (m_eState == STATE_INV )
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(m_iTextureIdx);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

HRESULT CGold::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// �ؽ��� �İ�ü ������Ʈ
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Gold_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Gold_Texture", pComponent });
	m_textureTag = L"Proto_Gold_Texture";

	// ������� ������Ʈ
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}


CGold * CGold::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
{
	CGold*	pInstance = new CGold(pGraphicDev, vPos, _eType);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CGold::Free(void)
{
	CGameObject::Free();
}

void CGold::CollisionEvent(CGameObject * pObj)
{

}

void CGold::InteractEvent()
{
	if (STATE_GROUND == m_eState)
	{
		Engine::StopSound(SOUND_ITEM);
		Engine::Play_Sound(L"I_Gold.mp3", SOUND_ITEM, 1.f);

		/*
		GOLD_0 : 2
		GOLD_1 : 4
		GOLD_2 : 6
		GOLD_3 : 8
		*/
		//CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
		//if (pMonster)
		//	return;

		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		m_iTextureIdx = (m_iTextureIdx + 1) * 2;
		pPlayer->Set_GoldPlus(m_iTextureIdx);

		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}