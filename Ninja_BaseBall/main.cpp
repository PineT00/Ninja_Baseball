#include "pch.h"

int main()
{
    FRAMEWORK.Init(640 * 2, 480 * 2, "Ninja Baseball");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}