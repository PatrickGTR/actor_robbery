/*
	actor_robbery.inc [https://github.com/PatrickGTR/actor_robbery]

		Inspired by GTA V store robbery. actor_robbery.inc imitates it! 
		 You basically aim at the actor and the actor will play 3 animation sequence, 
		  the actor will either give you the money or you will leave empty handed.

		Author: 
			PatrickGTR

		Contributors:
			PatrickGTR
			madgenius0
			adib-yg

	Log Change
		2024-02-04
			Fixed actor IDs (adib-yg)
			Optimized the code, formatting and removed unnecessary things (adib-yg)
			Added new function [Robbery_DestroyActor] (adib-yg)
			Dynamic actors support [ACTOR_ROBBERY_USE_STREAMER] (adib-yg)
			Interior ID support only for dynamic actors (adib-yg)

*/

#if defined _actor_robbery_included
	#endinput
#endif
#define _actor_robbery_included

#if !defined _INC_open_mp
	#if !defined KEY
		#define KEY: _:
	#endif
#endif

/*
	Streamer (dynamic actors)
*/

#if !defined ACTOR_ROBBERY_USE_STREAMER
	#define ACTOR_ROBBERY_USE_STREAMER (false)
#endif

#if ACTOR_ROBBERY_USE_STREAMER
	#if !defined _streamer_included
		#include <streamer>
	#endif
#endif

/*
	Constants
*/

#if !defined MIN_MONEY_ROB
	#define MIN_MONEY_ROB (500)
#endif

#if !defined MAX_MONEY_ROB
	#define MAX_MONEY_ROB (10000)
#endif

#if !defined ROBBERY_WAIT_TIME
	#define ROBBERY_WAIT_TIME (5)
#endif

const TYPE_SUCCESS     = 0;
const TYPE_FAILED      = 1;
const TYPE_UNFINISHED  = 2;

/*
	Enumerators
*/

enum E_ACTOR_ROBBERY_DATA
{
	bool:E_ROB_ACTOR_VALID,
	E_ROB_ACTOR_SKIN_ID,
	Float:E_ROB_ACTOR_X,
	Float:E_ROB_ACTOR_Y,
	Float:E_ROB_ACTOR_Z,
	Float:E_ROB_ACTOR_FACING_ANGLE,
	E_ROB_ACTOR_VIRTUAL_WORLD_ID,
	E_ROB_ACTOR_INTERIOR_ID,
	E_ROB_ACTOR_MIN_MONEY,
	E_ROB_ACTOR_MAX_MONEY,
	E_ROB_ACTOR_ROBBED_RECENTLY
}

/*
	Variables
*/

static s_RobberyActorData[MAX_ACTORS][E_ACTOR_ROBBERY_DATA];

/*
	Forwards
*/

forward RunActorAnimationSequence(playerid, actorid, animation_pattern);
forward OnPlayerStartRobbery(playerid, actorid, bool:robbed_recently);
forward OnPlayerFinishRobbery(playerid, actorid, robbedmoney, type);
forward OnPlayerRequestRobbery(playerid, actorid);

