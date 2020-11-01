#include "CryptoCurrency.h"

Cryptocurrency Cryptocurrency::currency;
Cryptocurrency::Cryptocurrency() :
	ID(1),
	wallets(),
	transactions(),
	sell_orders(),
	buy_orders()
{}

Cryptocurrency* Cryptocurrency::Currency()
{
	return &currency;
}

void Cryptocurrency::printWallets() const
{
	unsigned int numberOfWallets = currency.wallets.getSize();

	for (unsigned int i = 0; i < numberOfWallets; ++i)
	{
		std::cout << "ID: " << currency.wallets[i].id << std::endl;
		std::cout << "Money: " << currency.wallets[i].fiatMoney << std::endl;
		std::cout << "Name: " << currency.wallets[i].owner << std::endl;
	}
}

void Cryptocurrency::printTransactions() const
{
	unsigned int numberOfTransactions = currency.transactions.getSize();

	for (unsigned int i = 0; i < numberOfTransactions; ++i)
	{

		std::cout << "SenderID: " << currency.transactions[i].senderId << std::endl;
		std::cout << "ReceiverID: " << currency.transactions[i].receiverId << std::endl;
		std::cout << "FmiCoins: " << currency.transactions[i].fmiCoins << std::endl;
		std::cout << "Time: " << currency.transactions[i].time << std::endl;
	}
}

unsigned int Len(unsigned number)
{
	unsigned int len = 1;
	while (number > 10)
	{
		number /= 10;
		++len;
	}

	return len;
}
//had it in another way at the end of the cpp-file
char* Cryptocurrency::OrderName(unsigned  id)
{
	char* name = new char[(sizeof(unsigned int) * 8 + sizeof(long long) * 8 + 6)];

	unsigned int count = 0;

	unsigned int len = Len(id);
	unsigned  pow = 1;

	for (unsigned int i = 0; i < len - 1; ++i)
		pow *= 10;

	for (unsigned int i = 0; i < len; ++i)
	{
		name[count++] = static_cast<char>((id / pow) % 10) + '0';
		pow /= 10;
	}

	name[count++] = '_';

	unsigned long long currentTime = time(0);
	len = Len(currentTime);
	pow = 1;

	for (unsigned int i = 0; i < len - 1; ++i)
		pow *= 10;

	for (unsigned int i = 0; i < len; ++i)
	{
		name[count++] = static_cast<char>((currentTime / pow) % 10) + '0';
		pow /= 10;
	}
	name += count;
	strcpy(name, ".txt");
	name[4] = 0;
	name -= count;

	return name;
}


unsigned int Cryptocurrency::makeOrderSELL(const unsigned int wallet_id, double coins)
{
	unsigned int orders_satisfied = 0;

	if (calculateCoins(currency.wallets[wallet_id - 1].id) - coins >= EPS)
	{
		unsigned int buy_orders_len = currency.buy_orders.getSize();
		double recieving_amount = 0;

		Order* order = NULL;

		while (buy_orders_len && coins > EPS)
		{
			if (coins - buy_orders[0].fmiCoins > EPS)
			{
				coins -= recieving_amount = buy_orders[0].fmiCoins;
			}
			else
			{
				if (buy_orders[0].fmiCoins - coins > EPS)
				{
					recieving_amount = coins;
					coins = 0;
					order = new Order(Order::BUY,
						buy_orders[0].walletId,
						buy_orders[0].fmiCoins - coins);
				}
			}

			currency.wallets[wallet_id - 1].setMoney(currency.wallets[wallet_id - 1].fiatMoney + (recieving_amount * COIN_EXCH_RATE));

			makeTransaction(wallet_id, buy_orders[0].walletId, recieving_amount);
			buy_orders.remove(0);
			--buy_orders_len;
			++orders_satisfied;
		}
		if (!order)
		{
			if (coins > EPS)
			{
				order = new Order(Order::SELL,
					wallet_id,
					coins);

				sell_orders.pushBack(*order);
			}
		}

		else
			buy_orders.pushBack(*order);

		delete order;
	}
	else
	{
		std::cerr << "Not enough coins " << std::endl;

	}
	return orders_satisfied;
}

void Cryptocurrency::push_wallet(const double balance, const char owner[256])
{
	Wallet wallet(owner, balance, ID++);
	currency.wallets.pushBack(wallet);
}
void Cryptocurrency::addWallet(const double balance, const char owner[256])
{
	push_wallet(balance, owner);
	makeTransaction(SYSTEM_WALLET_ID, ID - 1, balance / COIN_EXCH_RATE);
}

