#include <a_samp>
#include "actor_robbery.inc"

main() {
	new actor = CreateActorRobbery(5, 192.168, 1.1, 1.10, 90, 1, 5000, 15000);

	new 
		Float:x,
		Float:y,
		Float:z,
		Float:ang,
		skinid,
		vwid,
		_min,
		_max;
	GetActorRobberyData(actor, skinid, x, y, z, ang, vwid, _min, _max);

	printf("actor, skinid, x, y, z, ang, min, max", actor, skinid, x, y, z, ang, vwid, _min, _max);
}	
