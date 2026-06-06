#include <iostream>
#include <string>

class safe{
    public:
    std::string data;
    safe(std::string a):data(a){}
    
    safe(safe &&o)noexcept: data(std::move(o.data)){
        std::cout<<"safe moved called"<<std::endl;
    }
    
    safe(const safe &o) : data(o.data){
        std::cout<<"safe copied called"<<std::endl;
    }
};

class Safe{
    public:
    std::string data;
    Safe(std::string a):data(a){}
    
    Safe(Safe &&o): data(std::move(o.data)){
        std::cout<<"Safe moved called"<<std::endl;
    }
    
    Safe(const Safe &o) : data(o.data){
        std::cout<<"Safe copied called"<<std::endl;
    }
};

int main() {
    std::string muthu="thisis india";
    safe m("muthu");
    Safe m1("muthu");
    safe s2=std::move_if_noexcept(m);
    Safe r2=std::move_if_noexcept(m1);
}
