# actor_robbery.inc

**Introduction**
Inspired by GTA V store robbery.  **actor_robbery.inc** imitates it! You basically aim at the actor and the actor will play 3 animation sequence, the actor will either give you the money or you will leave empty handed. 

**Animation Sequence**
1. SHP_Rob_HandsUp
2. SHP_Rob_GiveCash
3. DUCK_cower


**Functions**

```
Function:
	CreateActorRobbery

Info:
	Creates the robbery actor according to the position set.

Param:
* skinid -> Skin ID of the robbery actor
* Float:x -> Coordinate X of the robbery actor
* Float:y -> Coordinate X of the robbery actor
* Float:z -> Coordinate X of the robbery actor
* Float:ang -> Facing angle of the robbery actor
* actor_vwid -> virtualid of the robbery actor
* r_moneymin -> Minimum money to be robbed from the robbery actor
* r_moneymax -> Maximum money to be robbed from the robbery actor

Function:
	GetActorRobberyData

Info:
	Retrieves the actor data

Param:
	* actorid -> ID of robbery actor you want to retrieve data from. 
	* &skinid -> Skin ID of the robbery actor
	* &Float:x -> Coordinate X of the robbery actor
	* &Float:y -> Coordinate X of the robbery actor
	* &Float:z -> Coordinate X of the robbery actor
	* &Float:ang -> Facing angle of the robbery actor
	* &actor_vwid -> virtualid of the robbery actor
	* &r_moneymin -> Minimum money to be robbed from the robbery actor
	* &r_moneymax -> Maximum money to be robbed from the robbery actor

Function: 
	GetActorFreeID

Info:	
	Retrieves the unused ID of an actor.

Param:
	None
```

**Callbacks**
````
//OnPlayerStartRobbery is called when the player aims at an actor.
forward OnPlayerStartRobbery(playerid, actorid, robbed_recently);

//OnPlayerFinishRobbery is called when the 3 animation sequence has been played.
forward OnPlayerFinishRobbery(playerid, actorid, robbedmoney, type);
```
