// ================================================================
//
// combat.cpp
//
// ================================================================

#include "combat.hpp"

#include <sstream>
#include "random.hpp"
#include "../data/abilityData.hpp"
#include "../entity/actor.hpp"
#include "../scene/battleScene.hpp"

static int64_t getBaseDamage(Actor* user) {
	double level = static_cast<double>(user->GetLevel()) - 1.0;

	double base = 10.0 + (level) + (std::pow(level, 2.0) * 0.1);
	base = std::round(base);

	return static_cast<int64_t>(base);
}

static bool hitRoll(Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t accuracy = user->GetAccuracy(combatOptions, true);
	int64_t evasion = target->GetEvasion(combatOptions, true);

	if (evasion > 0) {
		int64_t hitChance = combatOptions.BaseHitChance * accuracy / evasion;
		if (Random::RandomInt(1, 10000) <= hitChance) {
			return true;
		}
		else {
			return false;
		}
	}

	return true;
}

static bool blockRoll(Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t blockChance = user->GetBlockChance(combatOptions, true);

	if (Random::RandomInt(1, 10000) <= blockChance) {
		return true;
	}
	return false;
}

static bool critRoll(Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t critChance = user->GetCritChance(combatOptions, true);
	int64_t critProtection = target->GetCritProtection(combatOptions, true);
	critChance = std::max(0ll, critChance - critProtection);

	if (Random::RandomInt(1, 10000) <= critChance) {
		return true;
	}
	return false;
}

static int64_t applyCrit(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t critPower = user->GetCritPower(combatOptions, true);
	int64_t critPowerProtection = target->GetCritPowerProtection(combatOptions, true);

	critPower = (critPower - 10000) * critPowerProtection / 10000;
	critPower += 10000;

	result = result * critPower / 10000;

	return result;
}

static int64_t applyBlock(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t blockPower = target->GetBlockPower(combatOptions, true);

	result = result * (10000 - blockPower) / 10000;

	return result;
}

static int64_t applyDamageFlat(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t damageFlat = user->GetDamageFlat(combatOptions, true);

	result = result + (damageFlat * 100);

	return result;
}

static int64_t applyDamageMulti(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t damageMulti = user->GetDamageMulti(combatOptions, true);

	result = result * damageMulti / 10000;

	return result;
}

static int64_t applyDamageTakenFlat(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t damageTakenFlat = target->GetDamageTakenFlat(combatOptions, true);

	result = result - (damageTakenFlat * 100);
	result = std::max(0ll, result);

	return result;
}

static int64_t applyDamageTakenMulti(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t damageTakenMulti = target->GetDamageTakenMulti(combatOptions, true);

	result = result * damageTakenMulti / 10000;

	return result;
}

static int64_t applyArmor(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions, bool useSpellDefense) {
	int64_t result = amount;
	int64_t armor = 0;
	int64_t armorPen = 0;
	int64_t armorMitigation = 0;

	double attackerLevel = static_cast<double>(user->GetLevel()) - 1.0;
	int64_t armorConstant = static_cast<int64_t>(std::round(30.0 + (attackerLevel * 3.0) + (std::pow(attackerLevel, 2.0) * 0.3)));

	if (useSpellDefense) {
		armor = target->GetSpellDefense(combatOptions, true);
		armorPen = user->GetSpellDefensePen(combatOptions, true);
	}
	else {
		armor = target->GetArmor(combatOptions, true);
		armorPen = user->GetArmorPen(combatOptions, true);
	}

	armor = armor * (10000 - armorPen) / 10000;
	armorMitigation = (armor * 10000) / (armor + armorConstant);
	result = result * (10000 - armorMitigation) / 10000;

	return result;
}

static int64_t applyResistance(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t resistance = target->GetResistance(combatOptions, true);

	result = result * (10000 - resistance) / 10000;

	return result;
}

static int64_t applyHealingFlat(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t healingFlat = user->GetHealingFlat(combatOptions, true);

	result = result + (healingFlat * 100);

	return result;
}

static int64_t applyHealingMulti(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t healingMulti = user->GetHealingMulti(combatOptions, true);

	result = result * healingMulti / 10000;

	return result;
}

static int64_t applyHealingTakenFlat(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t healingTakenFlat = target->GetHealingTakenFlat(combatOptions, true);

	result = result - (healingTakenFlat * 100);
	result = std::max(0ll, result);

	return result;
}

