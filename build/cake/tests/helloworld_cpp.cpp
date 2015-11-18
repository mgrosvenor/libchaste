#include <iostream>

int main(int argc, char** argv)
{
    #ifdef NDEBUG
        std::cout << "release " << argc << std::endl;
    #else
        std::cout << "debug " << argc << std::endl;
    #endif
    return 0;
}
