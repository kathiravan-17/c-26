#include <iostream>
#include <string>

void process(std::string& s){
    std::cout<<"lvalue"<<std::endl;
}

void process(std::string&& s){
    std::cout<<"rvalue"<<std::endl;
}
int main() {
    std::string muthu="thisis india";
    
    process(muthu);
    process("muthu engal sothu");
    process(std::move(muthu));

}
