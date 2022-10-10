#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE	 { MODE_FULL, MODE_WIN };
	
// ID_DYNAMIC : 매 프레임마다 갱신을 해야하는 컴포넌트 집단
// ID_STATIC : 한 번 만들면 그대로 사용할 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum INFOID { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATIONID { ROT_X, ROT_Y, ROT_Z, ROT_END};

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum LOADINGID { LOADING_TOOL, LOADING_STAGE, LOADING_BOSS, LOADING_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y,DIMS_Z,  DIMS_END }; 

	enum BULLETID { BULLET_WAND, BULLET_M_FIST, BULLET_SONGBOSS, STUN_SONGBOSS, FLOOR_SONGBOSS, 
						BULLET_ARROW, BULLET_M_LEAF, BULLET_END };

	enum EFFECTID { EFFECT_EXPLO, EFFECT_TEST, EFFECT_END };
	
	enum CubePlane { FRONT_X, BACK_X, FRONT_Y, BACK_Y, FRONT_Z, BACK_Z, CUBEPLANE_END };

	enum BLOCKTYPE { BLOCK_CAVE, BLOCK_COLD, BLOCK_DUNGEON, BLOCK_ROOM, BLOCK_SEWER, BLOCK_TEMPLE, BLOCKTYPE_END };

	enum PTYPE { PTYPE_FIREWORK, PTYPE_SNOW, PTYPE_LASER, PTYPE_SPOT, PTYPE_FOUNTAIN, PTYPE_REMAIN, PTYPE_TRACER, PTYPE_END };

	enum ITEMSTATE { STATE_GROUND, STATE_INV, STATE_EQUIP, STATE_END };

	enum ITEMTYPE { ITEM_WEAPON, ITEM_POTION, ITEM_IMG, ITEM_END };

	enum WEAPONLIST { WEAPON_WAND, WEAPON_ARROW, WEAPON_DAGGER, WEAPON_END };

	enum PTEXTUREID { TEXTURE_0, TEXTURE_1, TEXTURE_2, TEXTURE_3, TEXTURE_4, TEXTURE_5, TEXTURE_6, TEXTURE_7, TEXTURE_8,
						TEXTURE_9, TEXTURE10, TEXTURE_END };

	enum OBJECTTYPE { OBJ_PLAYER, OBJ_MONSTER, OBJ_NPC, OBJ_TERRAIN, OBJ_BLOCK, OBJ_END };

	enum ECOOBJTYPE { ECO_STONE, ECO_GRASS, ECO_TREE, ECO_JAR, ECO_JAM, ECO_END };
}

#endif // Engine_Enum_h__




