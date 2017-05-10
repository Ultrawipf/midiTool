#pragma once

#include "util.h"

#include <string>

std::string base64_encode(const Bytevector &);
Bytevector base64_decode(const std::string &);