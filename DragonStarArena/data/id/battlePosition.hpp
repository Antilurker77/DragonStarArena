// ================================================================
//
// battlePosition.hpp
// 
// List of positions an actor can have in battle.
//
// ================================================================

#pragma once

enum class BattlePosition {
	Undefined = 0,
	FrontFarLeft,
	FrontLeft,
	FrontCenter,
	FrontRight,
	FrontFarRight,
	BackFarLeft,
	BackLeft,
	BackCenter,
	BackRight,
	BackFarRight
};