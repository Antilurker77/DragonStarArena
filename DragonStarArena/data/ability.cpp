// ================================================================
//
// ability.cpp
//
// ================================================================

#include "ability.hpp"

#include "abilityData.hpp"
#include "abilityEffect.hpp"
#include "gameData.hpp"
#include "../core/random.hpp"
#include "../entity/actor.hpp"

Ability::Ability() {
	abilityData = nullptr;
}

Ability::Ability(size_t abilityID) {
	Initialize(abilityID);
}

void Ability::Initialize(size_t abilityID) {
	abilityData = gameData.GetAbility(abilityID);

	if (abilityData != nullptr) {
		currentCharges = abilityData->MaxCharges;
		currentCooldown = 0;
	}
}

void Ability::Execute(Actor* user, std::vector<Actor*>& targets, BattleScene* battleScene) {
	CombatResult result;
	CombatOptions combatOptions = getCombatOptions();

	// If base hit chance is negative, pull from the characters weapon.
	if (combatOptions.BaseHitChance < 0) {
		// todo: weapons
		combatOptions.BaseHitChance = 8000;
	}

	if (!IsNull()) {
		for (size_t i = 0; i < abilityData->Effects.size(); i++) {
			AbilityEffect effect = abilityData->Effects[i];
			switch (effect.Effect) {
			case Effect::WeaponDamage:
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::WeaponDamage(user, targets[j], combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
				}
				break;
			case Effect::WeaponDamageAuraHit:
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::WeaponDamage(user, targets[j], combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
					if (result.DidHit && Random::RandomInt(1, 10000) <= effect.Arguments[3]) {
						Combat::AddAura(user, targets[j], static_cast<size_t>(effect.Arguments[1]), static_cast<int>(effect.Arguments[2]), battleScene);
					}
				}
				break;
			case Effect::AttackDamage:
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::AttackDamage(user, targets[j], combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
				}
				break;
			case Effect::SpellDamage:
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::SpellDamage(user, targets[j], combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
				}
				break;
			case Effect::SpellDamageAuraHit:
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::SpellDamage(user, targets[j], combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
					if (result.DidHit && Random::RandomInt(1, 10000) <= effect.Arguments[3]) {
						Combat::AddAura(user, targets[j], static_cast<size_t>(effect.Arguments[1]), static_cast<int>(effect.Arguments[2]), battleScene);
					}
				}
				break;
			case Effect::Healing:
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::Healing(user, targets[j], combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
				}
				break;
			case Effect::HealingPercent:
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::HealingPercent(user, targets[j], combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
				}
				break;
			default:
				break;
			}
		}

		user->Exhaust(GetUseTime(true, user));
		user->SpendMP(GetMPCost(true, user));
		user->SpendSP(GetSPCost(true, user));


		int cooldown = GetCooldown(true, user);
		if (cooldown > 0) {
			currentCharges--;
			if (currentCooldown == 0) {
				currentCooldown = cooldown;
			}
		}
	}
}

void Ability::DecrementCooldown(Actor* user) {
	if (abilityData != nullptr) {
		if (currentCooldown > 0) {
			currentCooldown--;
			if (currentCooldown == 0) {
				currentCharges++;
				if (currentCharges < abilityData->MaxCharges) {
					currentCooldown = GetCooldown(user);
				}
			}
		}
	}
}

void Ability::Refresh() {
	currentCooldown = 0;
	if (abilityData != nullptr) {
		currentCharges = abilityData->MaxCharges;
	}
}

bool Ability::IsNull() {
	return abilityData == nullptr;
}

bool Ability::IsUsable(Actor* user) {
	if (user != nullptr && abilityData != nullptr) {
		if (currentCharges == 0) {
			return false;
		}
		
		// todo: status effects like disarm/silence, ect.
		if (user->GetCurrentMP() < GetMPCost(false, user)) {
			return false;
		}

		if (user->GetCurrentSP() < GetSPCost(false, user)) {
			return false;
		}

		return true;
	}
	return false;
}

