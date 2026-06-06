#include<iostream>
#include<string>

class hello{
public:
std::string msg;
  hello(std::string msg){
      this->msg=msg;
  }
  
  hello& append(std::string word)
  {
      this->msg +=word;
      return *this;
  }
  void print(){
      std::cout<<this->msg<<"\n";
      std::cout<<"this addess"<<this<<std::endl;
  }
};

int main(){
    hello n("muthu engal sothu");
    n.append("yaru sothu").append("c++ sothu").print();
}
