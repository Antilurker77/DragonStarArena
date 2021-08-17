// ================================================================
//
// actor.cpp
//
// ================================================================

#include "actor.hpp"

#include <algorithm>
#include <iostream>
#include "../core/random.hpp"
#include "../data/statMod.hpp"
#include "../data/id/aiCondition.hpp"
#include "../data/id/aiTarget.hpp"
#include "../data/id/attribute.hpp"
#include "../data/id/battlePosition.hpp"
#include "../data/id/statModType.hpp"

std::string Actor::GetName() {
	if (name.empty()) {
		return "Undefined";
	}
	return name;
}

void Actor::SetName(std::string n) {
	name = n;
}

size_t Actor::GetIndex() {
	return index;
}

void Actor::SetIndex(size_t index) {
	this->index = index;
}

std::pair<size_t, std::vector<Actor*>> Actor::CalcTactics(std::vector<ActorPtr>& actors) {
	std::pair<size_t, std::vector<Actor*>> result;
	result.first = 0;

	size_t ratingLevel = 0;
	size_t highestRating = 0;
	std::vector<Tactic> validTactics{};
	Tactic choosenTactic;

	// Get the highest rating amomg all tactics.
	for (size_t i = 0; i < tactics.size(); i++) {
		if (tactics[i].Rating > highestRating) {
			highestRating = tactics[i].Rating;
		}
	}

	while (validTactics.empty() && ratingLevel <= highestRating) {
		std::vector<Tactic> tacticsAtRating{};
		for (size_t i = 0; i < tactics.size(); i++) {
			if (tactics[i].Rating == ratingLevel) {
				tacticsAtRating.push_back(tactics[i]);
			}
		}
		if (!tacticsAtRating.empty()) {
			// Check to see which tactics have usable abilities and valid targets.
			for (size_t i = 0; i < tacticsAtRating.size(); i++) {
				if (isValidTactic(tacticsAtRating[i], actors)) {
					validTactics.push_back(tacticsAtRating[i]);
				}
			}
		}

		ratingLevel++;
	}

	// If there are multiple valid tactics, randomly select one.
	if (validTactics.size() > 1) {
		int weightSum = 0;
		for (size_t i = 0; i < validTactics.size(); i++) {
			weightSum += validTactics[i].Weight;
		}
		int roll = Random::RandomInt(1, weightSum);
		for (size_t i = 0; i < validTactics.size(); i++) {
			roll -= validTactics[i].Weight;
			if (roll <= 0) {
				choosenTactic = validTactics[i];
				break;
			}
		}
	}
	else if (validTactics.size() == 1) {
		choosenTactic = validTactics[0];
	}
	else {
		return result;
	}

	Ability* ability = nullptr;
	for (size_t i = 0; i < abilities.size(); i++) {
		if (abilities[i].GetAbilityID() == choosenTactic.AbilityID) {
			ability = &abilities[i];
			break;
		}
	}

	// Finally, select targets.
	auto possibleTargets = ability->GetActorsInRange(this, actors);
	possibleTargets = filterTargets(choosenTactic, possibleTargets);
	size_t index = Random::RandomSizeT(0, possibleTargets.size() - 1);

	result.first = choosenTactic.AbilityID;

	auto targetsInArea = ability->GetActorsInArea(possibleTargets[index], possibleTargets);
	for (size_t i = 0; i < targetsInArea.size(); i++) {
		result.second.push_back(targetsInArea[i].get());
	}

	return result;
}

void Actor::UseAbility(std::vector<Actor*>& targets, size_t abilityID, BattleScene* battleScene) {
	for (size_t i = 0; i < abilities.size(); i++) {
		if (abilityID == abilities[i].GetAbilityID()) {
			abilities[i].Execute(this, targets, battleScene);
			break;
		}
	}
}

void Actor::DecrementExhaustion(BattleScene* battleScene) {
	if (exhaustion > 0) {
		exhaustion--;
		for (size_t i = 0; i < abilities.size(); i++) {
			abilities[i].DecrementCooldown(this);
		}
		for (size_t i = 0; i < auras.size(); i++) {
			auras[i].Tick(this, battleScene);
		}
	}

	RemoveExpiredAuras();
}

void Actor::TakeDamage(Attribute attribute, int64_t amount) {
	if (amount > 0) {
		switch (attribute) {
		case Attribute::HP:
			currentHP = std::max(0ll, currentHP - amount);
			break;
		default:
			break;
		}
	}
}

