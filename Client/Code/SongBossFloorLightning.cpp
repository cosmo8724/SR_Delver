#include "stdafx.h"
#include "..\Header\SongBossFloorLightning.h"

#include "Export_Function.h"	
#include "BulletMgr.h"
#include "SongBossFloor.h"

CSongBossFloorLightning::CSongBossFloorLightning(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_iBulletCount(0)
{
}

CSongBossFloorLightning::CSongBossFloorLightning(const CSongBossFloorLightning & rhs)
	:CBullet(rhs)
{
}

CSongBossFloorLightning::~CSongBossFloorLightning()
{
}

HRESULT CSongBossFloorLightning::Ready_Object(_int iBulletCount)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(2.f, 20.f, 10.f);

	m_iBulletCount = iBulletCount;
	return S_OK;
}

HRESULT CSongBossFloorLightning::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_MusicNote_FloorLightning_Texture");

	return S_OK;
}

_int CSongBossFloorLightning::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	m_pAnimtorCom->Play_Animation(fTimeDelta * 2.2f);
	Add_RenderGroup(RENDER_ALPHA, this);

	// 이전 음표가 생겼던 자리에 번개가 치도록
	if (!m_bReady)
	{
		wstring objTags[5];
		for (_int i = 0; i < 5; i++)
		{
			objTags[i] = L"SongBoss_Floor";
			wchar_t index[10];
			_itow_s(i, index, 10);
			objTags[i] += index;

			CTransform* pFloor = static_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", objTags[i].c_str(), L"Proto_TransformCom", ID_DYNAMIC));
			NULL_CHECK(pFloor);
			pFloor->Get_Info(INFO_POS, &vFloorPos);
		}

		_float fNotePos = 3.f;
		_float fNotePosY = vFloorPos.y; // -65.8f;

		if (m_iBulletCount == 0)
			m_pTransCom->Set_Pos(vFloorPos.x, fNotePosY, vFloorPos.z + fNotePos);
		else if (m_iBulletCount == 1)
			m_pTransCom->Set_Pos(vFloorPos.x + fNotePos, fNotePosY, vFloorPos.z);
		else if (m_iBulletCount == 2)
			m_pTransCom->Set_Pos(vFloorPos.x - fNotePos, fNotePosY, vFloorPos.z);
		else if (m_iBulletCount == 3)
			m_pTransCom->Set_Pos(vFloorPos.x, fNotePosY, vFloorPos.z - fNotePos);
		else if (m_iBulletCount == 4)
			m_pTransCom->Set_Pos(vFloorPos.x, fNotePosY, vFloorPos.z);

		m_bReady = true;
	}

	m_fLifeTime += fTimeDelta;
	return iResult;
}

void CSongBossFloorLightning::LateUpdate_Object(void)
{
	Billboard();

	if (!m_bFire)
		return;

	if (0.3f < m_fLifeTime)
		Reset();

	CGameObject::LateUpdate_Object();
}

void CSongBossFloorLightning::Render_Obejct(void)
{
	if (!m_bFire)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pAnimtorCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void CSongBossFloorLightning::Billboard()
{
	// 빌보드
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);


	if (matView._21 > 0.f)
	{
		matBill = m_matOldBill;
	}
	else
	{
		D3DXMatrixIdentity(&matBill);
		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		m_matOldBill = matBill;
	}

	D3DXMatrixInverse(&matBill, 0, &matBill);

	_vec3 vScale = m_pTransCom->Get_Scale();
	_matrix matScale, matScaleInv;
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixInverse(&matScaleInv, 0, &matScale);

	m_matWorld = matBill *matWorld;
	m_pTransCom->Set_WorldMatrix(&m_matWorld);
}

CSongBossFloorLightning * CSongBossFloorLightning::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iBulletCount)
{
	CSongBossFloorLightning*		pInstance = new CSongBossFloorLightning(pGraphicDev);
	if (FAILED(pInstance->Ready_Object(iBulletCount)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSongBossFloorLightning::Free(void)
{
	CGameObject::Free();
}

void CSongBossFloorLightning::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_bReady = false;
	m_pAnimtorCom->Set_Frame();
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, LIGHTNING_SONGBOSS);
}