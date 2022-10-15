#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CCamera;
END

class CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	CCameraMgr();
	~CCameraMgr();

public:
	CCamera*			Get_CurCamera() { return m_vecCamera[m_CurId]; }

public:
	void			Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev);

	// add gameobject in stage
	HRESULT			Add_GameObject(CLayer* pLayer);

	void			Change_Camera(CAMERAID _eCur, CAMERAID _eChange); // �������� �ٲܶ�

	void			Change_Camera(CAMERAID _eChange);


	void			Set_Camera(CGameObject* pObj = nullptr				// Ÿ��: ObjectCamera�� ����
								, _float fEyeDist = 1.f		// Ÿ�ٿ� ���� Eye�� vLook ���� �Ÿ�
								, _float fAtDist = 1.f			// Ÿ�ٿ� ���� At�� vLook ���� �Ÿ�
	);	

	void			Action_Camera(_float fAngle);

	void			Action_PlayerDie();


public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	vector<CCamera*>	m_vecCamera;
	CAMERAID			m_CurId = CAM_STATIC;
};

