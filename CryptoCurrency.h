#pragma once
#include "Order.h"
#include "Vector.h"

struct Cryptocurrency
{

private:
	Cryptocurrency();
	static Cryptocurrency currency;
public:

	static Cryptocurrency* Currency();
	unsigned int ID;
	D_array<Wallet> wallets;
	D_array<Transaction> transactions;
	D_array<Order> sell_orders;
	D_array<Order> buy_orders;

	void push_wallet(const double, const char[256]);
	char* OrderName(unsigned int);

	unsigned int makeOrderBUY(const unsigned int, double);
	unsigned int makeOrderSELL(const unsigned int, double);
	double calculateCoins(const unsigned int);
	void addWallet(const double, const char[256]);
	void makeTransaction(const unsigned int, const unsigned int, const double);
	bool makeOrder(const Order::Type, const unsigned int, const double);
	void walletInfo(const unsigned int);
	void transfer(unsigned senderId, unsigned receiverId, double fmiCoins);


	void printWallets() const;
	void printTransactions() const;
	void deserializeWallets(const char*);
	void deserializeTransactions(const char*);
	void deserializeBuyOrders(const char*);
	void deserializeSellOrders(const char*);
	void serializeWallets(const char*) const;
	void serializeTransacitons(const char*) const;
	void serializeBuyOrders(const char*);
	void serializeSellOrders(const char*);
	void serializeOrderTransactions(const Order::Type, const unsigned int, const unsigned int);

	//for the attract investors but it didnd't work.....
	void attractInvestors();
	void change(Wallet& old_wallet, Wallet& new_wallet);
	time_t timeTransaction(unsigned mode, unsigned id);
	unsigned countTransactions(unsigned id);
	double walletFmiCoins(unsigned id);


};


