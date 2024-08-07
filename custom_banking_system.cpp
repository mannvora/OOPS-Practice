#include <bits/stdc++.h>
using namespace std;

class BankingSystem {
    unordered_map<string, int> accounts; // Map of account ID to balance
    map<string, int> outgoing_transactions; // Map of account ID to outgoing transactions
    set<pair<int, string>> ranking; // Set to maintain sorted order of outgoing transactions
    set<pair<int, pair<string, pair<string, int>>>> scheduledPayments; // <TimeStamp, {source_account_id, {target_account_id, amount}}>
    map<int, tuple<string, string, int, bool>> transactionHistory; // Map to record transaction history with timestamp

    int totalScheduledPayments = 0;

public:
    bool validAccount(const string &account_id) {
        return accounts.find(account_id) != accounts.end();
    }

    bool create_account(int timestamp, const string &account_id) {
        if (validAccount(account_id)) {
            transactionHistory[timestamp] = make_tuple(account_id, "", 0, false);
            return false;
        }
        accounts[account_id] = 0;
        outgoing_transactions[account_id] = 0;
        ranking.insert({0, account_id});
        transactionHistory[timestamp] = make_tuple(account_id, "", 0, true);
        return true;
    }

    bool deposit(int timestamp, const string &account_id, int amount) {
        if (!validAccount(account_id)) {
            transactionHistory[timestamp] = make_tuple(account_id, "", amount, false);
            return false;
        }
        accounts[account_id] += amount;
        transactionHistory[timestamp] = make_tuple(account_id, "", amount, true);
        return true;
    }

    bool transfer(int timestamp, const string &source_account_id, const string &target_account_id, int amount) {
        // Process scheduled payments before the transfer
        for(auto it = scheduledPayments.begin(); it != scheduledPayments.end(); ) {
            if(it->first <= timestamp) {
                string source = it->second.first;
                string target = it->second.second.first;
                int scheduled_amount = it->second.second.second;

                if (validAccount(source) && validAccount(target) && accounts[source] >= scheduled_amount) {
                    accounts[source] -= scheduled_amount;
                    accounts[target] += scheduled_amount;

                    // Update the ranking for outgoing transactions
                    ranking.erase({outgoing_transactions[source], source});
                    outgoing_transactions[source] += scheduled_amount;
                    ranking.insert({outgoing_transactions[source], source});
                }

                it = scheduledPayments.erase(it); // Remove processed payment
            } else {
                break;
            }
        }

        if (source_account_id == target_account_id) {
            transactionHistory[timestamp] = make_tuple(source_account_id, target_account_id, amount, false);
            return false;
        }

        if (!validAccount(source_account_id) || !validAccount(target_account_id) || accounts[source_account_id] < amount) {
            transactionHistory[timestamp] = make_tuple(source_account_id, target_account_id, amount, false);
            return false;
        }

        // Update the balance
        accounts[source_account_id] -= amount;
        accounts[target_account_id] += amount;

        // Update the ranking for outgoing transactions
        ranking.erase({outgoing_transactions[source_account_id], source_account_id});
        outgoing_transactions[source_account_id] += amount;
        ranking.insert({outgoing_transactions[source_account_id], source_account_id});

        transactionHistory[timestamp] = make_tuple(source_account_id, target_account_id, amount, true);
        return true;
    }

    // Function to print accounts based on their outgoing transactions ranking
    void print_ranking() {
        for (const auto& [amount, account_id] : ranking) {
            cout << account_id << ": " << amount << " outgoing transactions" << endl;
        }
    }

    bool schedule_payment(int timestamp, const string &source_account_id, const string &target_account_id, int amount, int delay) {
        if (source_account_id == target_account_id) return false;

        if (!validAccount(source_account_id) || !validAccount(target_account_id) || accounts[source_account_id] < amount) return false;

        totalScheduledPayments++;
        scheduledPayments.insert({timestamp + delay, {source_account_id, {target_account_id, amount}}});
        transactionHistory[timestamp] = make_tuple(source_account_id, target_account_id, amount, true);
        return true;
    }

    bool cancel_payment(int timestamp, const string &source_account_id, const string &target_account_id, int amount, int delay) {
        if (source_account_id == target_account_id) return false;

        if (!validAccount(source_account_id) || !validAccount(target_account_id)) return false;

        auto it = scheduledPayments.find({timestamp + delay, {source_account_id, {target_account_id, amount}}});
        if (it != scheduledPayments.end()) {
            scheduledPayments.erase(it);
            totalScheduledPayments--;
            transactionHistory[timestamp] = make_tuple(source_account_id, target_account_id, amount, false);
            return true;
        }
        return false;
    }

    // Function to print the transaction history
    void print_transaction_history() {
        for (const auto& [timestamp, details] : transactionHistory) {
            const auto& [source, target, amount, success] = details;
            cout << "Timestamp: " << timestamp 
                 << ", Source: " << source 
                 << ", Target: " << target 
                 << ", Amount: " << amount 
                 << ", Success: " << (success ? "Yes" : "No") 
                 << endl;
        }
    }

    bool merge_accounts(const string &account_id1, const string &account_id2) {
        if (!validAccount(account_id1) || !validAccount(account_id2)) return false;

        // Merge balances
        accounts[account_id1] += accounts[account_id2];
        accounts.erase(account_id2);

        // Merge outgoing transactions
        outgoing_transactions[account_id1] += outgoing_transactions[account_id2];
        outgoing_transactions.erase(account_id2);

        // Update ranking
        ranking.erase({outgoing_transactions[account_id2], account_id2});
        ranking.insert({outgoing_transactions[account_id1], account_id1});

        return true;
    }
};

int main() {
    BankingSystem bs;

    // Test create_account
    cout << bs.create_account(1, "A123") << endl; // Expected output: 1 (true)
    cout << bs.create_account(2, "B456") << endl; // Expected output: 1 (true)
    cout << bs.create_account(3, "A123") << endl; // Expected output: 0 (false)

    // Test deposit
    cout << bs.deposit(4, "A123", 500) << endl; // Expected output: 1 (true)
    cout << bs.deposit(5, "C789", 1000) << endl; // Expected output: 0 (false)

    // Test transfer
    cout << bs.transfer(6, "A123", "B456", 200) << endl; // Expected output: 1 (true)
    cout << bs.transfer(7, "A123", "B456", 300) << endl; // Expected output: 1 (true)
    cout << bs.transfer(8, "A123", "B456", 500) << endl; // Expected output: 0 (false)

    // Test schedule_payment
    cout << bs.schedule_payment(9, "A123", "B456", 100, 5) << endl; // Expected output: 1 (true)

    // Test cancel_payment
    cout << bs.cancel_payment(10, "A123", "B456", 100, 5) << endl; // Expected output: 1 (true)

    // Test merge_accounts
    cout << bs.create_account(11, "C789") << endl; // Expected output: 1 (true)
    cout << bs.deposit(12, "C789", 200) << endl; // Expected output: 1 (true)
    cout << bs.merge_accounts("A123", "C789") << endl; // Expected output: 1 (true)

    // Print transaction history
    bs.print_transaction_history();

    return 0;
}
