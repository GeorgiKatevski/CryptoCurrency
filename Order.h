#pragma once
#include "Transaction.h"
struct Order {
	enum Type { SELL, BUY } type;
	unsigned walletId;
	double fmiCoins;
	Order();
	Order(const Type, const unsigned, const double);
};
