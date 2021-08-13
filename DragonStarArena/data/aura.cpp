// ================================================================
//
// aura.cpp
//
// ================================================================

#include "aura.hpp"

#include "abilityEffect.hpp"
#include "auraData.hpp"
#include "gameData.hpp"
#include "../entity/actor.hpp"

Aura::Aura() {
	auraData = nullptr;
	source = nullptr;
}

Aura::Aura(size_t id, Actor* source, int stacks) {
	this->source = source;
	sourceIndex = source->GetIndex();

	Initialize(id, source, stacks);
}

void Aura::Initialize(size_t id, Actor* source, int stacks) {
	auraData = gameData.GetAura(id);
	if (auraData != nullptr) {
		CombatOptions co = getCombatOptions();

		currentDuration = auraData->BaseDuration;
		currentDuration = static_cast<int>(std::max(0ll, currentDuration + source->GetAuraDurationFlat(co, true)));
		currentDuration = currentDuration * static_cast<int>(source->GetAuraDurationMulti(co, true)) / 10000;

		currentNextTick = auraData->TickRate;
	}

	currentStacks = stacks;
}

void Aura::Tick(Actor* owner, BattleScene* battleScene) {
	if (auraData != nullptr) {
		if (currentNextTick > 0) {
			currentNextTick--;
			if (currentNextTick == 0) {
				onTick(owner, battleScene);
				currentNextTick = auraData->TickRate;
			}
		}

		if (currentDuration > 0) {
			currentDuration--;
			if (currentDuration == 0) {
				onExpiry(owner, battleScene);
				if (auraData->StacksExpireOneByOne) {
					CombatOptions co = getCombatOptions();

					currentStacks--;

					currentDuration = auraData->BaseDuration;
					currentDuration = static_cast<int>(std::max(0ll, currentDuration + source->GetAuraDurationFlat(co, true)));
					currentDuration = currentDuration * static_cast<int>(source->GetAuraDurationMulti(co, true)) / 10000;
				}
				else {
					currentStacks = 0;
				}
			}
		}

		if (wasUsed && auraData->ConsumeOnUse) {
			onExpiry(owner, battleScene);
			if (auraData->StacksConsumeOneByOne) {
				currentStacks--;
			}
			else {
				currentStacks = 0;
			}
		}
		wasUsed = false;
	}
}

void Aura::Refresh(Actor* newSource, int stacks) {
	CombatOptions co = getCombatOptions();
	source = newSource;
	sourceIndex = newSource->GetIndex();

	currentDuration = auraData->BaseDuration;
	currentDuration = static_cast<int>(std::max(0ll, currentDuration + source->GetAuraDurationFlat(co, true)));
	currentDuration = currentDuration * static_cast<int>(source->GetAuraDurationMulti(co, true)) / 10000;

	currentStacks = std::min(currentStacks + stacks, auraData->MaxStacks);
}

void Aura::WasUsed(bool used) {
	wasUsed = used;
}

bool Aura::IsExpired() {
	return currentStacks == 0;
}

size_t Aura::GetSourceIndex() {
	return sourceIndex;
}

size_t Aura::GetAuraID() {
	if (auraData != nullptr) {
		return auraData->ID;
	}
	return 0;
}

bool Aura::IsUnique() {
	if (auraData != nullptr) {
		return auraData->IsUnique;
	}
	return false;
}

bool Aura::IsUniqueByActor() {
	if (auraData != nullptr) {
		return auraData->IsUniqueByActor;
	}
	return false;
}

std::vector<StatMod> Aura::GetStatMods() {
	if (auraData != nullptr) {
		return auraData->StatMods;
	}
	return {};
}

CombatOptions Aura::getCombatOptions() {
	CombatOptions co;

	if (auraData != nullptr) {
		co.EventName = auraData->Name + " (Aura)";

		co.AuraID = auraData->ID;

		co.Categories = auraData->Categories;
		co.Elements = auraData->Elements;

		co.CanDodge = false;
		co.CanBlock = false;
		co.CanCrit = auraData->CanCrit;
		co.CanDoubleStrike = false;
	}

	return co;
}

void Aura::onTick(Actor* owner, BattleScene* battleScene) {
	CombatOptions combatOptions = getCombatOptions();

	for (size_t i = 0; i < auraData->OnTick.size(); i++) {
		AbilityEffect effect = auraData->OnTick[i];
		processEffect(effect, combatOptions, owner, battleScene);
	}
}

void Aura::onExpiry(Actor* owner, BattleScene* battleScene) {
	CombatOptions combatOptions = getCombatOptions();

	for (size_t i = 0; i < auraData->OnExpiry.size(); i++) {
		AbilityEffect effect = auraData->OnExpiry[i];
		processEffect(effect, combatOptions, owner, battleScene);
	}
}

void Aura::processEffect(AbilityEffect& effect, CombatOptions& combatOptions, Actor* owner, BattleScene* battleScene) {
	CombatResult result;
	switch (effect.Effect) {
	case Effect::WeaponDamage:
		result = Combat::WeaponDamage(source, owner, combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
		break;
	case Effect::AttackDamage:
		result = Combat::AttackDamage(source, owner, combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
		break;
	case Effect::SpellDamage:
		result = Combat::SpellDamage(source, owner, combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
		break;
	case Effect::Healing:
		result = Combat::Healing(source, owner, combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
		break;
	case Effect::HealingPercent:
		result = Combat::HealingPercent(source, owner, combatOptions, effect.Arguments[0], effect.Attribute, battleScene);
		break;
	default:
		break;
	}
}