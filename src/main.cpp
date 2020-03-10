#include<iostream>

#include "include/knight.hpp"

int main(int argc, char** argv) {
    // argv[0] => program name
    // argv[1] => board size
    // argv[2] => method type
    //
    size_t n = atoi(argv[1]);
    int8_t opt = (argc > 2) ? atoi(argv[2]) : 1;

    //for (size_t i=1; i <= n; ++i) {
    //    std::cout << "Board Size: " << i << "; Steps: " 
    //        << kc::knight_steps(i,opt) << std::endl;
    //}
    kc::knight_steps(n,opt);

    return 0;
}
