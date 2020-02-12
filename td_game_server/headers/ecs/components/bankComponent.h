#ifndef BANK_COMPONENT_H__
#define BANK_COMPONENT_H__

struct BankComponent
{
	BankComponent() { }
	BankComponent(const int& gold, const int& income) : gold(gold), income(income) { }
	int income;
	int gold;
};


#endif