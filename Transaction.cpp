#include "Transaction.h"


Transaction::Transaction() :
	time(0),
	senderId(0),
	receiverId(1),
	fmiCoins(0)
{}

Transaction::Transaction(const unsigned int _sender, const unsigned int _receiver, const double _fiatMoney, const long long _time) :
	time(_time),
	senderId(_sender),
	receiverId(_receiver),
	fmiCoins(_fiatMoney)
{}