#ifndef GENERIC_FUNC_H
#define GENERIC_FUNC_H


// string: a formatálni kívánt string
// mögötte printf()-hez hasonlóan a formáló változók
// vissszaadja a dinamikusan foglalt összerakott stringet
// remember_dmalloc()-al foglal
char* StringAssembler(char* string, ...);

#endif