void Cryptocurrency::makeTransaction(const unsigned int senderID,
	const unsigned int receiverID,
	const double fmiCoins)
{
	Transaction temp(senderID, receiverID, fmiCoins, time(0));
	currency.transactions.pushBack(temp);
}


unsigned int Cryptocurrency::makeOrderBUY(const unsigned int wallet_id,
	double coins)
{
	unsigned int orders_satisfied = 0;

	if (currency.wallets[wallet_id - 1].fiatMoney / COIN_EXCH_RATE >= coins)
	{
		currency.wallets[wallet_id - 1].setMoney(currency.wallets[wallet_id - 1].fiatMoney - coins * COIN_EXCH_RATE);

		unsigned int sell_order_len = sell_orders.getSize();

		Order* order = NULL;
		double recieving_amount = 0;
		while (sell_order_len && coins > EPS)
		{
			if (coins - sell_orders[0].fmiCoins > EPS)
			{
				coins -= recieving_amount = sell_orders[0].fmiCoins;
			}
			else
			{
				recieving_amount = coins;
				if (sell_orders[0].fmiCoins - coins > EPS)
				{
					coins = 0;
					order = new Order(Order::SELL,
						sell_orders[0].walletId,
						sell_orders[0].fmiCoins - recieving_amount);
				}
			}

			wallets[sell_orders[0].walletId - 1].setMoney(wallets[sell_orders[0].walletId - 1].fiatMoney + recieving_amount * COIN_EXCH_RATE);

			makeTransaction(sell_orders[0].walletId,
				wallet_id,
				recieving_amount);


			sell_orders.remove(0);
			--sell_order_len;
		}


		if (!order)
		{
			if (coins > EPS)
			{
				order = new Order(Order::BUY, wallet_id, coins);

				buy_orders.pushBack(*order);
			}
		}

		else
			sell_orders.pushBack(*order);

		delete order;
	}
	else
	{
		std::cout << "Not enough money" << std::endl;

	}
	return orders_satisfied;
}
bool  Cryptocurrency::makeOrder(const Order::Type  type, const unsigned int wallet_id, const double fmiCoins)
{
	if (wallet_id <= ID)
	{
		unsigned int transactions = 0;
		if (type == Order::BUY)
			transactions = makeOrderBUY(wallet_id, fmiCoins);
		else if (type == Order::SELL)
			transactions = makeOrderSELL(wallet_id, fmiCoins);

		serializeOrderTransactions(type, transactions, wallet_id);
		return true;
	}

	return false;
}

void Cryptocurrency::deserializeWallets(const char* file_name)
{
	std::ifstream file(file_name, std::ios::binary);

	if (!file)
	{
		std::cerr << "Coudn't open the file" << std::endl;
	}
	char owner[256] = {};
	double balance = 0;

	while (file.good())
	{
		file.read(owner, DEFAULT_SIZE);
		file.read(reinterpret_cast<char*>(&balance), sizeof(balance));

		push_wallet(balance, owner);
	}
	file.close();

}

void Cryptocurrency::deserializeTransactions(const char* file_name)
{
	std::ifstream file(file_name, std::ios::binary);

	if (!file)
	{
		std::cerr << "Coudn't open the file" << std::endl;
	}
	unsigned int senderID = 0, receiverID = 0;
	double fmiCoins = 0;
	long long tr_time = 0;


	while (file.good())
	{
		file.read(reinterpret_cast<char*>(&(senderID)), sizeof(senderID));
		file.read(reinterpret_cast<char*>(&receiverID), sizeof(receiverID));
		file.read(reinterpret_cast<char*>(&fmiCoins), sizeof(fmiCoins));
		file.read(reinterpret_cast<char*>(&tr_time), sizeof(tr_time));
		//learned from the seminar...
		currency.transactions.pushBack(Transaction(senderID, receiverID, fmiCoins, tr_time));
	}
	file.close();

}

void Cryptocurrency::deserializeBuyOrders(const char* file_name)
{
	std::ifstream file(file_name);

	unsigned int ID = 0;
	double coins = 0;

	if (!file)
	{
		std::cerr << "Coudn't open the file" << std::endl;
	}

	while (file.good())
	{
		file.read(reinterpret_cast<char*>(&(ID)), sizeof(ID));
		file.read(reinterpret_cast<char*>(&(coins)), sizeof(coins));

		buy_orders.pushBack(Order(Order::BUY, ID, coins));
	}
	file.close();

}

