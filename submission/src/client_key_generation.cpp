#include "openfhe.h"
// header files needed for de/serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"

using namespace lbcrypto;

int main(){

    // Step 1: Setup CryptoContext
    uint32_t multDepth = 0;
    uint32_t scaleModSize = 40;

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);
    cc->Enable(PKE);
    cc->Enable(LEVELEDSHE);

    // Step 2: Key Generation
    auto keys = cc->KeyGen();

    // Step 3: Serialize cryptocontext and keys
    if (!Serial::SerializeToFile("../io/public_keys/cc.bin", cc,
                                SerType::BINARY) ||
        !Serial::SerializeToFile("../io/public_keys/pk.bin",
                                keys.publicKey, SerType::BINARY) ||
        !Serial::SerializeToFile("../io/secret_key/sk.bin",
                                keys.secretKey, SerType::BINARY)) {
        throw std::runtime_error("Failed to write keys to ../io/");
    }

    return 0;
}