# Wallet Management System

## Overview

The Wallet Management System is designed to manage cryptocurrency portfolios, transactions, and trade orders. This system supports wallet creation, transaction processing, order management, and reporting. It also interacts with binary files for persistent storage.

## Functional Requirements

### 1. Wallet Management

- **Command**: `add-wallet fiatMoney name`
  - **Description**: Creates a new wallet with a specified initial balance (`fiatMoney`) and owner's name (`name`).
  - **Parameters**:
    - `fiatMoney`: Initial balance in real money (levs).
    - `name`: Owner's name (up to 255 characters).
  - **Behavior**:
    - Generate a unique wallet ID (a positive integer less than \(2^{32}\)).
    - Store wallet details in `wallets.dat` (binary file).
    - On creation, the wallet is credited with FMICoins equal to `fiatMoney / 375`.
    - Record a transaction from the system wallet (ID 4294967295) to the new wallet with the calculated FMICoins.

### 2. Transaction Management

- **Command**: `transfer senderId receiverId fmiCoins`
  - **Description**: Transfers FMICoins from one wallet to another.
  - **Parameters**:
    - `senderId`: ID of the sending wallet.
    - `receiverId`: ID of the receiving wallet.
    - `fmiCoins`: Amount of FMICoins to transfer.
  - **Behavior**:
    - Ensure both wallets exist and the sender has enough FMICoins.
    - Record the transaction in `transactions.dat` (binary file).

### 3. Order Management

- **Command**: `make-order type fmiCoins walletId`
  - **Description**: Creates a buy or sell order for FMICoins.
  - **Parameters**:
    - `type`: Type of the order (`SELL` or `BUY`).
    - `fmiCoins`: Amount of FMICoins.
    - `walletId`: ID of the wallet making the order.
  - **Behavior**:
    - Check if the wallet exists and has enough FMICoins or real money.
    - For a sell order, match with existing buy orders and adjust balances accordingly.
    - For a buy order, match with existing sell orders and adjust balances accordingly.
    - Record all orders in `orders.dat` (binary file).
    - Generate a text file reporting executed orders.

### 4. Wallet Information

- **Command**: `wallet-info walletId`
  - **Description**: Provides detailed information about a wallet.
  - **Parameters**:
    - `walletId`: ID of the wallet.
  - **Behavior**:
    - Display wallet owner’s name, real money balance, and FMICoins balance (computed from transactions).

### 5. Investor Reporting

- **Command**: `attract-investors`
  - **Description**: Lists the top 10 wallets with the most FMICoins.
  - **Behavior**:
    - Show the amount of FMICoins, number of orders, and the time of the first and last orders for each top wallet.

### 6. Application Termination

- **Command**: `quit`
  - **Description**: Exits the application and ensures all data is saved to the respective files.
  - **Behavior**:
    - Save current data in `wallets.dat`, `transactions.dat`, and `orders.dat`.

## Data Structures

- **Wallet**:
  ```cpp
  struct Wallet {
      char owner[256];
      unsigned id;
      double fiatMoney;
  };
  struct Transaction {
    long long time;
    unsigned senderId;
    unsigned receiverId;
    double fmiCoins;
  };
  struct Order {
    enum Type { SELL, BUY } type;
    unsigned walletId;
    double fmiCoins;
  };
