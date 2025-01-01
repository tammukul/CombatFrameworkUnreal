// M T All rights Reserved

#pragma once


UENUM(Blueprintable)
enum class ECF_ActionState : uint8
{
	Idle,
	Aggressive,
	Dodge,
	Attack,
	Hit,
	Finisher,
	Dead,
};

UENUM(Blueprintable)
enum class ECF_HitType : uint8
{
	Normal,
	Unconscious,
	Stun,
	Knockdown,
	Dead,
};