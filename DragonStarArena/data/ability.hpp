// ================================================================
//
// ability.hpp
// 
// Class for an individual instance of an ability.
//
// ================================================================

#pragma once

class Actor;
class BattleScene;
struct AbilityData;

#include <memory>
#include <string>
#include <vector>
#include "statMod.hpp"
#include "../core/combat.hpp"

typedef std::shared_ptr<Actor> ActorPtr;

class Ability {
public:
	Ability();
	Ability(size_t abilityID);

	// Initializes the ability based on the given ID.
	void Initialize(size_t abilityID);

	// Executes the ability on the given targets.
	void Execute(Actor* user, std::vector<Actor*>& targets, BattleScene* battleScene);

	// Decrements the cooldown of the ability, and lets it refill
	// charges if applicable.
	void DecrementCooldown(Actor* user);

	// Returns true if the ability is uninitialized.
	bool IsNull();

	// Returns true if the ability is ready to use.
	bool IsUsable(Actor* user);

	// Returns actors in range of the ability.
	std::vector<ActorPtr> GetActorsInRange(Actor* user, std::vector<ActorPtr>& actors);

	// Returns actors in the targeting area.
	std::vector<ActorPtr> GetActorsInArea(ActorPtr& mainTarget, std::vector<ActorPtr>& actors);

	// Returns the ID of the ability. Returns 0 if uninitialized.
	size_t GetAbilityID();

	// Returns the name of the ability. Returns an empty string if
	// uninitialized.
	std::string GetName();

	// Returns true if the ability is ranged. Returns false if the
	// ability is melee or unintialized.
	bool IsRanged();

	// Returns the recovery time of an ability. If an actor is provided,
	// will also take haste into account.
	int GetUseTime(bool consumeBuffs, Actor* actor = nullptr);

	// Returns the MP cost of the ability. If an actor is provided, it
	// will take MP Cost Rate into account.
	int64_t GetMPCost(bool consumeBuffs, Actor* actor = nullptr);

	// Returns the SP cost of the ability. If an actor is provided, it
	// will take SP Cost Rate into account.
	int64_t GetSPCost(bool consumeBuffs, Actor* actor = nullptr);

	// Returns the cooldown of the ability. If an actor is provided,
	// it will take the actor's cooldown recovery rate into account.
	int GetCooldown(bool consumeBuffs, Actor* actor = nullptr);

	// Returns the stat mods of the ability.
	std::vector<StatMod> GetStatMods();

private:
	// Builds the CombatOptions struct based on ability data.
	CombatOptions getCombatOptions();
	
	// ================================

	AbilityData* abilityData{};
	int currentCharges = 0;
	int currentCooldown = 0;
};