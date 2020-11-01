#pragma once
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <cassert>
#include <string>

using namespace std;
const char* const wallet_path = "wallets.dat";
const char* const transaction_path = "transactions.dat";
const char* const buy_order_path = "buy_orders.dat";
const char* const sell_order_path = "sell_orders.dat";
const char* const THE_RICHEST_PATH = "the richest.dat";
const double EPS = 0.0001;
const unsigned long long SYSTEM_WALLET_ID = 4294967295;
const double COIN_EXCH_RATE = 375;
const int DEFAULT_SIZE = 256;
const int DEFAULT_NAME_LENGTH = 256;
const int DEFAULT_INPUT_LEN = 1024;



struct Wallet {
	char owner[256];
	unsigned id;
	double fiatMoney;
	Wallet();
	Wallet(const char[DEFAULT_NAME_LENGTH], const double, const unsigned int);
	void setMoney(const double rhs_money);
	void printwallet(Wallet& wallet);
};
