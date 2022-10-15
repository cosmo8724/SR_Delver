#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE	 { MODE_FULL, MODE_WIN };
	
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum RENDERID	{ RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum INFOID		{ INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATIONID { ROT_X, ROT_Y, ROT_Z, ROT_END};

	enum TEXTUREID	{ TEX_NORMAL, TEX_CUBE, TEX_END };

	enum LOADINGID	{ LOADING_TOOL, LOADING_STAGE, LOADING_BOSS, LOADING_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y,DIMS_Z,  DIMS_END }; 

	enum BULLETID	{ BULLET_WAND, BULLET_M_FIST, BULLET_SONGBOSS, STUN_SONGBOSS, FLOOR_SONGBOSS, LIGHTNING_SONGBOSS,
						BULLET_ARROW, BULLET_M_LEAF, BULLET_M_SPIDER, BULLET_END };

	enum EFFECTID	{ EFFECT_EXPLO, EFFECT_TEST, EFFECT_END };
	
	enum CubePlane	{ FRONT_X, BACK_X, FRONT_Y, BACK_Y, FRONT_Z, BACK_Z, CUBEPLANE_END };

	enum BLOCKTYPE	{ BLOCK_CAVE, BLOCK_COLD, BLOCK_DUNGEON, BLOCK_ROOM, BLOCK_SEWER, BLOCK_TEMPLE, BLOCKTYPE_END };

	enum PTYPE		{ PTYPE_FIREWORK, PTYPE_SNOW, PTYPE_LASER, PTYPE_SPOT, PTYPE_FOUNTAIN, PTYPE_REMAIN, PTYPE_TRACER, PTYPE_CIRCLING, PTYPE_MOOD, PTYPE_END };

	enum ITEMSTATE	{ STATE_GROUND, STATE_INV, STATE_EQUIP, STATE_QUICK, STATE_END };

	enum ITEMTYPE	{ ITEM_WEAPON, ITEM_POTION, ITEM_KEY, ITEM_ARMOR, ITEM_FIRE, ITEM_IMG, ITEM_END };

	enum WEAPONLIST { WEAPON_WAND, WEAPON_ARROW, WEAPON_DAGGER, WEAPON_END };

	enum WEAPONTYPE { WT_AD, WT_MELEE, WT_END };

	enum PTEXTUREID { TEXTURE_0, TEXTURE_1, TEXTURE_2, TEXTURE_3, TEXTURE_4, TEXTURE_5, TEXTURE_6, TEXTURE_7, TEXTURE_8,
						TEXTURE_9, TEXTURE10, TEXTURE_END };

	enum OBJECTTYPE { OBJ_PLAYER, OBJ_MONSTER, OBJ_NPC, OBJ_TERRAIN, OBJ_BLOCK, OBJ_END };

	enum ECOOBJTYPE { ECO_STONE, ECO_GRASS, ECO_TREE, ECO_JAR, ECO_JAM, ECO_LADDER, ECO_BONFIRE, ECO_END };
	
	enum POTIONTYPE { POTION_0, POTION_1, POTION_2, POTION_3, POTION_4, POTION_END };

	enum CAMERAID	{ CAM_DYNAMIC, CAM_STATIC, CAM_OBJECT, CAM_END };

	enum CAMACTION	{ ACTION_REVOL, ACTION_END };
	
	enum CHANNELID { SOUND_EFFECT, SOUND_BGM, SOUND_PLAYER, SOUND_MONSTER, SOUND_BOSS, MAXCHANNEL };
}

#endif // Engine_Enum_h__




