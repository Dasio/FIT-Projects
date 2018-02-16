#ifndef COMMON_HPP
#define COMMON_HPP

#include "lib/ecc.h"

const size_t paritySize = NPAR;
// 255 is limit of library
const size_t dataSize = 255 - paritySize;
//const size_t dataSize = 145;
const size_t blockSize = dataSize + paritySize;

#endif
