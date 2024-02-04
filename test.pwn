#include <a_samp>

#define ACTOR_ROBBERY_USE_STREAMER (false)
#include "actor_robbery.inc"

enum
{
    TEAM_COP = 10
};

main()
{
	// Create an actor in the 24/7 store
	new actorid = Robbery_CreateActor(189, -28.6813, -187.2011, 1003.5469, 0.0000, 0, 5000, 15000, 17);

	new 
		Float:x,
		Float:y,
		Float:z,
		Float:angle,
		skinid,
		virtualWorldId,
		interiorId,
		minMoney,
		maxMoney;

	Robbery_GetActorData(actorid, skinid, x, y, z, angle, virtualWorldId, minMoney, maxMoney, interiorId);

	printf("actorid = %d\n\
			skinid = %d\n\
			x = %.4f\n\
			y = %.4f\n\
			z = %.4f\n\
			angle = %.4f\n\
			virtualWorldId = %d\n\
			interiorId = %d\n\
			minMoney = %d\n\
			maxMoney = %d", 
		actorid, 
		skinid, 
		x, 
		y, 
		z, 
		angle, 
		virtualWorldId, 
		interiorId, 
		minMoney, 
		maxMoney);
}	

public OnPlayerRequestRobbery(playerid, actorid)
{
	if (GetPlayerTeam(playerid) == TEAM_COP)
	{
		SendClientMessage(playerid, -1, "Cops cannot rob stores.");
		return 0; // Must return 0 for the robbery to not commence.
	}
	return 1;
}

public OnPlayerFinishRobbery(playerid, actorid, robbedmoney, type)
{
	if (type == TYPE_SUCCESS)
	{
		GivePlayerMoney(playerid, robbedmoney);

		new string[64];
		format(string, sizeof(string), "You stole %d$ from the store!", robbedmoney);
		SendClientMessage(playerid, -1, string);
	}
	else if (type == TYPE_FAILED)
	{
		SetPlayerWantedLevel(playerid, 4);
		SendClientMessage(playerid, -1, "Rob failed!");
	}
	else if (type == TYPE_UNFINISHED)
	{
		SendClientMessage(playerid, -1, "Unfinished rob!");
	}
	return 1;
}
