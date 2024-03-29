// Common/Random.cpp

#include "StdAfx.h"

#include <stdlib.h>

#ifndef _WIN32
#include <time.h>
#endif

#include "Random.h"

void CRandom::Init(unsigned int seed) { srand(seed); }

void CRandom::Init()
{
  Init((unsigned int)
    #ifdef _MY_WIN32
    GetTickCount()
    #else
    time(NULL)
    #endif
    );
}

int CRandom::Generate() const { return rand(); }
