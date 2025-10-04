#pragma once

#include <vector>
#include <string>

class Location {
    std::vector<std::string> paths;

    public:

        Location();
        ~Location();

        Location operator=(const Location& src);
};
