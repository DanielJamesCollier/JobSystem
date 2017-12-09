// std
#include <iostream>
#include <string>
#include <array>

// my
#include "job.hpp"

void empty_job(job *parent, void const *data){
    for (int i = 0; i < 100; i++) {
        std::cout << "doing some stuff\n"; 
    }
}

//---------------------------------
int 
main() {
   

    std::array<int, 10> test;

    job one {&empty_job}; 
    job two {&empty_job, &one};
    job thr {&empty_job, test};
    job fou {&empty_job, &one, test};

    one.run();
    two.run();
    thr.run();
    fou.run();
    
    std::string pause;
    std::cout << "press enter to continue\n";
    std::cin >> pause;
    return 0;
}
