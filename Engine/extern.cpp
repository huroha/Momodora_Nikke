#include "pch.h"
#include "global.h"


TransInfo g_Trans = {};
GlobalData g_Data = {};


const char* ASSET_TYPE_STRING[] =
{
	"MESH",
	"MESH_DATA",
	"TEXTURE",
	"SOUND",
	"PREFAB",
	"FLIPBOOK",
	"SPRITE",
	"MATERIAL",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
};

const char* layerNames[32] =
{
    "Background_0",
    "Player_Land_1",
    "Default_2",
    "Player_3",
    "PlayerObject_4",
    "Monster_Land_5",
    "MonsterObject_6",
    "Camera_7",
    "Collider_Land_8",
    "Tile_9",
    "BG_InterAction_10",
    "Water_11",
    "PlayerAttack_12",
    "MonsterDetect_13",
    "Monster_Body_14",
    "SlopeCollider_15",
    "CeilingCheck_16",
    "undetermined_17",
    "undetermined_18",
    "undetermined_19",
    "undetermined_20",
    "undetermined_21",
    "undetermined_22",
    "undetermined_23",
    "undetermined_24",
    "undetermined_25",
    "undetermined_26",
    "undetermined_27",
    "PauseUI_28",
    "undetermined_29",
    "undetermined_30",
    "UI_ObjectOnly"
};