void Actor::Heal(Attribute attribute, int64_t amount) {
	if (amount > 0) {
		switch (attribute) {
		case Attribute::HP:
			currentHP = std::min(currentHP + amount, GetMaxHP());
			break;
		default:
			break;
		}
	}
}

void Actor::AddAura(size_t auraID, Actor* source, int stacks) {
	size_t sourceIndex = source->GetIndex();
	for (size_t i = 0; i < auras.size(); i++) {
		if (auras[i].GetAuraID() == auraID) {
			// Unique Aura: Refresh duration and change ownership.
			if (auras[i].IsUnique()) {
				auras[i].Refresh(source, stacks);
				return;
			}
			// Unique By Actor Aura: Refresh duration if source actors match.
			else if (auras[i].IsUniqueByActor() && auras[i].GetSourceIndex() == source->GetIndex()) {
				auras[i].Refresh(source, stacks);
				return;
			}
		}
	}

	// Did not match existing auras, safe to add.
	Aura aura(auraID, source, stacks);
	auras.push_back(aura);
}

void Actor::RemoveExpiredAuras() {
	if (!auras.empty()) {
		auto removed = std::remove_if(auras.begin(), auras.end(), [&](Aura& au) {
			if (au.IsExpired()) {
				return true;
			}
			return false;
		});
		auras.erase(removed, auras.end());
	}
}

void Actor::ClearAuras() {
	auras.clear();
}

void Actor::SpendMP(int64_t amount) {
	currentMP = std::max(0ll, currentMP - amount);
}

void Actor::SpendSP(int64_t amount) {
	currentSP = std::max(0ll, currentSP - amount);
}

void Actor::SetBattlePosition(BattlePosition p) {
	battlePosition = p;
}

void Actor::Exhaust(int amount) {
	exhaustion += std::max(0, amount);
}

void Actor::PostBattleRecovery() {
	exhaustion = 0;
	currentSP = GetMaxSP();

	for (size_t i = 0; i < abilities.size(); i++) {
		abilities[i].Refresh();
	}

	ClearAuras();
}

bool Actor::IsReady() {
	if (currentHP <= 0) {
		return false;
	}

	if (exhaustion > 0) {
		return false;
	}

	// todo: stuns

	return true;
}

bool Actor::IsAlive() {
	return currentHP > 0;
}

BattlePosition Actor::GetBattlePosition() {
	return battlePosition;
}

bool Actor::IsInFrontRow() {
	std::vector<BattlePosition> frontRow{ BattlePosition::FrontFarLeft, BattlePosition::FrontLeft, BattlePosition::FrontCenter, BattlePosition::FrontRight, BattlePosition::FrontFarRight };
	return std::find(frontRow.begin(), frontRow.end(), battlePosition) != frontRow.end();
}

bool Actor::IsInBackRow() {
	std::vector<BattlePosition> backRow{ BattlePosition::BackFarLeft, BattlePosition::BackLeft, BattlePosition::BackCenter, BattlePosition::BackRight, BattlePosition::BackFarRight };
	return std::find(backRow.begin(), backRow.end(), battlePosition) != backRow.end();
}

int Actor::GetLevel() {
	return std::max(1, level);
}

int Actor::GetExhaustion() {
	return std::max(0, exhaustion);
}

int64_t Actor::GetAttributePercent(Attribute attribute) {
	int64_t result = 0;

	switch (attribute) {
	case Attribute::HP:
		result = currentHP * 10000 / GetMaxHP();
		break;
	case Attribute::MP:
		result = currentMP * 10000 / GetMaxMP();
		break;
	case Attribute::SP:
		result = currentSP * 10000 / GetMaxSP();
		break;
	default:
		break;
	}

	return result;
}

int64_t Actor::GetCurrentHP() {
	return std::max(0ll, currentHP);
}

int64_t Actor::GetMaxHP() {
	int64_t result = getBaseStat(StatModType::HP);
	int64_t multi = 10000ll;
	int64_t perLevel = 0ll;

	result = getStat(result, StatModType::HP, false, false);
	perLevel = getStat(perLevel, StatModType::HPPerLevel, false, false);
	multi = getStat(multi, StatModType::HPMulti, true, false);

	result = (result + (perLevel * level)) * multi / 10000ll;

	return std::max(1ll, result);
}

