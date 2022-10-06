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
		if (lowBound >= highBound) // �߸��� �Է�
			return lowBound;

		// [0,1] ������ ���� float �� ��´�.
		_float f = (rand() % 10000) * 0.0001f;

		// [lowBound, highBound] ������ float�� ����
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
	virtual void			reset();										// �ý��� �� ��� ��ƼŬ �Ӽ� ����
	virtual void			resetParticle(ATTINFO* attribute) = 0;	// �� ��ƼŬ�� �Ӽ��� ����
	virtual void			addParticle();									// ��ƼŬ�� �߰�
	virtual void			update(_float fTimeDelta) = 0;			// ��ƼŬ ����

	virtual void			preRender();									// �������� �ռ� ������ �ʱ� ���� ����
	virtual void			render();										// ������
	virtual void			postRender();									// Ư�� ��ƼŬ �ý����� �������� �� �ִ� ���� ���¸� �����Ҷ� �̿�

public:
	_bool					isEmpty();										// ���� �ý��ۿ� ��ƼŬ�� ������ True
	_bool					isDead();										// �ý��� �� ��� ��ƼŬ�� ������� True

protected:
	virtual void			removeDeadParticles();						// ���� ��ƼŬ�� ����Ʈ���� ����
	virtual void			removeAllParticles();

protected:
	_vec3					m_origin;			// �ý����� ��õ. ��ƼŬ�� ���۵Ǵ� ��
	BDBOX					m_bdBox;			// ��ƼŬ�� �̵��� �� �ִ� ���� ����
	_float					m_fEmitRate;	// �ý��ۿ� ���ο� ��ƼŬ�� �߰��Ǵ� ����. (����/s)
	_float					m_fSize;			// �ý��� �� ��� ��ƼŬ�� ũ��

	CTexture*			m_pTextureCom = nullptr;
	CPtBuffer*			m_pBufferCom = nullptr;

	list<ATTINFO>		m_particles;		// �ý��� �� ��ƼŬ �Ӽ� ����Ʈ
												// ��ƼŬ ����/����/���� �� �� ����Ʈ�� �̿�
												// ��ƼŬ�� �׸� �غ� �Ϸ�Ǹ� ����Ʈ ��� �Ϻθ�
												// ���ؽ� ���۷� �����ϰ�, ��ƼŬ ����� ������ ��ħ.
												// ��� ��ƼŬ�� �׸������� �� ������ �ݺ�.
	_int					m_maxParticles;	// �־��� �ð����� �ý����� ���� �� �ִ� �ִ� ��ƼŬ ��

	ATTINFO				m_Attribute; //	 Attribute ���ÿ� �߰� ����

	_float					m_fFrame = 0.f;
};

