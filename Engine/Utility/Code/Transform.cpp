#include "Transform.h"

#include "Export_Function.h"

USING(Engine)

CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_fAngleSpeed(1.f)
	, m_fAttackSpeed(3.f)
	, m_fDefenseAngle(-80.f)
	, m_fShieldvLook(0.9f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matOldBill);

}


Engine::CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs), m_vScale(rhs.m_vScale), m_vAngle(rhs.m_vAngle)
	, m_fAngleSpeed(rhs.m_fAngleSpeed)
	, m_fAttackSpeed(rhs.m_fAttackSpeed)
	, m_fDefenseAngle(rhs.m_fDefenseAngle)
	, m_fShieldvLook(rhs.m_fShieldvLook)
{
	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(m_vInfo[i], rhs.m_vInfo[i], sizeof(_vec3));

	memcpy(m_matWorld, rhs.m_matWorld, sizeof(_matrix));

	memcpy(m_matOldBill, rhs.m_matOldBill, sizeof(_matrix));

}

CTransform::~CTransform()
{
}


void CTransform::Set_Info(_vec3 vRight, _vec3 vUp, _vec3 vLook)
{
	memcpy(&m_matWorld.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matWorld.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matWorld.m[2][0], &vLook, sizeof(_vec3));

}

void CTransform::Prepare_Attack()
{
	m_bFinished = false;
	m_fAttackAngle = 0.f;
	m_fDefenseAngle = -80.f;
	m_fAttackSpeed = 3.f;
	m_fShieldvLook = 0.9f;
}

void Engine::CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matRot = *Compute_LookAtTarget(pTargetPos);
	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;
}

void CTransform::ChangeHeight_Target(const _vec3 * pTargetPos, const _float & fHeight, const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, fHeight, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;
}

const _matrix* Engine::CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3		vLook = *pTargetPos - m_vInfo[INFO_POS];

	_vec3		vAxis, vUp;
	_matrix		matRot;

	// D3DXMatrixRotationAxis : ������ ��ȸ�� ����� ������ִ� �Լ�
	return D3DXMatrixRotationAxis(&matRot,
		D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vLook),
		acosf(D3DXVec3Dot(D3DXVec3Normalize(&vLook, &vLook),
			D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}


void CTransform::Revolution(const _vec3* pCenter, _matrix _matView, _float fAngle, const _float& fTimeDelta, ITEMSTATE eState)
{

	// �߽���ǥ
	_vec3	vPos, vRight, vLook;

	vPos = pCenter[INFO_POS];
	vRight = pCenter[INFO_RIGHT];
	vLook = pCenter[INFO_LOOK];

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);

	if (STATE_EQUIP == eState)
	{
		m_vInfo[INFO_POS] = { vPos.x + 0.5f * vRight.x + 1.3f * vLook.x,
			vPos.y + 0.5f * vRight.y + 1.3f * vLook.y,
			vPos.z + 0.5f * vRight.z + 1.3f * vLook.z };
	}


	//m_vInfo[INFO_POS] = { 50.f, 5.f ,  50.f };

	// ������
	_matrix matBill;
	if (_matView._21 > 0.f)
	{
		matBill = m_matOldBill;
	}
	else
	{
		D3DXMatrixIdentity(&matBill);
		matBill._11 = _matView._11;
		matBill._13 = _matView._13;
		matBill._31 = _matView._31;
		matBill._33 = _matView._33;

		m_matOldBill = matBill;
	}


	D3DXMatrixInverse(&matBill, 0, &matBill);


	// �� �� �� �� ��
	// ��-1 �� �� ��
	// �� * ��-1 * ��-1 * �� * �� * ��

	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);


	// ȸ��
	_matrix		matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, fAngle);


	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld = matScale * matRot * matBill * matTrans;


}

void CTransform::KnockBack_Target(_vec3 * pTargetLook, const _float & fSpeed, const _float & fTimeDelta)
{
	D3DXVec3Normalize(pTargetLook, pTargetLook);
	m_vInfo[INFO_POS] += *pTargetLook * fSpeed * fTimeDelta;
}

void CTransform::Item_Motion(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld)
{
	_matrix matParent = _matWorld; // 부모행렬은 플레이어의 월드행렬

	_vec3 vRight, vUp, vLook, vPos;

	vRight = { matParent._11, matParent._12, matParent._13 };
	vUp = { matParent._21, matParent._22, matParent._23 };
	vLook = { matParent._31, matParent._32, matParent._33 };
	vPos = { matParent._41, matParent._42, matParent._43 };
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.f));

	_matrix matTrans;
	_vec3 vTrans = 0.5f * vRight + 0.8f * vLook - 0.2f * vUp;
	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);

	_matrix matRev;
	_float fRad = 0.f;
	if (m_bStop)
	{
		m_fAngle = 5.f;
	}

	if (!m_bStop && m_vOldPos.y == vPos.y && m_vOldPos != vPos)
	{
		m_fAngle += m_fAngleSpeed;
		if (m_fAngle > 5.f || m_fAngle < -5.f)
			m_fAngleSpeed *= -1;
		fRad = D3DXToRadian(m_fAngle);
	}
	m_vOldPos = vPos;
	D3DXMatrixRotationAxis(&matRev, &vRight, fRad);

	_matrix matPos; // parent
	D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);


	// 빌보드
	_matrix matBill, matView;
	D3DXMatrixIdentity(&matBill);
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_matWorld = matScale * matRot * matBill * matTrans * matRev * matPos;
}

