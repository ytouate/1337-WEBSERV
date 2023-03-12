#include "AConfig.hpp"

AConfig::AConfig() {}
AConfig::~AConfig()
{
    data.clear();
}

bool AConfig::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\t');
}

void AConfig::error(const std::string &s) const
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}