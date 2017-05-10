#pragma once

#include "util.h"
#include <string>

bool Deflate(Bytevector &output, const Bytevector &input);
bool Inflate(Bytevector &output, const Bytevector &input);
std::string BytevectorToString(const Bytevector &vec);
Bytevector StringToBytevector(const std::string &str);