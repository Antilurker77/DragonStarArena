// ================================================================
//
// actor.hpp
//
// Base class for the player and monster classes. Mostly handles
// stats and actions.
//
// ================================================================

#pragma once

class BattleScene;
enum class Attribute;
enum class BattlePosition;
enum class StatModType;

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "../data/ability.hpp"
#include "../data/aura.hpp"
#include "../data/tactic.hpp"

typedef std::shared_ptr<Actor> ActorPtr;

class Actor : public Entity {
public:
	// Returns the actor's name.
	std::string GetName();

	// Set's the actor's name.
	void SetName(std::string n);

	// Return the actor's index (position in vector).
	size_t GetIndex();

	// Set's the actor's index (position in vector).
	void SetIndex(size_t index);

	// Processes the actor's tactics and returns a pair containing the
	// ID of the ability and the targets. If the ability ID is 0, the
	// actor could find no valid action and should wait.
	std::pair<size_t, std::vector<Actor*>> CalcTactics(std::vector<ActorPtr>& actors);

	// Uses the ability with the specified ability ID, assuming the
	// actor has it.
	void UseAbility(std::vector<Actor*>& targets, size_t abilityID, BattleScene* battleScene);

	// Decrements the actor's exhaustion and ability cooldowns.
	void DecrementExhaustion(BattleScene* battleScene);

	// Deals damage to the actor. Should only be called after all
	// damage calculations are done.
	void TakeDamage(Attribute attribute, int64_t amount);

	// Heals the actor. Should only be called after all healing
	// calculations are done.
	void Heal(Attribute attribute, int64_t amount);

	// Attempts to add or refresh the specified aura to the actor.
	void AddAura(size_t auraID, Actor* source, int stacks = 1);

	// Removes auras from the actor that have expired.
	void RemoveExpiredAuras();

	// Removes all auras from this actor.
	void ClearAuras();

	// Spends the actor's MP. Should be called after MP cost calculations
	// are done.
	void SpendMP(int64_t amount);

	// Spends the actor's SP. Should be called after SP cost calculations
	// are done.
	void SpendSP(int64_t amount);

	// Sets the actor's battle position.
	void SetBattlePosition(BattlePosition p);

	// Adds exhaustion to the actor.
	void Exhaust(int amount);

	// Post-battle recovery. Restores all SP, refreshes cooldowns, and clears auras.
	void PostBattleRecovery();

	// Award EXP to the actor, and levels them up if possible.
	virtual void AwardEXP(int64_t exp) = 0;

	// Returns true if the actor is a player character.
	virtual bool IsPlayer() = 0;

	// Returns true if the actor is ready to act.
	bool IsReady();

	// Returns true if the actor is alive.
	bool IsAlive();

	// Returns the actor's current battle position.
	BattlePosition GetBattlePosition();

	// Returns true if the actor is in the front row.
	bool IsInFrontRow();

	// Returns true if the actor is in the back row.
	bool IsInBackRow();

	// Returns the actor's level.
	int GetLevel();

	// Returns the actor's exhaustion level.
	int GetExhaustion();

	// Returns the percent of a specified attribute an actor as.
	// 1 = 0.01%
	int64_t GetAttributePercent(Attribute attribute);

	// Returns the actor's current HP.
	int64_t GetCurrentHP();

	// Returns the actor's maximum HP.
	int64_t GetMaxHP();

