#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3		vPos;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;


	typedef struct tagVertexTex
	{
		_vec3		vPos;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTex
	{
		_vec3		vPos;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef	struct tagIndex16
	{
		_ushort			_0;
		_ushort			_1;
		_ushort			_2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;

	typedef struct tagTile
	{
		tagVertexTex*	pVertex = nullptr;
		tagIndex32*		pIndex= nullptr;
		BYTE				byOption = 0;
		BYTE				byDrawID = 0;
		_ulong				dwIndex = 0;
	}TILE;


	typedef struct Particle
	{
		_vec3		vPosition;	
		D3DCOLOR	tColor;		
								
	}PARTICLE;
	const _ulong		FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE; 
																	
											
	typedef struct Attribute
	{
		_vec3		_position;		
		_vec3		_velocity;		
		_vec3		_acceleration;	
		_float		_lifeTime;		
		_float		_age;			
		D3DXCOLOR	_color;			
		D3DXCOLOR	_colorFade;		
		_bool		_isAlive;		

	}ATTRIBUTE;

	typedef struct tBoundingBox
	{
		_vec3	vMin;
		_vec3	vMax;

		_bool isPointInside(_vec3 p)
		{
			if (p.x >= vMin.x && p.y >= vMin.y && p.z >= vMin.z &&
				p.x <= vMax.x && p.y <= vMax.y && p.z <= vMax.z)
				return true;
			else
				return false;
		}
	}BDBOX, *LPBDBOX;

	typedef struct tBoundingSphere
	{
		_vec3	vCenter;
		_float	fRadius;
	}BDSPHERE, *LPBDSPHERE;

	struct TexturePath
	{
		PDIRECT3DTEXTURE9 pTexture;
		_tchar*   wszFilePathName;
	};

	typedef struct tAttributeInput
	{
		tAttributeInput()
		{
			fLifeTime = 0.f;
			fAge = 0.f;
			bIsAlive = true;
		}

		_vec3			vPosition;		
		_vec3			vVelocity;		
		_vec3			vAcceleration;

		_float			fLifeTime;		
		_float			fAge;			

		D3DXCOLOR	tColor;		
		D3DXCOLOR	tColorFade;	

		_bool			bIsAlive;			
	} ATTINFO;

	typedef struct tParticleInfo 
	{
		_vec3			vOrigin;		
		BDBOX			tBdBox;			
		_float			fEmitRate;		
		_float			fSize;			
		_int			iMaxParticles;
		_ulong			dwVbSize;		
		_ulong			dwVbOffset;							
		_ulong			dwVbBatchSize;	
	} PINFO;

	typedef struct tagCreatureInfo
	{
		_int		iHp;
		_int		iAttack;

	}MONTSERINFO;

	typedef struct tPlayerInfo
	{
		_int		iHp;
		_int		iHpMax;
		_int		iLevel;
		_int		iExp;
		_int		iExpMax;
		_int		iHunger;
		_int		iAtk;
		_int		iDef;
		_float		fSpeed;

		_bool		bStun;
		_bool		bSlow;
		_float		fSlowSpeed;
	}PLAYERINFO;

	typedef struct tItemInfo
	{
		_int		iHp;	
		_int		iLevel;	
		_int		iExp;

		_int		iHpHeal = 0;	
		_int		iDef = 0;
		_float		fSpeed = 0.f;
		_int		iHunger = 0;
		_int		iAtk = 0;
	}ITEMINFO;
}


#endif // Engine_Struct_h__
