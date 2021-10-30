//testing std::unordered_set

#include <unordered_set>
#include <iostream>
#include <ctime>
#include <random>

using namespace std;

class Bank_account{
public:
    int id;
    int money = 0;
    Bank_account() {
        static int _id = 0;
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dist(100, 1000000);
        int _money = dist(gen);
        id = _id;
        _id += 1;
        money = _money;
    }
    explicit Bank_account(int _id) {
        id = _id;
    }
};

extern int SIZE_OF_CLIENTS;

bool operator==(Bank_account const& b1, Bank_account const& b2) {
    return b1.id == b2.id;
}

struct Bank_account_hash {
    size_t operator()(const Bank_account& Bank_account) const {
        return hash<int>()(Bank_account.id);
    }
};




Bank_account Make_to_Compare(int id) {
    Bank_account temp(id);
    return temp;
}


void Money_Transfer_u(unordered_set<Bank_account, Bank_account_hash> &base, int from, int to, int amount) {
    auto sender = *base.find(Make_to_Compare(from));
    if (sender.money < amount) {
        cout << "User " << from << " do not have enough money" << endl;
        return;}
    auto recipient = *base.find(Make_to_Compare(to));
    {
        sender.money -= amount;
        recipient.money += amount;
    }
}
void Random_Transfers_u(unordered_set<Bank_account, Bank_account_hash> &base, unsigned int number) {
    for (int i = 0; i < number;) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> rand_id(0, SIZE_OF_CLIENTS);
        Money_Transfer_u(base, rand_id(gen), rand_id(gen), 1);
        i++;
    }
}
void Fill_Clients_u(unordered_set<Bank_account, Bank_account_hash> &base, int number) {
    for (int i = 0 ; i < number;) {
        base.emplace(Bank_account());
        i++;
    }
}

int SIZE_OF_CLIENTS = 5000000;
int NUMBER_OF_TRANSACTIONS = 10000;

int main() {
    unordered_set<Bank_account, Bank_account_hash> Clients;

    cout << "Num of clients = " << SIZE_OF_CLIENTS << endl;
    cout << "Starting unordered_set part" << endl;


    auto start_time = clock();
    Clients.rehash((Clients.size() + 5000000) / Clients.max_load_factor() + 1);
    auto end_time = clock();
    cout << "Rehash time " << end_time - start_time << endl;

    start_time = clock();
    Fill_Clients_u(Clients, SIZE_OF_CLIENTS);
    end_time = clock();
    cout << "It took " << end_time - start_time << " for initiating" << endl;
    start_time = clock();
    Money_Transfer_u(Clients, 4765124, 1889657, 1);
    end_time = clock();
    cout << "It took " << end_time - start_time << " for one transfer" << endl;

    start_time = clock();
    Random_Transfers_u(Clients, NUMBER_OF_TRANSACTIONS);
    end_time = clock();
    cout << "It took " << end_time - start_time<< " for many transfers" << endl;

    cout << "Unordered_set has " << Clients.bucket_count() << " buckets for " << SIZE_OF_CLIENTS << " clients" << endl;
    cout << "Load factor "<< Clients.load_factor() << endl;

    start_time = clock();
    Clients.insert(Make_to_Compare(111925054));
    end_time = clock();
    cout << "It took "<< end_time - start_time << " for insertion" << endl;

    start_time = clock();
    Clients.insert(Make_to_Compare(3764981));
    end_time = clock();
    cout << "It took " << end_time - start_time << " for deletion" << endl;

}
