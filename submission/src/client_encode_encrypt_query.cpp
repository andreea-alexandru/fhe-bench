#include "openfhe.h"
// header files needed for de/serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"

using namespace lbcrypto;

int main(){
    CryptoContext<DCRTPoly> cc;

    if (!Serial::DeserializeFromFile("../io/public_keys/cc.bin", cc,
                                    SerType::BINARY)) {
        throw std::runtime_error("Failed to get CryptoContext from ../io/public_keys/cc.bin");
    }
    PublicKey<DCRTPoly> pk;
    if (!Serial::DeserializeFromFile("../io/public_keys/pk.bin", pk,
                                    SerType::BINARY)) {
        throw std::runtime_error("Failed to get public key from ../io/public_keys/pk.bin");
    }

    double x; 
    std::ifstream("../io/intermediate/plain_q.bin",std::ios::binary).read((char*)&x, sizeof(double));
    std::vector<double> query = {x};
    auto ptxt = cc->MakeCKKSPackedPlaintext(query);
    auto ctxt = cc->Encrypt(pk, ptxt);

    Serial::SerializeToFile("../io/ciphertexts_upload/cipher_q.bin", ctxt, SerType::BINARY);

    return 0;
}