int64_t Actor::GetHPLeech(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::HPLeech, combatOptions, false, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetCurrentMP() {
	return std::max(0ll, currentMP);
}

int64_t Actor::GetMaxMP() {
	int64_t result = getBaseStat(StatModType::MP);
	int64_t multi = 10000ll;
	int64_t perLevel = 0ll;

	result = getStat(result, StatModType::MP, false, false);
	perLevel = getStat(perLevel, StatModType::MPPerLevel, false, false);
	multi = getStat(multi, StatModType::MPMulti, true, false);

	result = (result + (perLevel * level)) * multi / 10000ll;

	return std::max(1ll, result);
}

int64_t Actor::GetMPLeech(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::MPLeech, combatOptions, false, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetCurrentSP() {
	return std::max(0ll, currentSP);
}

int64_t Actor::GetMaxSP() {
	int64_t result = getBaseStat(StatModType::SP);
	int64_t multi = 10000ll;
	int64_t perLevel = 0ll;

	result = getStat(result, StatModType::SP, false, false);
	perLevel = getStat(perLevel, StatModType::SPPerLevel, false, false);
	multi = getStat(multi, StatModType::SPMulti, true, false);

	result = (result + (perLevel * level)) * multi / 10000ll;

	return std::max(1ll, result);
}

int64_t Actor::GetSPLeech(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::SPLeech, combatOptions, false, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetDamageFlat(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::DamageFlat, combatOptions, false, consumeBuffs);

	return result;
}

int64_t Actor::GetDamageMulti(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::DamageMulti, combatOptions, true, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetHealingFlat(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::HealingFlat, combatOptions, false, consumeBuffs);

	return result;
}

int64_t Actor::GetHealingMulti(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::HealingMulti, combatOptions, true, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetAttackPower(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = getBaseStat(StatModType::AttackPower);
	int64_t multi = 10000ll;

	result = getStat(result, StatModType::AttackPower, combatOptions, false, consumeBuffs);
	multi = getStat(multi, StatModType::AttackPowerMulti, combatOptions, true, consumeBuffs);

	result = result * multi / 10000ll;

	return std::max(0ll, result);
}

int64_t Actor::GetSpellPower(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = getBaseStat(StatModType::SpellPower);
	int64_t multi = 10000ll;

	result = getStat(result, StatModType::SpellPower, combatOptions, false, consumeBuffs);
	multi = getStat(multi, StatModType::SpellPowerMulti, combatOptions, true, consumeBuffs);

	result = result * multi / 10000ll;

	return std::max(0ll, result);
}

int64_t Actor::GetHealingPower(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = getBaseStat(StatModType::HealingPower);
	int64_t multi = 10000ll;

	result = getStat(result, StatModType::HealingPower, combatOptions, false, consumeBuffs);
	multi = getStat(multi, StatModType::HealingPowerMulti, combatOptions, true, consumeBuffs);

	result = result * multi / 10000ll;

	return std::max(0ll, result);
}

int64_t Actor::GetAccuracy(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = getBaseStat(StatModType::Accuracy);

	result = getStat(result, StatModType::Accuracy, combatOptions, false, consumeBuffs);

	return std::max(1ll, result);
}

int64_t Actor::GetCritChance(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 500ll;

	result = getStat(result, StatModType::CritChance, combatOptions, false, consumeBuffs);

	return std::clamp(result, 0ll, 10000ll);
}

int64_t Actor::GetCritPower(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 15000ll;

	result = getStat(result, StatModType::CritPower, combatOptions, true, consumeBuffs);

	return result;
}

int64_t Actor::GetCritProtection(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::CritProtection, combatOptions, false, consumeBuffs);

	return std::clamp(result, 0ll, 10000ll);
}

int64_t Actor::GetCritPowerProtection(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::CritPowerProtection, combatOptions, false, consumeBuffs);

	return std::max(result, 0ll);
}

int64_t Actor::GetDoubleStrikeChance(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::DoubleStrikeChance, combatOptions, false, consumeBuffs);

	return std::clamp(result, 0ll, 10000ll);
}

int64_t Actor::GetDoubleStrikePower(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::DoubleStrikePower, combatOptions, true, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetHaste(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::Haste, combatOptions, true, consumeBuffs);

	return std::max(1ll, result);
}

