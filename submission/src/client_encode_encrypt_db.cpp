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
    std::ifstream("../io/intermediate/plain_db.bin",std::ios::binary).read((char*)&x, sizeof(double));
    std::vector<double> db = {x};
    auto ptxt = cc->MakeCKKSPackedPlaintext({db});
    auto ctxt = cc->Encrypt(pk, ptxt);

    Serial::SerializeToFile("../io/ciphertexts_upload/cipher_db.bin", ctxt, SerType::BINARY);

    return 0;
}