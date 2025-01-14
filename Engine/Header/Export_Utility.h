#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Include.h"

// Component
// Buffer
#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "SphereTex.h"
#include "PtBuffer.h"
#include "Collider.h"

#include "Shader.h"

// Logics
#include "Transform.h"
#include "Calculator.h"
#include "Texture.h"
#include "Animator.h"

// Object
#include "Camera.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Monster.h"
#include "UI.h"
#include "NPC.h"

// Management
#include "ProtoMgr.h"
#include "Management.h"
#include "Renderer.h"
#include "Pipeline.h"
#include "LightMgr.h"
#include "CollisionMgr.h"

BEGIN(Engine)

// Management


inline HRESULT		Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);
inline HRESULT		Set_Scene(CScene* pScene);
inline _int				Update_Scene(const _float& fTimeDelta);
inline void				LateUpdate_Scene(void);
inline void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
inline 	CComponent*	Get_Component(const _tchar* pLayerTag,
	const _tchar* pObjTag,
	const _tchar* pComponentTag,
	COMPONENTID eID);
inline 	CGameObject*Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline	CLayer*		Get_Layer(const _tchar* pLayerTag);

// ProtoMgr
inline HRESULT		Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*	Clone_Proto(const _tchar* pProtoTag);
inline HRESULT		Delete_Proto(const _tchar* pProtoTag);


// Renderer

inline void				Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void				Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void				Clear_RenderGroup(void);

// lightMgr

inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
inline void			Update_Light(const _uint& iIndex, const D3DLIGHT9* pLightInfo);
inline void			Update_Pos(const _uint& iIndex, _vec3 vPos);
inline void			Update_Color(const _uint& iIndex, D3DXCOLOR tColor);

// CollisionMgr
inline  void			CollisionAABB(CGameObject* _pDest, CGameObject* _pSour);
inline	void			CollisionSphere(CGameObject* _pDest, CGameObject* _pSour);
inline  void			CollisionTest(CGameObject* _pDest, CGameObject* _pSour);


inline void				Release_Utility(void);

#include "Export_Utility.inl"

END
#endif 