int64_t Actor::GetArmor(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = getBaseStat(StatModType::Armor);
	int64_t multi = 10000ll;

	result = getStat(result, StatModType::Armor, combatOptions, false, consumeBuffs);
	multi = getStat(multi, StatModType::ArmorMulti, combatOptions, true, consumeBuffs);

	result = result * multi / 10000ll;

	return std::max(0ll, result);
}

int64_t Actor::GetArmorPen(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::ArmorPen, combatOptions, false, consumeBuffs);

	return std::clamp(result, 0ll, 10000ll);
}

int64_t Actor::GetSpellDefense(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = getBaseStat(StatModType::SpellDefense);
	int64_t multi = 10000ll;

	result = getStat(result, StatModType::SpellDefense, combatOptions, false, consumeBuffs);
	multi = getStat(multi, StatModType::SpellDefenseMulti, combatOptions, true, consumeBuffs);

	result = result * multi / 10000ll;

	return std::max(0ll, result);
}

int64_t Actor::GetSpellDefensePen(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::SpellDefensePen, combatOptions, false, consumeBuffs);

	return std::clamp(result, 0ll, 10000ll);
}

int64_t Actor::GetEvasion(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = getBaseStat(StatModType::Evasion);

	result = getStat(result, StatModType::Evasion, combatOptions, false, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetBlockChance(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::BlockChance, combatOptions, false, consumeBuffs);

	return std::clamp(result, 0ll, 10000ll);
}

int64_t Actor::GetBlockPower(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 5000ll;

	result = getStat(result, StatModType::BlockPower, combatOptions, false, consumeBuffs);

	return std::clamp(result, 0ll, 10000ll);
}

int64_t Actor::GetDamageTakenFlat(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::DamageTakenFlat, combatOptions, false, consumeBuffs);

	return result;
}

int64_t Actor::GetDamageTakenMulti(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::DamageTakenMulti, combatOptions, true, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetResistance(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::Resistance, combatOptions, false, consumeBuffs);

	if (IsPlayer()) {
		return std::min(result, 5000ll);
	}
	else {
		return std::min(result, 10000ll);
	}
}

int64_t Actor::GetHealingTakenFlat(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::HealingTakenFlat, combatOptions, false, consumeBuffs);

	return result;
}

int64_t Actor::GetHealingTakenMulti(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::HealingTakenMulti, combatOptions, true, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetCooldownReduction(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::CooldownReduction, combatOptions, false, consumeBuffs);

	return result;
}

int64_t Actor::GetCooldownRecoveryRate(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::CooldownRecoveryRate, combatOptions, true, consumeBuffs);

	return std::max(1ll, result);
}

int64_t Actor::GetMPCostMulti(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::MPCostMulti, combatOptions, false, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetSPCostMulti(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::SPCostMulti, combatOptions, false, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetAuraDurationFlat(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0ll;

	result = getStat(result, StatModType::AuraDurationFlat, combatOptions, false, consumeBuffs);

	return result;
}

int64_t Actor::GetAuraDurationMulti(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::AuraDurationMulti, combatOptions, true, consumeBuffs);

	return std::max(0ll, result);
}

int64_t Actor::GetGoldFind() {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::GoldFind, true, false);

	return std::max(0ll, result);
}

int64_t Actor::GetEXPRate() {
	int64_t result = 10000ll;

	result = getStat(result, StatModType::EXPRate, true, false);

	return std::max(0ll, result);
}

int64_t Actor::getStat(int64_t base, StatModType statModType, bool isMultiplicative, bool consumeBuffs) {
	CombatOptions co{};
	return getStat(base, statModType, co, isMultiplicative, consumeBuffs);
}