std::vector<ActorPtr> Ability::GetActorsInRange(Actor* user, std::vector<ActorPtr>& actors) {
	std::vector<ActorPtr> actorsInRange{};
	std::vector<ActorPtr> actorsToCheck{};

	if (abilityData != nullptr) {
		switch (abilityData->TargetType) {
		case TargetType::Undefined:
			break;
		case TargetType::User:
			for (size_t i = 0; i < actors.size(); i++) {
				if (actors[i].get() == user) {
					actorsInRange.push_back(actors[i]);
					return actorsInRange;
				}
			}
			// we somehow didn't find the user in the actor list
			return {};
		case TargetType::Ally:
			for (size_t i = 0; i < actors.size(); i++) {
				if (user->IsPlayer() == actors[i]->IsPlayer() && actors[i]->IsAlive()) {
					actorsToCheck.push_back(actors[i]);
				}
			}
			break;
		case TargetType::Enemy:
			for (size_t i = 0; i < actors.size(); i++) {
				if (user->IsPlayer() != actors[i]->IsPlayer() && actors[i]->IsAlive()) {
					actorsToCheck.push_back(actors[i]);
				}
			}
			break;
		default:
			break;
		}
		
		// Ranged abilities can target anyone regardless of row.
		if (abilityData->IsRanged) {
			actorsInRange = actorsToCheck;
		}
		// Melee abilities can only target certain enemies based
		// on the user's position.
		// Front Row: Any unit in front row, any unit in back if
		// front row is empty.
		// Back Row: Allied front row must be empty.
		else {
			if (user->IsInBackRow()) {
				// Check if allied row is empty.
				for (size_t i = 0; i < actors.size(); i++) {
					if (user->IsPlayer() == actors[i]->IsPlayer() && actors[i]->IsInFrontRow() && actors[i]->IsAlive()) {
						// Ally in front row, cannot use melee attack.
						return {};
					}
				}
			}
			// Check if there are enemies in the front row.
			for (size_t i = 0; i < actorsToCheck.size(); i++) {
				if (actorsToCheck[i]->IsInFrontRow()) {
					actorsInRange.push_back(actorsToCheck[i]);
				}
			}
			// If no enemies are in the front row, we can assume it's empty
			// and thus all back row enemies can be targeted.
			if (actorsInRange.empty()) {
				actorsInRange = actorsToCheck;
			}
		}
	}

	return actorsInRange;
}

std::vector<ActorPtr> Ability::GetActorsInArea(ActorPtr& mainTarget, std::vector<ActorPtr>& actors) {
	std::vector<ActorPtr> targets;
	if (abilityData != nullptr) {
		switch (abilityData->TargetScope) {
		case TargetScope::Undefined:
			break;
		case TargetScope::Single:
			targets.push_back(mainTarget);
			break;
		case TargetScope::Row:
			for (size_t i = 0; i < actors.size(); i++) {
				if (mainTarget->IsInFrontRow() == actors[i]->IsInFrontRow()) {
					targets.push_back(actors[i]);
				}
			}
			break;
		default:
			break;
		}
	}

	return targets;
}

size_t Ability::GetAbilityID() {
	if (abilityData != nullptr) {
		return abilityData->ID;
	}
	return 0;
}

std::string Ability::GetName() {
	if (abilityData != nullptr) {
		return abilityData->Name;
	}
	return "";
}

bool Ability::IsRanged() {
	if (abilityData != nullptr) {
		return abilityData->IsRanged;
	}
	return false;
}

int Ability::GetUseTime(bool consumeBuffs, Actor* actor) {
	int useTime = 0;
	CombatOptions co = getCombatOptions();

	if (abilityData != nullptr) {
		useTime = abilityData->UseTime;

		// negative usetime means to use weapon speed
		// todo: weapon speed calculations
		if (useTime < 0 && actor != nullptr) {
			useTime = actor->GetAttackSpeed() * useTime / -100;
		}
	}

	if (actor != nullptr) {
		int haste = static_cast<int>(actor->GetHaste(co, consumeBuffs));
		useTime = useTime * 10000 / haste;
	}

	return useTime;
}

int64_t Ability::GetMPCost(bool consumeBuffs, Actor* actor) {
	int64_t mpCost = 0;
	CombatOptions co = getCombatOptions();

	if (abilityData != nullptr) {
		mpCost = abilityData->MPCost;
	}

	if (actor != nullptr) {
		mpCost = mpCost * actor->GetMPCostMulti(co, consumeBuffs) / 10000;
	}

	return mpCost;
}

int64_t Ability::GetSPCost(bool consumeBuffs, Actor* actor) {
	int64_t spCost = 0;
	CombatOptions co = getCombatOptions();

	if (abilityData != nullptr) {
		spCost = abilityData->SPCost;
	}

	if (actor != nullptr) {
		spCost = spCost * actor->GetSPCostMulti(co, consumeBuffs) / 10000;
	}

	return spCost;
}

int Ability::GetCooldown(bool consumeBuffs, Actor* actor) {
	int cooldown = 0;
	CombatOptions co = getCombatOptions();

	if (abilityData != nullptr) {
		cooldown = abilityData->Cooldown;
	}

	if (actor != nullptr) {
		int flatCDR = static_cast<int>(actor->GetCooldownReduction(co, consumeBuffs));
		int recoveryRate = static_cast<int>(actor->GetCooldownRecoveryRate(co, consumeBuffs));
		cooldown -= flatCDR;
		cooldown = cooldown * 10000 / recoveryRate;
	}

	return cooldown;
}

std::vector<StatMod> Ability::GetStatMods() {
	if (abilityData != nullptr) {
		return abilityData->StatMods;
	}
	return {};
}

CombatOptions Ability::getCombatOptions() {
	CombatOptions co;

	if (abilityData != nullptr) {
		co.EventName = abilityData->Name;

		co.AbilityID = abilityData->ID;

		co.Categories = abilityData->Categories;
		co.Elements = abilityData->Elements;

		co.BaseHitChance = abilityData->BaseHitChance;

		co.CanDodge = abilityData->CanDodge;
		co.CanBlock = abilityData->CanBlock;
		co.CanCrit = abilityData->CanCrit;
		co.CanDoubleStrike = abilityData->CanDoubleStrike;
	}

	return co;
}