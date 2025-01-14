#include "stdafx.h"
#include "Food.h"

#include "Export_Function.h"
#include "Player.h"
#include "CullingMgr.h"

CFood::CFood(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Food";
}

CFood::CFood(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Food";
	m_eItemType = ITEM_FOOD;
	m_str = L"<Food>\nType:Item\nMake a hamburger with this";
}

CFood::CFood(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Food";
	m_iTextureIdx = _eType;
}

CFood::~CFood()
{
}

HRESULT CFood::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_tInfo.iHunger = 10;
	m_eItemType = ITEM_FOOD;
	return S_OK;
}

_int CFood::Update_Object(const _float & fTimeDelta)
{
	if (m_eState == STATE_INV)
		return 0;

	if (m_bDead)
		return OBJ_DEAD;

	int iResult = CItem::Update_Object(fTimeDelta);

	if (STATE_EQUIP == m_eState)
	{
		if (m_bEat)
		{
			Engine::StopSound(SOUND_ITEM);
			Engine::Play_Sound(L"I_Food.mp3", SOUND_ITEM, 1.f);

			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_HungerPlus();
			m_bEat = false;
		}
	}
	else if (STATE_GROUND == m_eState)
	{
		m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pTransCom->Set_Y(m_vPos.y - 0.3f);
	}


	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return iResult;
}

void CFood::LateUpdate_Object(void)
{
	if (m_eState != STATE_GROUND)
		return;

	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	Billboard();
	CGameObject::LateUpdate_Object();
}

void CFood::Render_Obejct(void)
{
	if (m_eState != STATE_GROUND)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

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
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetMaterial(&tOldMtrl);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

HRESULT CFood::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// �ؽ��� �İ�ü ������Ʈ
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Food_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Food_Texture", pComponent });
	m_textureTag = L"Proto_Food_Texture";

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


CFood * CFood::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
{
	CFood*	pInstance = new CFood(pGraphicDev, vPos, _eType);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFood::Free(void)
{
	CGameObject::Free();
}

void CFood::CollisionEvent(CGameObject * pObj)
{

}

void CFood::InteractEvent()
{
	if (STATE_GROUND == m_eState)
	{
		//CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
		//if (pMonster)
		//	return;

		m_bEat = true;
		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}