/*
Robbery_CreateActor(skinid, Float:x, Float:y, Float:z, Float:angle, actor_vwid = 0, r_moneymin = MIN_MONEY_ROB, r_moneymax = MAX_MONEY_ROB, actor_interiorid = -1)
	Params
		* skinid - Skin ID of the robbery actor
		* Float:x - Coordinate X of the robbery actor
		* Float:y - Coordinate X of the robbery actor
		* Float:z - Coordinate X of the robbery actor
		* Float:angle - Facing angle of the robbery actor
		* actor_vwid - virtual world id of the robbery actor
		* r_moneymin - Minimum money to be robbed from the robbery actor
		* r_moneymax - Maximum money to be robbed from the robbery actor
		* actor_interiorid - interior id of the robbery actor
*/
stock Robbery_CreateActor(skinid, Float:x, Float:y, Float:z, Float:angle, actor_vwid = 0, r_moneymin = MIN_MONEY_ROB, r_moneymax = MAX_MONEY_ROB, actor_interiorid = -1)
{
	new actorid;

	#if !ACTOR_ROBBERY_USE_STREAMER
		actorid = CreateActor(skinid, x, y, z, angle);

		if (actorid == INVALID_ACTOR_ID)
		{
			print("ERROR: Robbery_CreateActor - MAX_ACTORS reached.");
			return INVALID_ACTOR_ID;
		}

		SetActorVirtualWorld(actorid, actor_vwid);
	#else
		actorid = CreateDynamicActor(skinid, x, y, z, angle, .worldid = actor_vwid, .interiorid = actor_interiorid);

		if (actorid == INVALID_STREAMER_ID)
		{
			print("ERROR: Robbery_CreateActor - Cannot create dynamic actor.");
			return INVALID_STREAMER_ID;
		}
	#endif

	s_RobberyActorData[actorid][E_ROB_ACTOR_VALID] = true;
	s_RobberyActorData[actorid][E_ROB_ACTOR_SKIN_ID] = skinid;
	s_RobberyActorData[actorid][E_ROB_ACTOR_X] = x;
	s_RobberyActorData[actorid][E_ROB_ACTOR_Y] = y;
	s_RobberyActorData[actorid][E_ROB_ACTOR_Z] = z;
	s_RobberyActorData[actorid][E_ROB_ACTOR_FACING_ANGLE] = angle;
	s_RobberyActorData[actorid][E_ROB_ACTOR_VIRTUAL_WORLD_ID] = actor_vwid;
	s_RobberyActorData[actorid][E_ROB_ACTOR_INTERIOR_ID] = actor_interiorid;
	s_RobberyActorData[actorid][E_ROB_ACTOR_MIN_MONEY] = r_moneymin;
	s_RobberyActorData[actorid][E_ROB_ACTOR_MAX_MONEY] = r_moneymax;

	return actorid;
}

/*
Robbery_DestroyActor(actorid)
	Params
		* actorid - ID of robbery actor you want to destroy.
*/
stock bool:Robbery_DestroyActor(actorid)
{
	if (!(0 <= actorid < MAX_ACTORS))
	{
		print("ERROR: Robbery_DestroyActor - Invalid actor id.");
		return false;
	}

	#if !ACTOR_ROBBERY_USE_STREAMER
		DestroyActor(actorid);
	#else
		DestroyDynamicActor(actorid);
	#endif

	s_RobberyActorData[actorid][E_ROB_ACTOR_VALID] = false;
	s_RobberyActorData[actorid][E_ROB_ACTOR_SKIN_ID] = 0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_X] = 0.0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_Y] = 0.0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_Z] = 0.0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_FACING_ANGLE] = 0.0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_VIRTUAL_WORLD_ID] = 0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_INTERIOR_ID] = 0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_MIN_MONEY] = 0;
	s_RobberyActorData[actorid][E_ROB_ACTOR_MAX_MONEY] = 0;

	return true;
}

/*
Robbery_GetActorData(actorid, &skinid, &Float:x, &Float:y, &Float:z, &Float:angle, &actor_vwid, &r_moneymin, &r_moneymax)
	Params
		* actorid - ID of robbery actor you want to retrieve data from.
		* &skinid - Skin ID of the robbery actor
		* &Float:x - Coordinate X of the robbery actor
		* &Float:y - Coordinate X of the robbery actor
		* &Float:z - Coordinate X of the robbery actor
		* &Float:angle - Facing angle of the robbery actor
		* &actor_vwid - virtual world id of the robbery actor
		* &r_moneymin - Minimum money to be robbed from the robbery actor
		* &r_moneymax - Maximum money to be robbed from the robbery actor
		* &actor_interiorid - interior id of the robbery actor
*/
stock bool:Robbery_GetActorData(actorid, &skinid, &Float:x, &Float:y, &Float:z, &Float:angle, &actor_vwid, &r_moneymin, &r_moneymax, &actor_interiorid)
{
	if (!(0 <= actorid < MAX_ACTORS))
	{
		print("ERROR: Robbery_GetActorData - Invalid actor id.");
		return false;
	}

	skinid = s_RobberyActorData[actorid][E_ROB_ACTOR_SKIN_ID];
	x = s_RobberyActorData[actorid][E_ROB_ACTOR_X];
	y = s_RobberyActorData[actorid][E_ROB_ACTOR_Y];
	z = s_RobberyActorData[actorid][E_ROB_ACTOR_Z];
	angle = s_RobberyActorData[actorid][E_ROB_ACTOR_FACING_ANGLE];
	actor_vwid = s_RobberyActorData[actorid][E_ROB_ACTOR_VIRTUAL_WORLD_ID];
	actor_interiorid = s_RobberyActorData[actorid][E_ROB_ACTOR_INTERIOR_ID];
	r_moneymin = s_RobberyActorData[actorid][E_ROB_ACTOR_MIN_MONEY];
	r_moneymax = s_RobberyActorData[actorid][E_ROB_ACTOR_MAX_MONEY];

	return true;
}

