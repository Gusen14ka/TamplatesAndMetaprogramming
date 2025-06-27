#include "hpp/templatePrint.hpp"
#include <tuple>

int main(){
    std::tuple tp {5, 1.1, "hell"};
    std::cout << tp << std::endl;
}
