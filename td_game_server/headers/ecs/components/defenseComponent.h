#ifndef DEFENSE_COMPONENT_H__
#define DEFENSE_COMPONENT_H__

struct DefenseComponent
{
    DefenseComponent() { }
    DefenseComponent(const int& baseHealth, const int& baseHealthRegeneration) : baseHealth(baseHealth), maxHealth(baseHealth),
        curHealth(baseHealth), baseHealthRegeneration(baseHealthRegeneration), healthRegeneration(baseHealthRegeneration) {  }
    int baseHealth;
    int maxHealth;
    int curHealth;
    int baseHealthRegeneration;
    int healthRegeneration;
};

#endif