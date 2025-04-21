#include "chatroomserver.hpp"

void Useage()
{
    std::cout<<"Useage: ./chatroom port"<<std::endl;
    exit(-1);
}

int main(int argc,char* argv[])
{
    int i = 0;
    if(argc< 2 )
    {
        Useage();
    }
    
    server s1(std::stoi(argv[1]));
    while(1)
    {
        // std::cout<<"i:"<<i++<<std::endl;
        s1.connect();
        s1.receive();
        s1.handlep();
        sleep(1);
    }
    
    return 0;
}