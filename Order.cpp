#include "Order.h"


Order::Order() :
	type{ Order::SELL },
	walletId(0),
	fmiCoins(0)
{}
Order::Order(const Type _type, const unsigned int _ID, const double _coins) :
	type(_type),
	walletId(_ID),
	fmiCoins(_coins)
{}