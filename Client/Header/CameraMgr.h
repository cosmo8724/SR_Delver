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

	void			Change_Camera(CAMERAID _eCur, CAMERAID _eChange); // 수동으로 바꿀때

	void			Change_Camera(CAMERAID _eChange);


	void			Set_Camera(CGameObject* pObj = nullptr				// 타겟: ObjectCamera에 쓰임
								, _float fEyeDist = 1.f		// 타겟에 대해 Eye의 vLook 기준 거리
								, _float fAtDist = 1.f			// 타겟에 대해 At의 vLook 기준 거리
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

