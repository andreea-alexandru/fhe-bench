#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <test_case>\n";
        return 1;
    }

    std::string db_path    = "../datasets/DB_" + std::string(argv[1]) + ".txt";
    std::string query_path = "../datasets/query_" + std::string(argv[1]) + ".txt";

    double q, d;

    std::ifstream db(db_path);
    if (!db.is_open())
        throw std::runtime_error("Cannot open " + db_path);
    if (!(db >> d))
        throw std::runtime_error("Failed to read double from " + db_path);

    std::ifstream qry(query_path);
    if (!qry.is_open())
        throw std::runtime_error("Cannot open " + query_path);
    if (!(qry >> q))
        throw std::runtime_error("Failed to read double from " + query_path);

    std::ofstream("../io/intermediate/plain_db.bin", std::ios::binary).write((char*)&d, sizeof(double));
    std::ofstream("../io/intermediate/plain_q.bin", std::ios::binary).write((char*)&q, sizeof(double));

    return 0;
}