int64_t Actor::getStat(int64_t base, StatModType statModType, CombatOptions& combatOptions, bool isMultiplicative, bool consumeBuffs) {
	int64_t result = base;
	std::vector<StatMod> statMods{};

	// categories must be sorted for std::includes to work
	std::sort(combatOptions.Categories.begin(), combatOptions.Categories.end());

	// element vector compaison lambda, just checking if there are any matches
	auto elementCheck = [](std::vector<Element>& v1, std::vector<Element>& v2) {
		for (auto e : v2) {
			if (std::find(v1.begin(), v1.end(), e) != v1.end()) {
				return true;
			}
		}
		return false;
	};

	// stat mod check lambda
	auto statModCheck = [&](StatMod& s) {
		if (s.StatModType == statModType) {
			bool match = false;
			if (combatOptions.AbilityID != 0 && combatOptions.AbilityID == s.AbilityID) {
				match = true;
			}
			else if (combatOptions.AuraID != 0 && combatOptions.AuraID == s.AuraID) {
				match = true;
			}
			else if (s.Elements.empty() || elementCheck(combatOptions.Elements, s.Elements)) {
				// must be sorted for std::includes to work	
				std::sort(s.Categories.begin(), s.Categories.end());
				if (s.Categories.empty() || std::includes(combatOptions.Categories.begin(), combatOptions.Categories.end(), s.Categories.begin(), s.Categories.end())) {
					match = true;
				}
			}
			if (match) {
				int64_t value = s.Value;

				//if (aura != nullptr) {
				//	if (aura->MultiplyStatModsByStackSize()) {
				//		value *= aura->GetCurrentStackSize();
				//	}
				//}

				if (isMultiplicative) {
					result = result * (10000ll + value) / 10000ll;
				}
				else {
					// resistances only partially apply to multi-element attacks
					if (s.StatModType == StatModType::Resistance) {
						result += value / static_cast<int64_t>(combatOptions.Elements.size());
					}
					else {
						result += value;
					}
				}
			}
		}
	};

	// Base Stat Mods
	statMods = getBaseStatMods();
	for (auto& s : statMods) {
		statModCheck(s);
	}

	// Equipment Stat Mods
	int64_t equipValue = getStatFromEquipment(statModType, combatOptions);
	if (isMultiplicative) {
		result = result * (10000ll + equipValue) / 10000ll;
	}
	else {
		result += equipValue;
	}

	// Ability Stat Mods
	for (size_t i = 0; i < abilities.size(); i++) {
		statMods = abilities[i].GetStatMods();
		for (auto& s : statMods) {
			statModCheck(s);
		}
	}

	// Auras
	for (size_t i = 0; i < auras.size(); i++) {
		statMods = auras[i].GetStatMods();
		for (auto& s : statMods) {
			statModCheck(s);
		}
	}

	return result;
}

bool Actor::isValidTactic(Tactic& tactic, std::vector<ActorPtr>& actors) {
	// Check if ability is usable.
	Ability* ability = nullptr;
	for (size_t i = 0; i < abilities.size(); i++) {
		if (abilities[i].GetAbilityID() == tactic.AbilityID) {
			if (abilities[i].IsUsable(this)) {
				ability = &abilities[i];
				break;
			}
		}
	}

	if (ability == nullptr) {
		return false;
	}

	// Get actors in range.
	std::vector<ActorPtr> actorsInRange = ability->GetActorsInRange(this, actors);

	if (actorsInRange.empty()) {
		return false;
	}

	// Check if the tactic's condition is satisfied.
	switch (tactic.Condition) {
	case AICondition::Undefined:
		return false;
	case AICondition::None:
		break;
	// todo: aditional conditions
	case AICondition::HPPercentLessThan:
		if (!(GetAttributePercent(Attribute::HP) <= tactic.ConditionValue)) {
			return false;
		}
		break;
	case AICondition::HPPercentGreaterThan:
		if (!(GetAttributePercent(Attribute::HP) >= tactic.ConditionValue)) {
			return false;
		}
		break;
	default:
		return false;
	}

	if (filterTargets(tactic, actorsInRange).size() == 0) {
		return false;
	}

	return true;
}

std::vector<ActorPtr> Actor::filterTargets(Tactic& tactic, std::vector<ActorPtr>& targets) {
	std::vector<ActorPtr> results;

	switch (tactic.Target) {
	case AITarget::Undefined:
		break;
	case AITarget::Any:
		results = targets;
		break;
	// todo: additional target conditions
	case AITarget::HPPercentLessThan:
		for (size_t i = 0; i < targets.size(); i++) {
			if (targets[i]->GetAttributePercent(Attribute::HP) <= tactic.TargetValue) {
				results.push_back(targets[i]);
			}
		}
		break;
	case AITarget::HPPercentGreaterThan:
		for (size_t i = 0; i < targets.size(); i++) {
			if (targets[i]->GetAttributePercent(Attribute::HP) >= tactic.TargetValue) {
				results.push_back(targets[i]);
			}
		}
		break;
	default:
		break;
	}

	return results;
}