void Cryptocurrency::deserializeSellOrders(const char* file_name)
{
	std::ifstream file(file_name, std::ios::binary);

	unsigned int wallet_id = 0;
	double coins = 0;

	if (!file)
	{
		std::cerr << "Coudn't open the file" << std::endl;
	}

	while (file.good())
	{
		file.read(reinterpret_cast<char*>(&(wallet_id)), sizeof(wallet_id));
		file.read(reinterpret_cast<char*>(&(coins)), sizeof(coins));
		sell_orders.pushBack(Order(Order::SELL, wallet_id, coins));
	}
	file.close();

}

double Cryptocurrency::calculateCoins(const unsigned int client_id)
{

	unsigned int transactions_len = currency.transactions.getSize();
	double balance = 0;

	for (unsigned int i = 0; i < transactions_len; ++i)
	{
		if (currency.transactions[i].receiverId == client_id)
		{
			balance += currency.transactions[i].fmiCoins;
		}
		else if (currency.transactions[i].senderId == client_id)
		{
			balance -= currency.transactions[i].fmiCoins;
		}
	}

	return balance;
}

void Cryptocurrency::walletInfo(const unsigned int client_id)
{
	if (client_id > ID)
	{
		std::cout << "Wallet doesn't exist" << std::endl;
	}
	else
	{
		std::cout << "Owner: " << currency.wallets[client_id - 1].owner << std::endl;
		std::cout << "Money: " << currency.wallets[client_id - 1].fiatMoney << std::endl;
		std::cout << "Coins: " << calculateCoins(client_id) << std::endl;

	}
}
void Cryptocurrency::serializeWallets(const char* file_name) const
{
	std::ofstream file(file_name, std::ios::binary);

	if (!file)
	{
		std::cerr << "Cannot open the file" << std::endl;
	}
	int size = currency.wallets.getSize();

	for (int w = 0; w < size; ++w)
	{
		if (file.good())
		{
			file.write(currency.wallets[w].owner, DEFAULT_SIZE);
			file.write(reinterpret_cast<const char*>(&(currency.wallets[w].fiatMoney)), sizeof(currency.wallets[w].fiatMoney));
		}
	}
	file.close();

}

void Cryptocurrency::serializeTransacitons(const char* file_name) const
{
	std::ofstream file(file_name, std::ios::binary);

	if (!file)
	{
		std::cerr << "Cannot open the file" << std::endl;
	}
	int size = currency.transactions.getSize();

	for (int w = 0; w < size; ++w)
	{
		if (file.good())
		{
			file.write(reinterpret_cast<const char*>(&(currency.transactions[w].senderId)), sizeof(currency.transactions[w].senderId));
			file.write(reinterpret_cast<const char*>(&(currency.transactions[w].receiverId)), sizeof(currency.transactions[w].receiverId));
			file.write(reinterpret_cast<const char*>(&(currency.transactions[w].fmiCoins)), sizeof(currency.transactions[w].fmiCoins));
			file.write(reinterpret_cast<const char*>(&(currency.transactions[w].time)), sizeof(currency.transactions[w].time));
		}
	}

	file.close();


}

void Cryptocurrency::serializeBuyOrders(const char* file_name)
{
	std::ofstream file(file_name, std::ios::binary);
	if (!file)
	{
		std::cerr << "Cannot open the file" << std::endl;
	}
	unsigned int orders_len = buy_orders.getSize();
	for (unsigned int i = 0; i < orders_len; ++i)
	{
		if (file.good())
		{
			file.write(reinterpret_cast<const char*>(&(buy_orders[i].walletId)), sizeof(buy_orders[i].walletId));
			file.write(reinterpret_cast<const char*>(&(buy_orders[i].fmiCoins)), sizeof(buy_orders[i].fmiCoins));
		}
	}

	file.close();

}

void Cryptocurrency::serializeSellOrders(const char* file_name)
{
	std::ofstream file(file_name, std::ios::binary);
	if (!file)
	{
		std::cerr << "Cannot open the file" << std::endl;
	}
	unsigned int orders_len = sell_orders.getSize();
	for (unsigned int i = 0; i < orders_len; ++i)
	{
		if (file.good())
		{
			file.write(reinterpret_cast<const char*>(&(sell_orders[i].walletId)), sizeof(sell_orders[i].walletId));
			file.write(reinterpret_cast<const char*>(&(sell_orders[i].fmiCoins)), sizeof(sell_orders[i].fmiCoins));
		}
	}
	file.close();

}

