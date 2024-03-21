#include "pch.h"

int main()
{
    FRAMEWORK.Init(1280, 960, "Ninja Baseball");
    //FRAMEWORK.Init(960, 720, "Ninja Baseball");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}