_bool CTransform::Item_Attack(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld)
{
	_matrix matParent = _matWorld; // �θ������ �÷��̾��� �������

	_vec3 vRight, vUp, vLook, vPos;

	vRight = { matParent._11, matParent._12, matParent._13 };
	vUp = { matParent._21, matParent._22, matParent._23 };
	vLook = { matParent._31, matParent._32, matParent._33 };
	vPos = { matParent._41, matParent._42, matParent._43 };
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	_matrix matRot, matRotY, matRotX;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(60.f));
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(30.f));
	matRot = matRotY * matRotX;

	_matrix matTrans;
	_vec3 vTrans = 0.5f * vRight + 0.9f * vLook - 0.2f * vUp;
	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);

	_matrix matRev;
	_float fRad = 0.f;
	m_fAttackAngle -= m_fAttackSpeed;
	if (m_fAttackAngle < -25.f)
		m_fAttackSpeed *= -1;
	if (m_fAttackAngle > 10.f)
		m_bFinished = true;

	fRad = D3DXToRadian(m_fAttackAngle);
	D3DXMatrixRotationAxis(&matRev, &_vec3{ 0.f, 1.f, 0.f }, fRad);

	_matrix matPos; // parent
	D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);

	// ������
	_matrix matBill, matView;
	D3DXMatrixIdentity(&matBill);
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_matWorld = matScale * matRot * matBill * matTrans * matRev * matPos;

	return m_bFinished;
}

void CTransform::Item_LeftMotion(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld)
{
	_matrix matParent = _matWorld; 

	_vec3 vRight, vUp, vLook, vPos;

	vRight = { matParent._11, matParent._12, matParent._13 };
	vUp = { matParent._21, matParent._22, matParent._23 };
	vLook = { matParent._31, matParent._32, matParent._33 };
	vPos = { matParent._41, matParent._42, matParent._43 };
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(130.f));

	_matrix matTrans;
	_vec3 vTrans = -0.5f * vRight + 0.8f * vLook - 0.2f * vUp;
	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);

	_matrix matRev;
	D3DXMatrixIdentity(&matRev);
	_float fRad = 0.f;
	if (m_vOldPos.y == vPos.y && m_vOldPos != vPos)
	{
		m_fAngle += m_fAngleSpeed;
		if (m_fAngle > 5.f || m_fAngle < -5.f)
			m_fAngleSpeed *= -1;
		fRad = D3DXToRadian(m_fAngle);
	}
	m_vOldPos = vPos;
	D3DXMatrixRotationAxis(&matRev, &vRight, fRad);

	_matrix matPos; // parent
	D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);


	_matrix matBill, matView;
	D3DXMatrixIdentity(&matBill);
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_matWorld = matScale * matRot * matBill * matTrans * matRev * matPos;
}

_bool CTransform::Item_Defense(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld)
{
	_matrix matParent = _matWorld; // �θ������ �÷��̾��� �������

	_vec3 vRight, vUp, vLook, vPos;

	vRight = { matParent._11, matParent._12, matParent._13 };
	vUp = { matParent._21, matParent._22, matParent._23 };
	vLook = { matParent._31, matParent._32, matParent._33 };
	vPos = { matParent._41, matParent._42, matParent._43 };
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	_matrix matRot, matRotY, matRotX;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(60.f));
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(30.f));
	matRot = matRotY * matRotX;

	_matrix matTrans;
	m_fShieldvLook -= 0.03f;
	if (m_fShieldvLook < 0.3f)
	{
		m_fShieldvLook = 0.3f;
	}

	_vec3 vTrans = 0.5f * vRight + m_fShieldvLook * vLook - 0.2f * vUp;
	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);

	_matrix matRev;
	_float fRad = 0.f;

	m_fDefenseAngle += m_fAttackSpeed;
	if (m_fDefenseAngle > -45.f)
		m_fAttackSpeed *= -1;
	if (m_fDefenseAngle < -80.f)
		m_bFinished = true;

	fRad = D3DXToRadian(m_fDefenseAngle);
	D3DXMatrixRotationAxis(&matRev, &_vec3{ 0.f, 1.f, 0.f }, fRad);

	_matrix matPos; // parent
	D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);

	// ������
	_matrix matBill, matView;
	D3DXMatrixIdentity(&matBill);
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_matWorld = matScale * matRot * matBill * matTrans * matRev * matPos;

	return m_bFinished;

}

HRESULT CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float & fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	// ȸ��
	_matrix		matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}


	for (_uint i = 0; i < INFO_END; ++i)
	{
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

	return 0;
}

CTransform * CTransform::Create(void)
{
	CTransform* pInstance = new CTransform;
	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}

void CTransform::Free(void)
{
	CComponent::Free();
}