static int64_t applyHealingTakenMulti(int64_t amount, Actor* user, Actor* target, CombatOptions& combatOptions) {
	int64_t result = amount;
	int64_t healingTakenMulti = target->GetHealingTakenMulti(combatOptions, true);

	result = result * healingTakenMulti / 10000;

	return result;
}

static int64_t applyVariance(int64_t amount) {
	int64_t result = amount;
	result = result * Random::RandomInt(9500, 10500) / 10000;

	// +/- 1 damage based on how close the damage is to the next whole number
	// divide by 100 to get the true damage
	int64_t roundUpChance = result % 100;
	if (Random::RandomInt(1, 100) <= roundUpChance) {
		result = result / 100 + 1;
	}
	else {
		result = result / 100;
	}
	return result;
}

static void outputDamageMessage(Actor* user, Actor* target, CombatOptions& combatOptions, CombatResult& combatResult, BattleScene* battleScene) {
	if (battleScene != nullptr) {
		std::stringstream message;

		if (user->IsPlayer()) {
			message << "#player ";
		}
		else {
			message << "#monster ";
		}

		message << user->GetName() << "'s #default " << combatOptions.EventName;

		if (combatResult.DidHit) {
			message << " dealt #damage ";
			if (combatResult.DidCrit) {
				message << "\\*" << combatResult.Amount << "\\*";
			}
			else {
				message << combatResult.Amount;
			}

			message << " #default damage to ";
		}
		else {
			message << " #aaaaaa missed #default ";
		}

		if (target->IsPlayer()) {
			message << "#player ";
		}
		else {
			message << "#monster ";
		}

		message << target->GetName() << "#default .";

		if (combatResult.DidBlock) {
			message << " (Blocked)";
		}

		battleScene->AddMessage(message.str());
	}
}

static void outputHealingMessage(Actor* user, Actor* target, CombatOptions& combatOptions, CombatResult& combatResult, BattleScene* battleScene) {
	if (battleScene != nullptr) {
		std::stringstream message;

		if (user->IsPlayer()) {
			message << "#player ";
		}
		else {
			message << "#monster ";
		}

		message << user->GetName() << "'s #default " << combatOptions.EventName << " healed ";

		if (target->IsPlayer()) {
			message << "#player ";
		}
		else {
			message << "#monster ";
		}

		message << target->GetName() << " #default for #heal ";

		if (combatResult.DidCrit) {
			message << "\\*" << combatResult.Amount << "\\*";
		}
		else {
			message << combatResult.Amount;
		}

		message << "#default .";

		battleScene->AddMessage(message.str());
	}
}

CombatResult Combat::WeaponDamage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene) {
	CombatResult results;
	int64_t amount = 0;

	if (!combatOptions.CanDodge || hitRoll(user, target, combatOptions)) {
		results.DidHit = true;

		if (combatOptions.CanCrit) {
			results.DidCrit = critRoll(user, target, combatOptions);
		}

		if (combatOptions.CanBlock) {
			results.DidBlock = blockRoll(user, target, combatOptions);
		}

		amount = user->GetMainHandDamage(combatOptions, true) * coefficient / 100; // Intentionally dividing by 100, last two digits used as two-decimal precision

		if (results.DidCrit) {
			amount = applyCrit(amount, user, target, combatOptions);
		}

		if (results.DidBlock) {
			amount = applyBlock(amount, user, target, combatOptions);
		}

		amount = applyDamageFlat(amount, user, target, combatOptions);
		amount = applyDamageMulti(amount, user, target, combatOptions);

		amount = applyDamageTakenFlat(amount, user, target, combatOptions);
		amount = applyDamageTakenMulti(amount, user, target, combatOptions);

		amount = applyArmor(amount, user, target, combatOptions, false);
		amount = applyResistance(amount, user, target, combatOptions);

		amount = applyVariance(amount);

		target->TakeDamage(attribute, amount);
		results.Amount = amount;

	}

	outputDamageMessage(user, target, combatOptions, results, battleScene);

	return results;
}

