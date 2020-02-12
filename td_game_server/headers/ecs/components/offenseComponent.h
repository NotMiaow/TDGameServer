#ifndef OFFENSE_COMPONENT_H__
#define OFFENSE_COMPONENT_H__

struct OffenseComponent
{
	OffenseComponent() { }
	OffenseComponent(const float& baseAttackRate, const int& baseDamage) : baseAttackRate(baseAttackRate), curAttackRate(baseAttackRate),
		baseDamage(baseDamage), curDamage(baseDamage) { }
	//DamageType damageType;
	float baseAttackRate;
	float curAttackRate;
	int baseDamage;
	int curDamage;
};

#endif