/*
	RunActorAnimationSequence
*/

public RunActorAnimationSequence(playerid, actorid, animation_pattern)
{
	switch (animation_pattern)
	{
		case 0:
		{
			#if !ACTOR_ROBBERY_USE_STREAMER
				ClearActorAnimations(actorid);
				ApplyActorAnimation(actorid, "SHOP", "SHP_Rob_HandsUp", 4.1, false, true, true, true, false);
			#else
				ClearDynamicActorAnimations(actorid);
				ApplyDynamicActorAnimation(actorid, "SHOP", "SHP_Rob_HandsUp", 4.1, false, true, true, true, false);
			#endif

			SetTimerEx("RunActorAnimationSequence", 1000 * ROBBERY_WAIT_TIME, false, "iii", playerid, actorid, 1);

			for (new i = 0; i < MAX_PLAYERS; i++)
			{
				if (!IsPlayerConnected(i))
				{
					continue;
				}
				if (!IsPlayerInRangeOfPoint(i, 30.0, s_RobberyActorData[actorid][E_ROB_ACTOR_X], s_RobberyActorData[actorid][E_ROB_ACTOR_Y], s_RobberyActorData[actorid][E_ROB_ACTOR_Z]) || GetPlayerVirtualWorld(i) != s_RobberyActorData[actorid][E_ROB_ACTOR_VIRTUAL_WORLD_ID])
				{
					continue;
				}
				PlayerPlaySound(i, 3401, s_RobberyActorData[actorid][E_ROB_ACTOR_X], s_RobberyActorData[actorid][E_ROB_ACTOR_Y], s_RobberyActorData[actorid][E_ROB_ACTOR_Z]);
			}
		}
		case 1:
		{
			if (!IsPlayerInRangeOfPoint(playerid, 10.0, s_RobberyActorData[actorid][E_ROB_ACTOR_X], s_RobberyActorData[actorid][E_ROB_ACTOR_Y], s_RobberyActorData[actorid][E_ROB_ACTOR_Z]))
			{
				CallLocalFunction("OnPlayerFinishRobbery", "iiii", playerid, actorid, 0, TYPE_UNFINISHED);
			}
			else
			{
				#if !ACTOR_ROBBERY_USE_STREAMER
					ClearActorAnimations(actorid);
					ApplyActorAnimation(actorid, "SHOP", "SHP_Rob_GiveCash", 4.1, false, true, true, true, false);
				#else
					ClearDynamicActorAnimations(actorid);
					ApplyDynamicActorAnimation(actorid, "SHOP", "SHP_Rob_GiveCash", 4.1, false, true, true, true, false);
				#endif

				SetTimerEx("RunActorAnimationSequence", 1000 * ROBBERY_WAIT_TIME, false, "iii", playerid, actorid, 2);
			}
		}
		case 2:
		{
			#if !ACTOR_ROBBERY_USE_STREAMER
				ClearActorAnimations(actorid);
				ApplyActorAnimation(actorid, "PED", "DUCK_cower", 4.1, true, true, true, true, true);
			#else
				ClearDynamicActorAnimations(actorid);
				ApplyDynamicActorAnimation(actorid, "PED", "DUCK_cower", 4.1, true, true, true, true, true);
			#endif

			SetTimerEx("RunActorAnimationSequence", 1000 * 60 * ROBBERY_WAIT_TIME, false, "iii", playerid, actorid, 3);

			new robberyChance = random(100);
			if (robberyChance > 40)
			{
				CallLocalFunction("OnPlayerFinishRobbery", "iiii", playerid, actorid, (random(s_RobberyActorData[actorid][E_ROB_ACTOR_MAX_MONEY] - s_RobberyActorData[actorid][E_ROB_ACTOR_MIN_MONEY]) + s_RobberyActorData[actorid][E_ROB_ACTOR_MIN_MONEY]), TYPE_SUCCESS);
			}
			else
			{
				CallLocalFunction("OnPlayerFinishRobbery", "iiii", playerid, actorid, 0, TYPE_FAILED);
			}
		}
		case 3:
		{
			#if !ACTOR_ROBBERY_USE_STREAMER
				ClearActorAnimations(actorid);
			#else
				ClearDynamicActorAnimations(actorid);
			#endif

			PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
		}
	}
	return 1;
}

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	EnablePlayerCameraTarget(playerid, true);

	ApplyAnimation(playerid, "PED", "null", 0.0, false, false, false, false, 0);
	ApplyAnimation(playerid, "SHOP", "null", 0.0, false, false, false, false, 0);

	#if defined ActorRob_OnPlayerConnect
		return ActorRob_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect ActorRob_OnPlayerConnect
