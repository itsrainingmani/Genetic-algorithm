#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <string>

std::vector<std::string> initPopulation()
{
    std::vector<std::string> population;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 36; j++)
        {
            int bit = rand()%2;
            population[i] += (char)bit;
        }
        //std::cout<<population[i]<<std::endl;
    }
    return population;
}

int main()
{
    int target = 0;
    std::vector<std::string> sample;
    std::cout<<"Please enter the target number"<<std::endl;
    std::cin>>target;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 36; j++)
        {
            int bit = rand()%2;
            sample[i] += (char)bit;
        }
        //std::cout<<population[i]<<std::endl;
    }
}
