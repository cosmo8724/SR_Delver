#pragma once
#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CPtBuffer;
END

class CPSystem : public CGameObject
{
protected:
	explicit CPSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPSystem(const CPSystem& rhs);
	virtual ~CPSystem();

public:
	inline _float			GetRandomFloat(_float lowBound, _float highBound)
	{
		if (lowBound >= highBound) // 잘못된 입력
			return lowBound;

		// [0,1] 범위의 임의 float 를 얻는다.
		_float f = (rand() % 10000) * 0.0001f;

		// [lowBound, highBound] 범위의 float을 리턴
		return (f* (highBound - lowBound)) + lowBound;
	}
	inline void				GetRandomVector(_vec3* pOut, _vec3* pMin, _vec3* pMax)
	{
		pOut->x = GetRandomFloat(pMin->x, pMax->x);
		pOut->y = GetRandomFloat(pMin->y, pMax->y);
		pOut->z = GetRandomFloat(pMin->z, pMax->z);

	}

public:
	virtual	 HRESULT		Ready_Object(void);
	virtual	 _int			Update_Object(const _float& fTimeDelta);
	virtual	 void			LateUpdate_Object(void);
	virtual	 void			Render_Obejct(void);
	virtual	 void			Free(void);

public:
	virtual void			reset();										// 시스템 내 모든 파티클 속성 리셋
	virtual void			resetParticle(ATTINFO* attribute) = 0;	// 한 파티클의 속성을 리셋
	virtual void			addParticle();									// 파티클을 추가
	virtual void			update(_float fTimeDelta) = 0;			// 파티클 갱신

	virtual void			preRender();									// 렌더링에 앞서 지정할 초기 렌더 상태
	virtual void			render();										// 렌더링
	virtual void			postRender();									// 특정 파티클 시스템이 지정했을 수 있는 렌더 상태를 복구할때 이용

public:
	_bool					isEmpty();										// 현재 시스템에 파티클이 있으면 True
	_bool					isDead();										// 시스템 내 모든 파티클이 죽은경우 True

protected:
	virtual void			removeDeadParticles();						// 죽은 파티클을 리스트에서 제거
	virtual void			removeAllParticles();

protected:
	_vec3					m_origin;			// 시스템의 원천. 파티클이 시작되는 곳
	BDBOX					m_bdBox;			// 파티클이 이동할 수 있는 부피 제한
	_float					m_fEmitRate;	// 시스템에 새로운 파티클이 추가되는 비율. (개수/s)
	_float					m_fSize;			// 시스템 내 모든 파티클의 크기

	CTexture*			m_pTextureCom = nullptr;
	CPtBuffer*			m_pBufferCom = nullptr;

	list<ATTINFO>		m_particles;		// 시스템 내 파티클 속성 리스트
												// 파티클 생성/제거/갱신 시 이 리스트를 이용
												// 파티클을 그릴 준비가 완료되면 리스트 노드 일부를
												// 버텍스 버퍼로 복사하고, 파티클 드로잉 과정을 거침.
												// 모든 파티클을 그릴때까지 이 과정을 반복.
	_int					m_maxParticles;	// 주어진 시간동안 시스템이 가질 수 있는 최대 파티클 수

	ATTINFO				m_Attribute; //	 Attribute 세팅용 추가 변수

	_float					m_fFrame = 0.f;
};

