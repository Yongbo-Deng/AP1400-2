#include "../include/client.h"
#include "../include/server.h"
#include <random>
#include <string>


using namespace std;

Client::Client(string id, const Server& server) : server(&server), id(id) {
    crypto::generate_key(public_key, private_key);
}



std::string Client::get_id() {
    return this->id;
}

std::string Client::get_publickey() const {
    return this->public_key;
}

double Client::get_wallet() const{
    return server->get_wallet(this->id);
}

std::string Client::sign(std::string txt) const {
    string signature = crypto::signMessage(private_key, txt);
    return signature;
}

bool Client::transfer_money(std::string receiver, double value) const {
    string trx = this->id + "-" + receiver + "-" + to_string(value);
    return server->add_pending_trx(trx, sign(trx));
}


size_t Client::generate_nonce() const {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 99999);
    return dis(gen);
}
