#ifndef TESTHELPER_HPP
#define TESTHELPER_HPP

#include <functional>
#include <iostream>
#include <vector>

template<typename T>
void register_test_case(T const &names, std::vector<std::function<void()>> const &funcs)
{
    for(size_t i = 0; i != names.size(); ++i){
        std::cout<<i<<" : "<<names[i]<<std::endl;
    }

    size_t input = 0;
    std::cin>>input;
    if(input < names.size()){
        std::cout<<"\nexecute "<<input<<" : "<<names[input]<<"\n"<<std::endl;
        funcs[input]();
    }else{
        std::cout<<"out of range"<<std::endl;
    }
    std::cout<<"program exist"<<std::endl;
}

template<typename T>
void register_test_case(T const &name_func)
{
    for(size_t i = 0; i != name_func.size(); ++i){
        std::cout<<i<<" : "<<name_func[i].first<<std::endl;
    }

    size_t input = 0;
    std::cin>>input;
    if(input < name_func.size()){
        std::cout<<"\nexecute "<<input;
        std::cout<<" : "<<name_func[input].first;
        std::cout<<"\n"<<std::endl;
        name_func[input].second();
    }else{
        std::cout<<"out of range"<<std::endl;
    }
    std::cout<<"program exist"<<std::endl;
}

#endif // TESTHELPER_HPP
