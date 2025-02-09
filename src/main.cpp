#include <cstdlib>

#include <CxxUtils/data_types/multi_vector.hpp>
#include <iostream>

using namespace CxxUtils;
int main()
{
    std::cout << "Hello, World!" << std::endl;
    MultiVector<int, float, double, bool, char, short, unsigned, unsigned char, long long, long double> vec_{};
    vec_.push_back<long double>(1.0);
    std::cout << vec_.front<long double>() << std::endl;

    return EXIT_SUCCESS;
}
