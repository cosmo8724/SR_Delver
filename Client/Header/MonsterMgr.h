#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CItem;
class CMonster;
END

class CMonsterMgr : public CBase
{
	DECLARE_SINGLETON(CMonsterMgr)

private:
	CMonsterMgr();
	~CMonsterMgr();

public:
	void			Ready_ItemMgr(LPDIRECT3DDEVICE9 pGraphicDev);

	// 아이템 Proto 컴포넌트 생성(주로 텍스쳐)
	HRESULT			Ready_Proto();

	// 아직 미정(필요할 경우 사용)
	HRESULT			Ready_Clone(CLayer* pLayer);

	// 아이템 정적 할당용(스테이지에서 Layer 생성 전 Add GameObject로 미리 삽입되는 경우)
	HRESULT			Add_GameObject(CLayer* pLayer, const _tchar* objTag, ITEMTYPE eType, _vec3 vPos);

	// 아이템 동적 할당용
	HRESULT			Add_GameObject(const _tchar* pLayerTag, const _tchar* objTag, ITEMTYPE eType, _vec3 vPos);

	// 아이템 동적 할당용 (인벤토리 이미지아이템 용)
	CGameObject*	Add_GameObject(const _tchar* pLayerTag, wstring texTag, CItem* pObj);



public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	vector<TCHAR*>		m_vecItemObjTags[ITEM_END];	// 아이템이 동적생성될 때 오브젝트 태그 저장소
	vector<TCHAR*>		m_vecItemTextureTags;		// 인벤토리에 들어가는 아이템 InvImg 용 텍스쳐 컴퍼넌트 태그 저장소

	vector<CGameObject*>	m_vecItemPool;

};

