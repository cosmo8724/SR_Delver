#include "stdafx.h"
#include "..\Header\Lantern.h"
#include "Export_Function.h"
#include "Player.h"

CLantern::CLantern(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEquipment(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Lantern";
	m_vPos = vPos;
	m_str = L"<Lantern>\nType:Equipment\nIt will light up your vision";
}

CLantern::~CLantern()
{
}

HRESULT CLantern::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_eItemType = ITEM_LANTERN;
	return S_OK;
}

_int CLantern::Update_Object(const _float & fTimeDelta)
{
	int iResult = CItem::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		m_pCenter = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pCenter, -1);
		m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		NULL_CHECK_RETURN(m_pPlayer, -1);
		m_bReady = true;
	}

	_vec3* pPlayerInfo = m_pCenter->Get_InfoAll();

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	switch (m_eState)
	{
	case STATE_GROUND:
	{
		m_pTransCom->Set_Scale(0.6f, 0.6f, 0.6f);
		m_pTransCom->Revolution(pPlayerInfo, matView, 0.f, m_fTimeDelta, STATE_GROUND);
	}
		break;
	case STATE_EQUIP:
	{

		m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);

		m_pTransCom->Item_LeftMotion(m_pGraphicDev, *m_pCenter->Get_WorldMatrixPointer());

		m_pColliderCom->Set_Free(true);

		BOOL bOn = false;
		_vec3 vPos;
		m_pCenter->Get_Info(INFO_POS, &vPos);
		CLightMgr::GetInstance()->Update_Pos(2, vPos);

		m_pGraphicDev->LightEnable(2, TRUE);
	}
		break;
	case STATE_INV:
		m_pGraphicDev->LightEnable(2, FALSE);
		break;
	}

	m_fTimeDelta = fTimeDelta;

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	

	return iResult;
}

void CLantern::LateUpdate_Object(void)
{
	if (STATE_INV != m_eState)
		Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::LateUpdate_Object();
}

void CLantern::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	/*if(m_bText)
		Render_Font(L"Font_Jinji", L"불을 밝혀주는 도구", &_vec2( - 17.f, m_fPosY - 7.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/


#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

CLantern * CLantern::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CLantern*		pInstance = new CLantern(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLantern::Free(void)
{
	CGameObject::Free();
}

void CLantern::CollisionEvent(CGameObject * pObj)
{

}

void CLantern::InteractEvent()
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

HRESULT CLantern::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Lantern_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Lantern_Texture", pComponent });
	m_textureTag = L"Proto_Lantern_Texture";

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}
