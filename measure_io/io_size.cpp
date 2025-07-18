#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <folder path>\n";
    }

    std::uintmax_t bytes = 0;
    for (auto& p: std::filesystem::recursive_directory_iterator(std::string(argv[1])))
        if (p.is_regular_file()) bytes += p.file_size();
    std::cout << bytes << std::endl;

    return 0;
}
