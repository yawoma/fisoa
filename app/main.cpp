#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Test the app for " << argc << " arguments that will be ignored" << '\n';
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << '\n'; //NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    return 0;
}