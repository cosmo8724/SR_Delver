#include "stdafx.h"
#include "..\Header\GuitarMan.h"

#include "Export_Function.h"

#include "TalkWindow.h"
#include "MiniMap.h"

CGuitarMan::CGuitarMan(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNPC(pGraphicDev)
{
}


CGuitarMan::~CGuitarMan()
{
}

HRESULT CGuitarMan::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(19.f, 2.f, 20.f);

	// 다이얼로그 테스트 (※메모리 누수 주의)
	wstring str = L"";
	m_vecDialogue.push_back(str);

	str = L"내 기타 연주가 마음에 드나?";
	m_vecDialogue.push_back(str);

	str = L"그렇다면 팁을 내주셔야지!";
	m_vecDialogue.push_back(str);

	str = L"(2 Gold 를 빼았겼다!)";
	m_vecDialogue.push_back(str);

	str = L"대신 내가 이 곳 전체에 울릴 정도로 기타를 크게 연주해주지.";
	m_vecDialogue.push_back(str);

	return S_OK;
}

_int CGuitarMan::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta, 0.5f);

	OnText(fTimeDelta);

	return 0;
}

void CGuitarMan::LateUpdate_Object(void)
{
	CNPC::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CGuitarMan::Render_Obejct(void)
{
	CNPC::Render_Obejct();

}

HRESULT CGuitarMan::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_NPC_GuitarMan_Texture");

	return S_OK;
}

void CGuitarMan::OnText(const _float& fTimeDelta)
{
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3 vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));


	m_fLBClick += fTimeDelta;
	CTalkWindow* pTalkWindow = dynamic_cast<CTalkWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_TalkWindow"));
	NULL_CHECK(pTalkWindow);
	if (fDist < 2.f) // 거리 안 으로 들어올 때 대화 가능
	{
		m_bDetected = true;

		if (Engine::Get_DIKeyState(DIK_T) && m_fLBClick > 0.3f)
		{
			pTalkWindow->Set_OnText();
			pTalkWindow->Set_TextCount();
			pTalkWindow->Set_Text(&m_vecDialogue);

			m_fLBClick = 0.f;
		}
	}
	else
	{
		if (m_bDetected)
		{
			pTalkWindow->Reset_TextCount();
			pTalkWindow->Set_OFFText();
		}
		m_bDetected = false;
	}
}

CGuitarMan * CGuitarMan::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGuitarMan *	pInstance = new CGuitarMan(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGuitarMan::Free(void)
{
	CNPC::Free();
}
