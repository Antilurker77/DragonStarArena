// ================================================================
//
// statModType.hpp
// 
// List of all stat modifiers.
//
// ================================================================

#pragma once

enum class StatModType {
	Undefined = 0,
	HP,
	HPMulti,
	HPPerLevel,
	HPRegen,
	HPLeech,
	MP,
	MPMulti,
	MPPerLevel,
	MPRegen,
	MPLeech,
	SP,
	SPMulti,
	SPPerLevel,
	SPRegen,
	SPLeech,
	DamageFlat,
	DamageMulti,
	HealingFlat,
	HealingMulti,
	AttackPower,
	AttackPowerMulti,
	SpellPower,
	SpellPowerMulti,
	HealingPower,
	HealingPowerMulti,
	Accuracy,
	CritChance,
	CritPower,
	CritProtection,
	CritPowerProtection,
	DoubleStrikeChance,
	DoubleStrikePower,
	Haste,
	Armor,
	ArmorMulti,
	ArmorPen,
	SpellDefense,
	SpellDefenseMulti,
	SpellDefensePen,
	Evasion,
	BlockChance,
	BlockPower,
	DamageTakenFlat,
	DamageTakenMulti,
	Resistance,
	HealingTakenFlat,
	HealingTakenMulti,
	CooldownReduction,
	CooldownRecoveryRate,
	MPCostMulti,
	SPCostMulti,
	AuraDurationFlat,
	AuraDurationMulti,
	GoldFind,
	EXPRate
};
