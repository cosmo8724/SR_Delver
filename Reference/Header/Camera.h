#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual const _vec3&		Get_Eye()  const { return m_vEye; }
	void		Get_At(_vec3& pAt) { pAt = m_vAt; }
	void		Get_Up(_vec3& pUp) { pUp = m_vUp; }
	void		Set_Eye(_float& pEye) { m_vEye.y = pEye; }
	void		Get_ViewMatrix(_matrix& _viewMat) { _viewMat = m_matView; }
	void		Get_ProjectMatrix(_matrix& _ProjMat) { _ProjMat = m_matProj; }
	void		Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }
	_bool		Get_Switch() { return m_bSwitch; }

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int Update_Object(const _float& fTimeDelta)	override;
	virtual void LateUpdate_Object(void)					override;

protected:
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix		m_matView, m_matProj;
	_bool		m_bSwitch = false;
	
protected:
	virtual void		Free(void);
};
END