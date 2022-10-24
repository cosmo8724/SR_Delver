#include "..\Header\Monster.h"

#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_fIdle_Speed(0.f)
	, m_fAttack_Speed(0.f)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
	, m_eType(rhs.m_eType)
	, m_vPos(rhs.m_vPos)
	, m_tInfo(rhs.m_tInfo)
	, m_fIdle_Speed(rhs.m_fIdle_Speed)
	, m_fAttack_Speed(rhs.m_fAttack_Speed)
	, m_bClone(true)
{
	m_pTransCom = dynamic_cast<CTransform*>(rhs.m_pTransCom->Clone());
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", m_pTransCom });
}


CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMonster::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	KnockBack(fTimeDelta);
	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return 0;
}

void CMonster::Render_Obejct()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f); // ����
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f); // ȯ��ݻ�

	m_pGraphicDev->SetMaterial(&tMtrl);

	m_pAnimtorCom->Set_Texture();
	//m_pTextureCom->Set_Texture((_ulong)m_fFrame);	// �ؽ�ó ���� ������ �켱������ �Ѵ�.
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

HRESULT	CMonster::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;
}

void CMonster::Billboard()
{
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

	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
}

void CMonster::KnockBack(const _float & fTimeDeleta, _float	fKnockBackSpeed)
{
	if (!m_bKnockBack)
		return;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3 vPos, vTargetLook;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	pPlayerTransformCom->Get_Info(INFO_LOOK, &vTargetLook);

	if (m_fBTimeDelta > 0.3f && m_fHeight >= vPos.y)
	{
		m_bKnockBack = false;
		m_fBTimeDelta = 0.f;

		m_pTransCom->Set_Y(m_fHeight);
		m_pTransCom->KnockBack_Target(&vTargetLook, fKnockBackSpeed, fTimeDeleta);
		m_fBSpeed = m_fBSpeed0;
	}
	else
	{
		m_fBSpeed -= m_fBAccel;
		m_pTransCom->Plus_PosY(m_fBSpeed);
		m_fBTimeDelta += 0.1f;
	}
}

void CMonster::Free(void)
{
	CGameObject::Free();
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster *	pInstance = new CMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
