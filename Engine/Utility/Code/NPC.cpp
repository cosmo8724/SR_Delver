#include "..\Header\NPC.h"

#include "Export_Function.h"

CNPC::CNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CNPC::~CNPC()
{
}

void CNPC::Billboard()
{
	// ������
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// ���� ���� �� �ڵ�� ������ ������ ���߿� ������ �� �� ����
	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
}

void CNPC::Free(void)
{
	CGameObject::Free();
}
