#include <iostream>
#include <string>


int main() {
    std::string muthu="thisis india";
    
    std::string copy=muthu;
    
    std::string moved=std::move(muthu);
    
    std::cout<<copy<<"\n"<<muthu<<"\n"<<moved<<std::endl;
}
