#include "..\..\Header\PtBuffer.h"

USING(Engine)

CPtBuffer::CPtBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_vbSize(2048)
	, m_vbOffset(0)
	, m_vbBatchSize(512)
	, m_pParticles(nullptr)

{
}

CPtBuffer::CPtBuffer(const CPtBuffer & rhs)
	: CComponent(rhs)
	, m_vbSize(rhs.m_vbSize)
	, m_vbOffset(rhs.m_vbOffset)
	, m_vbBatchSize(rhs.m_vbBatchSize)
	, m_pParticles(nullptr)
{
	// D3D장치 의존적 초기화 작업을 처리
	m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(PARTICLE),	// 버텍스 버퍼 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		// _DYNAMIC : 동적 버텍스버퍼를 사용한다.
		// _POINTS	: 버텍스 버퍼가 포인트 스프라이트를 보관할 것임을 지정
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0);
}

CPtBuffer::~CPtBuffer()
{
}

HRESULT CPtBuffer::Ready_Buffer()
{
	// D3D장치 의존적 초기화 작업을 처리
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(PARTICLE),	// 버텍스 버퍼 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		// _DYNAMIC : 동적 버텍스버퍼를 사용한다.
		// _POINTS	: 버텍스 버퍼가 포인트 스프라이트를 보관할 것임을 지정
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0), E_FAIL);

	return S_OK;
}

void CPtBuffer::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(Particle));
	m_pGraphicDev->SetFVF(FVF_PARTICLE);


	// 버텍스 버퍼를 벗어날 경우 처음부터 시작한다.
	if (m_vbOffset >= m_vbSize)
		m_vbOffset = 0;

	Particle* v = 0;

	m_pVB->Lock(
		m_vbOffset * sizeof(PARTICLE),
		m_vbBatchSize * sizeof(PARTICLE),
		(void**)&v,
		m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

	DWORD numParticlesInBatch = 0;

	// 모든 파티클이 렌더링 될때까지
	for (auto iter = m_pParticles->begin(); iter != m_pParticles->end(); ++iter)
	{
		if (iter->bIsAlive)
		{
			// 한단계의 생존한 파티클을 다음 버텍스 버퍼 세그먼트로 복사
			v->vPosition = iter->vPosition;
			v->tColor = (D3DCOLOR)iter->tColor;
			v++; // next element;

			numParticlesInBatch++; // 단계 카운터를 증가시킨다.

								   // 현재 단계가 모두 채워져있는가?
			if (numParticlesInBatch == m_vbBatchSize)
			{
				// 버텍스 버퍼로 복사된 마지막 단계의 파티클들을 그린다.
				m_pVB->Unlock();

				m_pGraphicDev->DrawPrimitive(
					D3DPT_POINTLIST,
					m_vbOffset,
					m_vbBatchSize);

				// 단계가 그려지는 동안 다음 단계를 파티클로 채운다.

				// 다음 단계의 처음 오프셋으로 이동한다.
				m_vbOffset += m_vbBatchSize;

				// 버텍스 버퍼의 경계를 넘는 메모리로 오프셋을 설정하지 ㅇ낳는다.
				// 경계를 넘을 경우 처음부터 시작
				if (m_vbOffset >= m_vbSize)
					m_vbOffset = 0;

				m_pVB->Lock(
					m_vbOffset * sizeof(PARTICLE),
					m_vbBatchSize * sizeof(PARTICLE),
					(void**)&v,
					m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

				numParticlesInBatch = 0; // 다음단계를 위한 리셋
			}
		}
	}

	m_pVB->Unlock();

	// numParticleInBatch == vbBatchSize 조건이 만족되지 않아
	// 마지막단계가 렌더링되지 않는 경우가 발생할 수 있음.
	// 일부만 채워진 단계는 이곳에서 렌더링함.
	if (numParticlesInBatch)
	{
		m_pGraphicDev->DrawPrimitive(
			D3DPT_POINTLIST,
			m_vbOffset,
			numParticlesInBatch);
	}

	// 다음블록
	m_vbOffset += m_vbBatchSize;
}

void CPtBuffer::Free()
{
	Safe_Release(m_pVB);
	m_pParticles = nullptr;
	CComponent::Free();
}

CPtBuffer * CPtBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPtBuffer* pInstance = new CPtBuffer(pGraphicDev);
	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CPtBuffer::Clone()
{
	return new CPtBuffer(*this);
}
