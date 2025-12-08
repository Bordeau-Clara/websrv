#include <iostream>
#include <fstream>

int main()
{
    // Tu changes juste cette ligne pour changer le fichier généré :
    const char *filename = "RequestTest/chunked_4.txt";

    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file)
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
        return 1;
    }

    // Exemple de requête HTTP GET :
    file << "GET index.html HTTP/1.1\r\n";
    file << "Host: localhost:8002\r\n";
    file << "content-type:185\r\n";
    file << "Connection: close\r\n";
    file << "transfer-encoding:chunked\r\n";
    file << "\r\n"; // Fin des headers
    file << "A;foo=123;bar=xyz\r\nHelloWorld\r\n6;debug=true\r\nSalut!\r\n";
    file << "A;foo=123;bar=xyz\r\nHelloWorld\r\n6;debug=true\r\nSalut!\r\n0\r\n\r\n";

    file.close();
    std::cout << "Fichier " << filename << " généré avec succès !" << std::endl;

    return 0;
}

//g++ -Wall -Wextra -Werror -std=c++98 generate_request.cpp -o generate_request
