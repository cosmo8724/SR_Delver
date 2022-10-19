#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	explicit CTransform(void);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	const _matrix*		Get_WorldMatrixPointer() { return &m_matWorld; }
	void				Move_Pos(const _vec3* const pDir) { m_vInfo[INFO_POS] += *pDir; }
	void				Rotation(ROTATIONID eID, const _float& fAngle) { *(((_float*)&m_vAngle) + eID) += fAngle; }
	void				Get_Info(INFOID eID, _vec3* pInfo) { memcpy(pInfo, &m_matWorld.m[eID][0], sizeof(_vec3)); }
	const _vec3&		Get_Pos() { return m_vInfo[INFO_POS]; }
	void				Set_Angle(_float fX, _float fY, _float fZ) { m_vAngle = { fX, fY, fZ }; }
	const _vec3&		Get_Angle() { return m_vAngle; }
	void				Set_Scale(_float fX, _float fY, _float fZ) { m_vScale = { fX, fY, fZ }; }
	const _vec3&		Get_Scale() { return m_vScale; }
	void				Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y = fY;
		m_vInfo[INFO_POS].z = fZ;
	}
	void				Set_Y(const _float& fY) { m_vInfo[INFO_POS].y = fY; }
	void				Plus_PosY(_float _fY) { m_vInfo[INFO_POS].y += _fY; }
	void				Get_WorldMatrix(_matrix* pWorld) const { *pWorld = m_matWorld; }
	void				Set_WorldMatrix(_matrix* pWorld) { m_matWorld = *pWorld; }
	_vec3*				Get_InfoAll() { return m_vInfo; }
	void				Set_Info(_vec3 vRight, _vec3 vUp, _vec3 vLook);
	void				Set_Look(_vec3 vLook) { m_vLook = vLook; }
	_vec3				Get_Look() { return m_vLook; }
	_float				Get_AttackAngle() { return m_fAttackAngle; }

public:
	void				Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void				ChangeHeight_Target(const _vec3* pTargetPos, const _float& fSTest, const _float& fSpeed, const _float& fTimeDelta);
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);
	void				Revolution(const _vec3* pCenter, _matrix _matView, _float fAngle, const _float& fTimeDelta, ITEMSTATE eState);
	void				KnockBack_Target(_vec3* pTargetLook, const _float& fSpeed, const _float& fTimeDelta); // �˹�
	void				Item_Motion(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld);
	_bool				Item_Attack(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld);
	void				Item_LeftMotion(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld);
	_bool				Item_Defense(LPDIRECT3DDEVICE9 pGraphicDev, _matrix _matWorld);




public:
	HRESULT				Ready_Transform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	void			Prepare_Attack();
	_bool			Is_Finished() { return m_bFinished; }
public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;
	_float			m_fAngle = 0.f;
	_float			m_fAngleSpeed;

	_vec3			m_vOldPos;
	_bool			m_bFinished = false;
	_float			m_fAttackAngle = 0.f;
	_float			m_fAttackSpeed;
	_float			m_fDefenseAngle;// = 90.f;


	_matrix			m_matOldBill;
	_float			m_fShieldvLook;
	_vec3			m_vLook;

public:
	static CTransform*		Create(void);
	virtual CComponent*		Clone(void);
private:
	virtual void Free(void);

};

END