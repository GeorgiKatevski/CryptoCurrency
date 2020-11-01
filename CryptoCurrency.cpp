// CryptoCurrency.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Order.h"
#include "CryptoCurrency.h"

void deserialize()
{

	Cryptocurrency* test = Cryptocurrency::Currency();
	test->deserializeWallets(wallet_path);
	test->deserializeTransactions(transaction_path);
	test->deserializeBuyOrders(buy_order_path);
	test->deserializeSellOrders(sell_order_path);

}
void serialize()
{
	Cryptocurrency* test = Cryptocurrency::Currency();
	test->serializeWallets(wallet_path);
	test->serializeTransacitons(transaction_path);
	test->serializeBuyOrders(buy_order_path);
	test->serializeSellOrders(sell_order_path);
}
void start()
{
	double curr = 0;
	unsigned int id = 0;
	Cryptocurrency* test = Cryptocurrency::Currency();
	deserialize();
	char input[DEFAULT_INPUT_LEN] = {};
	std::cout << "Welcome to the FMI-Coin Market" << std::endl;
	std::cout << "Enter your command: ";
	std::cin.getline(input, 256);
	while (strcmp(input, "quit") != 0)
	{
		char input2[DEFAULT_INPUT_LEN];
		if (strcmp(input, "add-wallet") == 0)
		{

			std::cout << "Money :"; std::cin >> curr;
			std::cin.ignore();
			std::cout << "Name: "; std::cin.getline(input2, DEFAULT_INPUT_LEN);
			test->addWallet(curr, input2);
		}
		else if (strcmp(input, "make-order") == 0)
		{
			std::cout << "Order type: ";	std::cin.getline(input, 256);
			std::cout << "Currency: "; std::cin >> curr;
			std::cout << "ID: "; std::cin >> id;


			if (strcmp(input, "BUY") == 0)
			{
				if (!test->makeOrder(Order::BUY, id, curr))
					std::cout << "There is no such wallet" << std::endl;
			}
			else if (strcmp(input, "SELL") == 0)
			{
				if (!test->makeOrder(Order::SELL, id, curr))
					std::cout << "There is no such wallet" << std::endl;
			}
		}
		else if (strcmp(input, "wallet-info") == 0)
		{
			std::cout << "Wallet ID: ";
			std::cin >> id;
			test->walletInfo(id);
		}
		
		else if (strcmp(input, "transfer") == 0)
		{
			unsigned fmiCoins;
			unsigned id2;
			std::cout << "Sending from: ";
			std::cin >> id;
			std::cout << "Id to be received: ";
			std::cin >> id2;
			std::cout << "Coins: ";
			std::cin >> fmiCoins;
			test->transfer(id, id2, fmiCoins);
		}

		std::cin.getline(input, 256);
	}
	serialize();
}






int main()
{
	start();

	Cryptocurrency* test = Cryptocurrency::Currency();
	test->printWallets();
	test->printTransactions();


	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
