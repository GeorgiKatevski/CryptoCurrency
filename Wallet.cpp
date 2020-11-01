#include "Wallet.h"

Wallet::Wallet() :
	owner(),
	id(0),
	fiatMoney()
{
	++id;
}

Wallet::Wallet(const char _owner[DEFAULT_NAME_LENGTH], const double _money, const unsigned int _id) :
	owner(),
	id(_id),
	fiatMoney(_money)
{
	strcpy(owner, _owner);
	id = _id;
	fiatMoney = _money;
}

void Wallet::setMoney(const double _fiatMoney)
{
	fiatMoney = _fiatMoney;
}