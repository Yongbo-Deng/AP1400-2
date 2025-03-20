#include "../include/server.h"
#include "../include/client.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>

using namespace std;

Server::Server() {
}

std::string append_random_four_digit_number(const std::string &id) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);

    string result = id;
    for (int i = 0; i < 4; i++) {
        result += to_string(dis(gen));
    }
    return result;
}

std::shared_ptr<Client> Server::add_client(const std::string id) {
    string client_id = id;
    if (get_client(client_id)!= nullptr) {
        client_id = append_random_four_digit_number(id);
    }
    auto client = std::make_shared<Client>(client_id, *this);
    clients.insert({client, 5});
    cout << "Client: " << client->get_id() << endl;
    return client;
}


std::shared_ptr<Client> Server::get_client(std::string id) const{
    for (const auto &client: clients) {
        if (client.first->get_id() == id) {
            return client.first;
        }
    }
    return nullptr;
}

double Server::get_wallet(std::string id) const{
    shared_ptr<Client> client = get_client(id);
    if (client == nullptr) {
        return -1;
    }
    return clients.at(client);
}

bool Server::parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value) {
    /* Check the format of trx. */
    size_t first_dash = trx.find("-");
    size_t second_dash = trx.find("-", first_dash + 1);
    if (first_dash == string::npos || second_dash == string::npos) {
        throw runtime_error("Invalid transaction format.");
    }

    sender = trx.substr(0, first_dash);
    receiver = trx.substr(first_dash + 1, second_dash - first_dash - 1);
    value = stod(trx.substr(second_dash + 1));
    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const {
    string sender, receiver;
    double value;

    parse_trx(trx, sender, receiver, value);
    auto sender_client = get_client(sender);
    auto receiver_client = get_client(receiver);
    if (sender_client == nullptr || receiver_client == nullptr) {
        return false;
    } else if (get_wallet(sender) < value) {
        return false;
    }

    bool authentic = crypto::verifySignature(sender_client->get_publickey(), trx, signature);
    if (authentic) {
        pending_trxs.push_back(trx);
        return true;
    }
    return false;
}

size_t Server::mine() {
    string mempool{};
    string mining{};
    size_t nonce = 0;
    bool found = false;

    for (const auto &str : pending_trxs) {
        mempool += str;
    }

    cout << "mempool: " << mempool << endl;

    while (!found) {
        for (const auto &client : clients) {
            nonce = client.first->generate_nonce();
            mining = mempool + to_string(nonce);
            string hash = crypto::sha256(mining);
            if (hash.substr(0, 10).find("000") != string::npos) {
                found = true;
                cout << "MINER is: " << client.first->get_id() << endl;
                clients.at(client.first) += 6.25;
                finish_transactions();
            }
        }
    }
    return nonce;
}




void Server::finish_transactions() {
    string sender, receiver;
    double value;

    for (const string &str : pending_trxs) {
        Server::parse_trx(str, sender, receiver, value);
        if (clients.at(get_client(sender)) < value) {
            cout << "Insufficient balance for transaction: " << str << endl;
            continue;
        }
        clients.at(get_client(sender)) -= value;
        clients.at(get_client(receiver)) += value;
    }
    pending_trxs.clear();
}


 void  show_wallets(const  Server& server)
 {
 	std::cout << std::string(20, '*') << std::endl;
 	for(const auto& client: server.clients)
 		std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
 	std::cout << std::string(20, '*') << std::endl;
 }

