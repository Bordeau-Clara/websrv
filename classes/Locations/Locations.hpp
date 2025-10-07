#pragma once

#include <vector>
#include <string>

class Location {
	
	std::string _path;
    std::vector<std::string> _methods;
	
	Location();

    public:

        Location(
				std::string& path, 
				std::vector<std::string> methods);
		Location(const Location& src);
        ~Location();

    	Location& operator=(const Location& src);

		void print();

		std::string getPath() const;
		std::vector<std::string> getMethod() const;
};
