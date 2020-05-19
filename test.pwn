#include <a_samp>
#include "actor_robbery.inc"

main() {
	new actor= Robbery_CreateActor(5, 192.168, 1.1, 1.10, 90, 1, 5000, 15000);

	new 
		Float:x,
		Float:y,
		Float:z,
		Float:ang,
		skinid,
		vwid,
		_min,
		_max;
	Robbery_GetActorData(actor, skinid, x, y, z, ang, vwid, _min, _max);

	printf("actor, skinid, x, y, z, ang, min, max", actor, skinid, x, y, z, ang, vwid, _min, _max);
}	

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	return 1;
}

public OnPlayerRequestRobbery(playerid, actorid) {
	new exampleName[24];
	GetPlayerName(playerid, exampleName, 24);
	
	if(!strcmp(exampleName, "COP"))
	{
		SendClientMessage(playerid, -1, "COP no ROB.");
		return 0; // Must return 0 for the robbery to not commence.
	}
	return 1;
}
