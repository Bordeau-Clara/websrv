#include "webserv.hpp"

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << USAGE << std::endl;
        return 1;
    }
    std::cout << "HELLO SERV" << std::endl;

    return 0;
}