#if defined ActorRob_OnPlayerConnect
    forward ActorRob_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerKeyStateChange
*/

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if ((newkeys & KEY_HANDBRAKE) == KEY_HANDBRAKE && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		switch (GetPlayerWeapon(playerid))
		{
			case WEAPON_COLT45..WEAPON_RIFLE:
			{
				new actorid;
				
				#if !ACTOR_ROBBERY_USE_STREAMER
					actorid = GetPlayerCameraTargetActor(playerid);
					
					if (actorid == INVALID_ACTOR_ID)
					{
						return 1;
					}
				#else
					actorid = GetPlayerCameraTargetDynActor(playerid);

					if (actorid == INVALID_STREAMER_ID)
					{
						return 1;
					}
				#endif

				if (!s_RobberyActorData[actorid][E_ROB_ACTOR_VALID])
				{
					// this fixes the issue with normal create actors.
					// there was a bug that you aim at an actor and rob it even though it wasn't created by actor_robbery.
					return 1;
				}

				if (!OnPlayerRequestRobbery(playerid, actorid))
				{
					return 1;
				}

				if (gettime() - s_RobberyActorData[actorid][E_ROB_ACTOR_ROBBED_RECENTLY] < 60 * ROBBERY_WAIT_TIME)
				{
					return CallLocalFunction("OnPlayerStartRobbery", "iii", playerid, actorid, true);
				}

				s_RobberyActorData[actorid][E_ROB_ACTOR_ROBBED_RECENTLY] = gettime();
				RunActorAnimationSequence(playerid, actorid, 0);
				CallLocalFunction("OnPlayerStartRobbery", "iii", playerid, actorid, false);
			}
		}
	}
   	#if defined ActorRob_OnPlayerKeyStateChange
		return ActorRob_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
   		return 1;
	#endif
}

#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange ActorRob_OnPlayerKeyStateChange

#if defined ActorRob_OnPlayerKeyStateChange
	forward ActorRob_OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys);
#endif

/* 
	OnPlayerRequestRobbery
		Return 0 to disallow the robbery.
		See test.pwn for example.
*/

public OnPlayerRequestRobbery(playerid, actorid)
{
	#if defined ActorRob_OnPlayerRequestRobbery
		return ActorRob_OnPlayerRequestRobbery(playerid, actorid);
	#else
		return 1; // Default version always wants to returns 1 to allow robbery
	#endif
}

#if defined _ALS_OnPlayerRequestRobbery
    #undef OnPlayerRequestRobbery
#else
    #define _ALS_OnPlayerRequestRobbery
#endif
#define OnPlayerRequestRobbery ActorRob_OnPlayerRequestRobbery
#if defined ActorRob_OnPlayerRequestRobbery
    forward ActorRob_OnPlayerRequestRobbery(playerid);
#endif

/*
	Deprecated functions
*/

#pragma deprecated Use `Robbery_GetActorData`
stock GetActorRobberyData(actorid, &skinid, &Float:x, &Float:y, &Float:z, &Float:ang, &actor_vwid, &r_moneymin, &r_moneymax) {
	#pragma unused actorid, skinid, x, y, z, ang, actor_vwid, r_moneymin, r_moneymax
}

#pragma deprecated Use `Robbery_CreateActor`
stock CreateActorRobbery(skinid, Float:x, Float:y, Float:z, Float:ang, actor_vwid = 0, r_moneymin = MIN_MONEY_ROB, r_moneymax = MAX_MONEY_ROB) {
	#pragma unused skinid, x, y, z, ang, actor_vwid, r_moneymin, r_moneymax
	return -1;
}
