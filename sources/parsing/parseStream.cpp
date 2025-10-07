#include "webserv.hpp"

// Code duplciation
static inline bool isComment(const std::string& line) {
    return !line.empty() && *line.begin() == '#';
}

// Code duplciation
static inline bool isGrammarToken(const char c) {
	return (c == GRAMMAR_OPEN || c == GRAMMAR_CLOSE || c == GRAMMAR_STOP);
}

// Supress excessive whitespaces and add some (pretty clear comment right xd)
static std::string normalizeWhitespaces(std::string& line) {
    bool space = false;
    std::string processed;
    std::string::iterator it = line.begin();
    std::string::iterator end = line.end();

    while (it != line.end() && isspace(static_cast<unsigned char>(*it))) {
        ++it;
    }
    for (; it != end; ++it) {
        bool is_space = isspace(static_cast<unsigned char>(*it));
		if (isGrammarToken(*it)) {
			if (!space)
				processed += ' ';
			processed += *it;
			processed += ' ';
			space = true;
		} else if (!is_space) {
            processed += *it;
            space = false;
        } else if (!space) {
            processed += ' ';
            space = true;
        }
    }
    if (!processed.empty() && *processed.end() == ' ') {
        processed.erase(processed.size() - 1);
    }
    return processed;
}

static std::vector<std::string> tokenizeLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string currentToken;

    for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
        char c = *it;
        if (isGrammarToken(c)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        } else if (isspace(static_cast<unsigned char>(c))) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }
    return tokens;
}

static std::vector<std::string> processLine(std::string& line) {
    if (isComment(line)) {
        return std::vector<std::string>();
    }
    std::string normalizedLine = normalizeWhitespaces(line);
    return tokenizeLine(normalizedLine);
}

std::vector<std::string> parseStream(std::ifstream& configFile) {
    std::vector<std::string> tokens;
    std::string line;
    try {
        while (std::getline(configFile, line)) {
            std::vector<std::string> lineTokens = processLine(line);
            tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
        }
        if (configFile.bad()) {
            throw CustomException(std::string(IO_CONFIG_ERROR), READ_ERROR_CODE);
        }
    } catch (const std::ios::failure& e) {
        throw CustomException(std::string(e.what()), READ_ERROR_CODE);
    }
    return tokens;
}