void Cryptocurrency::serializeOrderTransactions(const Order::Type  type, const unsigned int transactions_made,
	const unsigned int client_id)
{
	char* order_log_name = OrderName(client_id);

	std::ofstream file(order_log_name);

	if (!file)
	{
		std::cerr << "Cannot open the file" << std::endl;
	}
	double amount_transfered = 0;
	unsigned int transactions_len = currency.transactions.getSize();
	unsigned int i = currency.transactions.getSize() - transactions_made;

	for (; i < transactions_len; ++i)
	{
		if (file)
		{
			file << wallets[transactions[i].senderId - 1].owner;
			file << ", " << wallets[transactions[i].receiverId - 1].owner;
			file << ", " << currency.transactions[i].fmiCoins << '\n';
		}
		amount_transfered += currency.transactions[i].fmiCoins;
	}

	amount_transfered *= COIN_EXCH_RATE;

	if (!file)
	{
		std::cerr << "Something happened with the file" << std::endl;
	}
	else
	{
		file << "Transactions made: " << transactions_made << ", Amount transfered: " << amount_transfered << endl;
	}
	file.close();


	delete[] order_log_name;
}
const int SIZE = 10;

void Cryptocurrency::transfer(unsigned senderId, unsigned receiverId, double fmiCoins)
{
	if (senderId <= ID && receiverId <= ID)
	{
		if (currency.wallets[senderId - 1].fiatMoney / COIN_EXCH_RATE >= fmiCoins)
		{
			currency.wallets[senderId - 1].setMoney(currency.wallets[senderId - 1].fiatMoney - fmiCoins * COIN_EXCH_RATE);
			currency.wallets[receiverId - 1].setMoney(currency.wallets[receiverId - 1].fiatMoney + fmiCoins * COIN_EXCH_RATE);
			makeTransaction(senderId, receiverId, fmiCoins);
		}
	}
	else
		std::cerr << "Invalid Id" << std::endl;
}

//-----------------------------------------------------------------------------------------------------------

//function attract-investors -- different because in the beggining the homework was based on only files 
//and i started it again from the beginning and couldnt find a way and time to fix that function



time_t Cryptocurrency::timeTransaction(unsigned mode, unsigned id) {
	Transaction transaction;
	std::time_t transaction_time = 0;
	if (mode == 1)
	{
		std::ifstream file(transaction_path, std::ios::binary);
		if (!file)
		{
			std::cerr << ": Error while opening." << std::endl;
			return -1;
		}
		bool found = false;
		while (!found) {
			file.read((char*)&transaction, sizeof(transaction));
			if (file)
			{
				if (transaction.senderId == id || transaction.receiverId == id) {
					found = true;
					transaction_time = transaction.time;
				}
			}
		}
		file.close();
	}
	else {
		std::ifstream file(transaction_path, std::ios::binary);
		if (!file) {
			std::cerr << " Error while opening." << std::endl;
			return -1;
		}
		bool found_last = false;
		while (!found_last)
		{
			file.read((char*)&transaction, sizeof(transaction));
			if (!file)
			{
				found_last = true;
			}
			if (transaction.senderId == id || transaction.receiverId == id) {
				transaction_time = transaction.time;
			}
		}
		file.close();
	}
	return transaction_time;
}

void Cryptocurrency::change(Wallet& old_wallet, Wallet& new_wallet)
{
	old_wallet.fiatMoney = new_wallet.fiatMoney;
	old_wallet.id = new_wallet.id;
	strcpy_s(old_wallet.owner, new_wallet.owner);


}
void printWallet(Wallet& wallet) {
	std::cout << "Wallet money: " << wallet.fiatMoney << std::endl;
	std::cout << "Wallet id: " << wallet.id << std::endl;
	std::cout << "Wallet owner: " << wallet.owner << std::endl;
}
unsigned Cryptocurrency::countTransactions(unsigned id) {
	Transaction transaction;
	unsigned counter_transactions = 0;
	std::ifstream file(transaction_path, std::ios::binary);
	if (!file) {
		std::cerr << transaction_path << ": error while opening." << std::endl;
		return -1;
	}
	while (!file.eof()) {
		file.read((char*)&transaction, sizeof(transaction));
		if (file) {
			if (transaction.receiverId == id || transaction.senderId == id) {
				counter_transactions++;
			}
		}
	}
	file.close();
	return counter_transactions;
}