CombatResult Combat::AttackDamage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene) {
	CombatResult results;
	int64_t amount = 0;

	if (!combatOptions.CanDodge || hitRoll(user, target, combatOptions)) {
		results.DidHit = true;

		if (combatOptions.CanCrit) {
			results.DidCrit = critRoll(user, target, combatOptions);
		}

		if (combatOptions.CanBlock) {
			results.DidBlock = blockRoll(user, target, combatOptions);
		}

		amount = (getBaseDamage(user) + user->GetAttackPower(combatOptions, true)) * coefficient / 100; // Intentionally dividing by 100, last two digits used as two-decimal precision

		if (results.DidCrit) {
			amount = applyCrit(amount, user, target, combatOptions);
		}

		if (results.DidBlock) {
			amount = applyBlock(amount, user, target, combatOptions);
		}

		amount = applyDamageFlat(amount, user, target, combatOptions);
		amount = applyDamageMulti(amount, user, target, combatOptions);

		amount = applyDamageTakenFlat(amount, user, target, combatOptions);
		amount = applyDamageTakenMulti(amount, user, target, combatOptions);

		amount = applyArmor(amount, user, target, combatOptions, false);
		amount = applyResistance(amount, user, target, combatOptions);

		amount = applyVariance(amount);

		target->TakeDamage(attribute, amount);
		results.Amount = amount;

	}

	outputDamageMessage(user, target, combatOptions, results, battleScene);

	return results;
}

CombatResult Combat::SpellDamage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene) {
	CombatResult results;
	int64_t amount = 0;

	if (!combatOptions.CanDodge || hitRoll(user, target, combatOptions)) {
		results.DidHit = true;

		if (combatOptions.CanCrit) {
			results.DidCrit = critRoll(user, target, combatOptions);
		}

		if (combatOptions.CanBlock) {
			results.DidBlock = blockRoll(user, target, combatOptions);
		}

		amount = (getBaseDamage(user) + user->GetSpellPower(combatOptions, true)) * coefficient / 100; // Intentionally dividing by 100, last two digits used as two-decimal precision

		if (results.DidCrit) {
			amount = applyCrit(amount, user, target, combatOptions);
		}

		if (results.DidBlock) {
			amount = applyBlock(amount, user, target, combatOptions);
		}

		amount = applyDamageFlat(amount, user, target, combatOptions);
		amount = applyDamageMulti(amount, user, target, combatOptions);

		amount = applyDamageTakenFlat(amount, user, target, combatOptions);
		amount = applyDamageTakenMulti(amount, user, target, combatOptions);

		amount = applyArmor(amount, user, target, combatOptions, true);
		amount = applyResistance(amount, user, target, combatOptions);

		amount = applyVariance(amount);

		target->TakeDamage(attribute, amount);
		results.Amount = amount;

	}

	outputDamageMessage(user, target, combatOptions, results, battleScene);

	return results;
}

CombatResult Combat::Healing(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene) {
	CombatResult results;
	int64_t amount = 0;

	results.DidHit = true;

	if (combatOptions.CanCrit) {
		results.DidCrit = critRoll(user, target, combatOptions);
	}

	amount = (getBaseDamage(user) + user->GetHealingPower(combatOptions, true)) * coefficient / 100; // Intentionally dividing by 100, last two digits used as two-decimal precision

	amount = applyHealingFlat(amount, user, target, combatOptions);
	amount = applyHealingMulti(amount, user, target, combatOptions);

	amount = applyHealingTakenFlat(amount, user, target, combatOptions);
	amount = applyHealingTakenMulti(amount, user, target, combatOptions);

	amount = applyVariance(amount);

	target->Heal(attribute, amount);
	results.Amount = amount;

	outputHealingMessage(user, target, combatOptions, results, battleScene);

	return results;
}

CombatResult Combat::HealingPercent(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t percent, Attribute attribute, BattleScene* battleScene) {
	CombatResult results;
	int64_t amount = 0;

	results.DidHit = true;

	if (combatOptions.CanCrit) {
		results.DidCrit = critRoll(user, target, combatOptions);
	}

	switch (attribute) {
	case Attribute::HP:
		amount = target->GetMaxHP();
		break;
	case Attribute::MP:
		amount = target->GetMaxMP();
		break;
	case Attribute::SP:
		amount = target->GetMaxSP();
		break;
	default:
		break;
	}

	amount = amount * percent / 100; // Intentionally dividing by 100, last two digits used as two-decimal precision

	amount = applyHealingFlat(amount, user, target, combatOptions);
	amount = applyHealingMulti(amount, user, target, combatOptions);

	amount = applyHealingTakenFlat(amount, user, target, combatOptions);
	amount = applyHealingTakenMulti(amount, user, target, combatOptions);

	amount = applyVariance(amount);

	target->Heal(attribute, amount);
	results.Amount = amount;

	outputHealingMessage(user, target, combatOptions, results, battleScene);

	return results;
}

void Combat::AddAura(Actor* user, Actor* target, size_t auraID, int stacks, BattleScene* battleScene) {
	target->AddAura(auraID, user, stacks);
}