	// Returns the actor's HP leech.
	int64_t GetHPLeech(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's current MP.
	int64_t GetCurrentMP();

	// Returns the actor's maximum MP.
	int64_t GetMaxMP();

	// Returns the actor's MP leech.
	int64_t GetMPLeech(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's current SP.
	int64_t GetCurrentSP();

	// Returns the actor's maximum SP.
	int64_t GetMaxSP();

	// Returns the actor's SP leech.
	int64_t GetSPLeech(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's flat added damage.
	int64_t GetDamageFlat(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's damage multiplier.
	int64_t GetDamageMulti(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's flat added healing.
	int64_t GetHealingFlat(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's healing multiplier.
	int64_t GetHealingMulti(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's attack power.
	int64_t GetAttackPower(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's spell power.
	int64_t GetSpellPower(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's healing power.
	int64_t GetHealingPower(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's accuracy.
	int64_t GetAccuracy(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's critical strike chance.
	int64_t GetCritChance(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's critical strike power.
	int64_t GetCritPower(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's critical strike chance protection.
	int64_t GetCritProtection(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's critical strike power protection.
	int64_t GetCritPowerProtection(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's double strike chance.
	int64_t GetDoubleStrikeChance(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's double strike power.
	int64_t GetDoubleStrikePower(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's haste.
	int64_t GetHaste(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's armor.
	int64_t GetArmor(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's armor penetration.
	int64_t GetArmorPen(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's spell defense.
	int64_t GetSpellDefense(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's spell defense penetration.
	int64_t GetSpellDefensePen(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's evasion.
	int64_t GetEvasion(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's block chance.
	int64_t GetBlockChance(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's block amount.
	int64_t GetBlockPower(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's flat amount of damage taken.
	int64_t GetDamageTakenFlat(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's damage taken multiplier.
	int64_t GetDamageTakenMulti(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's resistance.
	int64_t GetResistance(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's flat additional healing taken.
	int64_t GetHealingTakenFlat(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's healing taken multiplier.
	int64_t GetHealingTakenMulti(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's cooldown reduction.
	int64_t GetCooldownReduction(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's cooldown recovery rate.
	int64_t GetCooldownRecoveryRate(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's MP cost multiplier.
	int64_t GetMPCostMulti(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's SP cost multiplier.
	int64_t GetSPCostMulti(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's flat aura duration modifier.
	int64_t GetAuraDurationFlat(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's aura duration multiplier.
	int64_t GetAuraDurationMulti(CombatOptions& combatOptions, bool consumeBuffs);

	// Returns the actor's gold find multiplier.
	int64_t GetGoldFind();

	// Returns the actor's EXP rate multiplier.
	int64_t GetEXPRate();

	// Returns the attack speed of the actor's weapon. If a player is dual
	// wielding, the slowest weapon is used.
	virtual int GetAttackSpeed() = 0;

	// Returns the damage of the actor's main hand, including
	// damage gained from attack power.
	virtual int64_t GetMainHandDamage(CombatOptions& combatOptions, bool consumeBuffs) = 0;

	// Returns the amount of EXP the monster is worth. Players
	// return 0.
	virtual int64_t GetEXPDrop() = 0;

	// Returns the amount of gold the monster is worth. Players
	// return 0.
	virtual int64_t GetGoldDrop() = 0;

	// Returns the amount of loot points the monster is worth. Players
	// return 0.
	virtual int64_t GetLootPoints() = 0;

protected:
	// Returns the base stat for the designated stat type. Pulls from race
	// for players, monster data for monsters.
	virtual int64_t getBaseStat(StatModType statModType) = 0;

	// Returns the built in stat mods of the actor. This is race data for
	// players, monster data for monsters.
	virtual std::vector<StatMod> getBaseStatMods() = 0;

	// Returns stat from equipment. Monsters will return 0.
	virtual int64_t getStatFromEquipment(StatModType statModType, CombatOptions& combatOptions) = 0;

	// Checks stat modifiers for a given stat. Should be used for stats where
	// category and element checks don't make sense, like HP.
	int64_t getStat(int64_t base, StatModType statModType, bool isMultiplicative, bool consumeBuffs);

	// Checks stat modifiers for a given stat.
	int64_t getStat(int64_t base, StatModType statModType, CombatOptions& combatOptions, bool isMultiplicative, bool consumeBuffs);

	// Checks to see if a tactic is usable and has valid targets.
	bool isValidTactic(Tactic& tactic, std::vector<ActorPtr>& actors);

	// Filters targets that meet the condition of the tactic.
	std::vector<ActorPtr> filterTargets(Tactic& tactic, std::vector<ActorPtr>& targets);

	// ================================

	std::string name = "Unknown";
	size_t index = 0;
	int level = 1;

	int exhaustion = 0;

	BattlePosition battlePosition{};

	int64_t currentHP = 0ll;
	int64_t currentMP = 0ll;
	int64_t currentSP = 0ll;

	std::vector<Ability> abilities{};
	std::vector<Aura> auras{};
	std::vector<Tactic> tactics{};
};