// ================================================================
//
// aura.hpp
// 
// Class for an individual instance of an aura.
//
// ================================================================

#pragma once

class Actor;
class BattleScene;
struct AuraData;

#include <memory>
#include <string>
#include <vector>
#include "abilityEffect.hpp"
#include "statMod.hpp"
#include "../core/combat.hpp"

typedef std::shared_ptr<Actor> ActorPtr;

class Aura {
public:
	Aura();
	Aura(size_t id, Actor* source, int stacks = 1);

	// Initializes the aura based on the given ID.
	void Initialize(size_t id, Actor* source, int stacks = 1);

	// Ticks down the duration of the aura, triggering OnTick
	// effects if applicable.
	void Tick(Actor* owner, BattleScene* battleScene);

	// Adds a stack if possible, and refreshes the aura to it's maximum duration.
	// Can also change ownership of the aura.
	void Refresh(Actor* newSource, int stacks = 1);

	// Marks this aura as being used in a stat calculation or
	// triggering an effect.
	void WasUsed(bool used);

	// Returns true if the aura has no stacks.
	bool IsExpired();

	// Returns the index of the actor that applied the aura.
	size_t GetSourceIndex();

	// Returns the ID of the aura.
	size_t GetAuraID();

	// Returns true if the aura is unique.
	bool IsUnique();

	// Returns true if the aura is unique by actor.
	bool IsUniqueByActor();

	// Returns the stat mods of the aura.
	std::vector<StatMod> GetStatMods();

private:
	// Gets combat options for this aura.
	CombatOptions getCombatOptions();

	// Triggers on tick effects.
	void onTick(Actor* owner, BattleScene* battleScene);

	// Triggers on expiry effects.
	void onExpiry(Actor* owner, BattleScene* battleScene);

	// Processes an ability effect.
	void processEffect(AbilityEffect& effect, CombatOptions& combatOptions, Actor* owner, BattleScene* battleScene);

	// ================================

	AuraData* auraData = nullptr;
	Actor* source = nullptr;
	size_t sourceIndex = 0;
	int currentDuration = 0;
	int currentNextTick = 0;
	int currentStacks = 0;
	bool wasUsed = false;
};