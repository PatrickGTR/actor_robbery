# actor_robbery

[![sampctl](https://img.shields.io/badge/sampctl-actor_robbery-2f2f2f.svg?style=for-the-badge)](https://github.com/PatrickGTR/actor_robbery)

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.# actor_robbery

[![sampctl](https://img.shields.io/badge/sampctl-actor_robbery-2f2f2f.svg?style=for-the-badge)](https://github.com/PatrickGTR/actor_robbery)

Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

## Table of Contents

- [actor\_robbery](#actor_robbery)
	- [Table of Contents](#table-of-contents)
	- [Introduction](#introduction)
	- [Installation](#installation)
	- [Testing](#testing)
	- [Usage](#usage)

## Introduction

Inspired by GTA V store robbery.  **actor_robbery.inc** imitates it! You basically aim at the actor and the actor will play 3 animation sequence, the actor will either give you the money or you will leave empty handed.

## Installation

Simply install to your project:

```bash
sampctl install PatrickGTR/actor_robbery
```

Include in your code and begin using the library:

```pawn
#include <actor_robbery>
```

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```

## Usage

**Functions**
```
Function:
	Robbery_CreateActor

Info:
	Creates the robbery actor according to the position set.

Param:
	* skinid -> Skin ID of the robbery actor
	* Float:x -> Coordinate X of the robbery actor
	* Float:y -> Coordinate X of the robbery actor
	* Float:z -> Coordinate X of the robbery actor
	* Float:angle -> Facing angle of the robbery actor
	* actor_vwid -> virtual world id of the robbery actor
	* r_moneymin -> Minimum money to be robbed from the robbery actor
	* r_moneymax -> Maximum money to be robbed from the robbery actor
	* actor_interiorid -> interior id of the robbery actor
```

```
Function:
	Robbery_DestroyActor

Info:
	Destroys the robbery actor.

Param:
	* actorid -> ID of robbery actor you want to destroy.
```

```
Function:
	Robbery_GetActorData

Info:
	Retrieves the actor data

Param:
	* actorid -> ID of robbery actor you want to retrieve data from.
	* &skinid -> Skin ID of the robbery actor
	* &Float:x -> Coordinate X of the robbery actor
	* &Float:y -> Coordinate X of the robbery actor
	* &Float:z -> Coordinate X of the robbery actor
	* &Float:angle -> Facing angle of the robbery actor
	* &actor_vwid -> virtual world id of the robbery actor
	* &r_moneymin -> Minimum money to be robbed from the robbery actor
	* &r_moneymax -> Maximum money to be robbed from the robbery actor
	* &actor_interiorid -> interior id of the robbery actor
```

**Callbacks**
```
//OnPlayerRequestRobbery is called when the player aims at an actor. You can disallow a player from robbing by returning 0.
forward OnPlayerRequestRobbery(playerid, actorid);

//OnPlayerStartRobbery is called when the player aims at an actor after OnPlayerRequestRobbery allows the player to rob.
forward OnPlayerStartRobbery(playerid, actorid, robbed_recently);

//OnPlayerFinishRobbery is called when the 3 animation sequence has been played.
forward OnPlayerFinishRobbery(playerid, actorid, robbedmoney, type);
```
**Animation Sequence**

1. SHP_Rob_HandsUp
2. SHP_Rob_GiveCash
3. DUCK_cower
