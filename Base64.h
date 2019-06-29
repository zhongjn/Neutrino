// ***此代码源于互联网***
// ***此代码源于互联网***
// ***此代码源于互联网***
#ifndef BASE_64_H
#define BASE_64_H
#include <string>
std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);
#endif