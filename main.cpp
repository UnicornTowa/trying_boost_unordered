#include <iostream>
#include <random>
#include "boost/unordered/unordered_set.hpp"



using namespace std;

class Bank_account{
public:
    unsigned int id;
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
    explicit Bank_account(unsigned int _id) {
         id = _id;
     }
};

extern int SIZE_OF_CLIENTS;

bool operator==(Bank_account const& b1, Bank_account const& b2) {
    return b1.id == b2.id;
}
bool operator==(Bank_account const& b, int const& id) {
    return b.id == id;
}
size_t hash_value(Bank_account const& b) {
    size_t seed = 0;
    boost::hash_combine(seed, b.id);
    return seed;
}
Bank_account Make_to_Compare(unsigned int id) {
    Bank_account temp(id);
    return temp;
}
//unordered part
void Money_Transfer_u(boost::unordered::unordered_set<Bank_account> &base, int from, int to, int amount) {
    Bank_account sender = *base.find(Make_to_Compare(from));
    if (sender.money < amount) {
        cout << "User " << from << " do not have enough money" << endl;
        return;}
    Bank_account recipient = *base.find(Make_to_Compare(to));
    {
        sender.money -= amount;
        recipient.money += amount;
    }
}
void Random_Transfers_u(boost::unordered::unordered_set<Bank_account> &base, unsigned int number) {
    for (int i = 0; i < number;) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> rand_id(0, SIZE_OF_CLIENTS);
        Money_Transfer_u(base, rand_id(gen), rand_id(gen), 1);
        i++;
    }
}
void Fill_Clients_u(boost::unordered::unordered_set<Bank_account> &base, int number) {
    for (int i = 0 ; i < number;) {
        Bank_account temp;
        base.insert(temp);
        i++;
    }
}
//vector part
void Fill_Clients_v(vector<Bank_account> &base, int number) {
    for (int i = 0; i < number;) {
        Bank_account temp;
        base.push_back(temp);
        i++;
    }
}
void Money_Transfer_v(vector<Bank_account> &base, int from, int to, int amount) {
    Bank_account sender = *find(base.begin(), base.end(), Make_to_Compare(from));
    if (sender.money < amount) {
        cout << "Sender " << from << " do not have enough money" << endl;
        return;}
    Bank_account recipient = *find(base.begin(), base.end(), Make_to_Compare(to));
    {
        sender.money -= amount;
        recipient.money += amount;
    }
}
void Random_Transfers_v(vector<Bank_account> &base, int number) {
    for (int i = 0; i < number;) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> rand_id(SIZE_OF_CLIENTS, 2 * SIZE_OF_CLIENTS);
        Money_Transfer_v(base, rand_id(gen), rand_id(gen), 1);
        i++;
    }
}

int SIZE_OF_CLIENTS = 5000000;
int NUMBER_OF_TRANSACTIONS = 10000;

int main() {
    cout << "Num of clients = " << SIZE_OF_CLIENTS << endl;
    cout << "Starting unordered_set part" << endl;
    boost::unordered::unordered_set<Bank_account> Clients;

    auto start_time = clock();
    Fill_Clients_u(Clients, SIZE_OF_CLIENTS);
    auto end_time = clock();
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
    cout << "Max load factor " << Clients.max_load_factor() << endl;

    start_time = clock();
    Clients.insert(Make_to_Compare(111925054));
    end_time = clock();
    cout << "It took "<< end_time - start_time << " for insertion" << endl;

    start_time = clock();
    Clients.insert(Make_to_Compare(3764981));
    end_time = clock();
    cout << "It took " << end_time - start_time << " for deletion" << endl;



    cout << "Unordered_set part finished. Starting vector part" << endl;

    vector<Bank_account> Clients_v;
    start_time = clock();
    Fill_Clients_v(Clients_v, SIZE_OF_CLIENTS);
    end_time = clock();
    cout << "It took " << end_time - start_time << " for initiating vector" << endl;

    start_time = clock();
    Money_Transfer_v(Clients_v, 5567653, 7356881, 1);
    end_time = clock();
    cout << "It took " << end_time - start_time << " for one transfer" << endl;

    start_time = clock();
    Random_Transfers_v(Clients_v, 10000);
    end_time = clock();
    cout << "It took " << end_time - start_time << " for many transfers" << endl;

    start_time = clock();
    Clients_v.erase(find(Clients_v.begin(), Clients_v.end(), Make_to_Compare(6712346)));
    end_time = clock();
    cout << "It took " << end_time - start_time << " for deletion" << endl;

    start_time = clock();
    Clients_v.push_back(Make_to_Compare(98452378));
    end_time = clock();
    cout << "It took " << end_time - start_time << " for back insertion" << endl;

    start_time = clock();
    Clients_v.insert(Clients_v.begin() + 3578954, Make_to_Compare(120550259));
    end_time = clock();
    cout << "It took " << end_time - start_time << " for insertion" << endl;


    return 0;
}
