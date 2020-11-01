#pragma once
#include "Wallet.h"
struct Transaction {
	long long time;
	unsigned senderId;
	unsigned receiverId;
	double fmiCoins;
	Transaction();
	Transaction(const unsigned int, const unsigned int, const double, const long long);

};