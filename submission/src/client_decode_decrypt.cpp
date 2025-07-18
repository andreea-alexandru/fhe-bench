#include "openfhe.h"
// header files needed for de/serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"

using namespace lbcrypto;

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <test_case>\n";
        return 1;
    }

    CryptoContext<DCRTPoly> cc;

    if (!Serial::DeserializeFromFile("../io/public_keys/cc.bin", cc,
                                    SerType::BINARY)) {
        throw std::runtime_error("Failed to get CryptoContext from ../io/public_keys/cc.bin");
    }

  PrivateKey<DCRTPoly> sk;
  if (!Serial::DeserializeFromFile("../io/secret_key/sk.bin", sk,
                                   SerType::BINARY)) {
    throw std::runtime_error("Failed to get secret key from ../io/secret_key/sk.bin");
  }
    Ciphertext<DCRTPoly> ctxt;     
    if (!Serial::DeserializeFromFile("../io/ciphertexts_download/cipher_sum.bin", ctxt, SerType::BINARY)) {
      throw std::runtime_error("Failed to get ciphertext from ../io/ciphertexts_download/cipher_sum.bin");
    }

    Plaintext ptxt; 
    cc->Decrypt(sk, ctxt, &ptxt);
    ptxt->SetLength(1);
    double res = ptxt->GetCKKSPackedValue()[0].real();

    // No post-processing, so write in the result file.
    std::ofstream("../io/result_" + std::string(argv[1]) + ".txt") << res << '\n';

    return 0;
}