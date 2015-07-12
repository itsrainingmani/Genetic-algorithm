//-----------------------------------------ga_tutorial.cpp--------------------------------------
//
//	code to illustrate the use of a genetic algorithm to solve a number
//	by Manikandan Sundararajan
//
//-----------------------------------------------------------------------------------------------
#include <string>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <sstream>
#include <vector>

using std::string;

#define CROSSOVER_RATE            0.85
#define MUTATION_RATE             0.002
#define POP_SIZE                  100			//must be an even number
#define CHROMO_LENGTH             300
#define GENE_LENGTH               4
#define MAX_ALLOWABLE_GENERATIONS	10000

//returns a float between 0 & 1

//#define RANDOM_NUM		((float)rand()/(float)(RAND_MAX+1));
#define RANDOM_NUM		((float)rand()/(RAND_MAX+1))

struct chromo_type
{
    string bits;
    float fitness;

    chromo_type():bits(""),fitness(0.0f){}
    chromo_type(string bts, float ftns):bits(bts), fitness(ftns){}
};

string GetRandomBits(int length);
float assignFitness(string chr, int target_value);
int numberValue(string flt);
int parseBits(int* buffer, string chr);
void mutation(string &offspring);
void crossover(string &offspring1, string &offstring2);
string roulette(int total_fitness, chromo_type* Population);
void printChromo(string bits);
void printGeneSymbol(string val);

//prototypes
int main()
{
    srand((int)time(NULL));
    while (true)
    {
	chromo_type population[POP_SIZE];
	float target;
	std::cout << "Please enter the target: ";
	std::cin >> target;
	for (int i = 0; i < POP_SIZE; i++)
	{
		population[i].bits = GetRandomBits(CHROMO_LENGTH);
		population[i].fitness = 0.0f;
	}

	bool bFlag = false;
	int iMaxNumberOfGenerations = 0;

	while (!bFlag)
	{
		float fTotalFitness = 0.0f;
		for (int i = 0; i < POP_SIZE; i++)
		{
			population[i].fitness = assignFitness(population[i].bits, target);
			fTotalFitness += population[i].fitness;
		}
		for (int i = 0; i < POP_SIZE; i++)
		{
			if (population[i].fitness == 999.0f)
			{
				std::cout << "\nSolution found in " << iMaxNumberOfGenerations << " generations!" << std::endl;
				printChromo(population[i].bits);
				bFlag = true;
				break;
			}

		}


		chromo_type temp[POP_SIZE];
		int cPop = 0;

		while (cPop < POP_SIZE)
		{
			string off1 = roulette(fTotalFitness, population);
			string off2 = roulette(fTotalFitness, population);
			crossover(off1, off2);
			mutation(off1);
			mutation(off2);

			temp[cPop++] = chromo_type(off1, 0.0f);
			temp[cPop++] = chromo_type(off2, 0.0f);
		}

		for (int i = 0; i<POP_SIZE; i++)
		{
			population[i] = temp[i];
		}

		++iMaxNumberOfGenerations;

		if (iMaxNumberOfGenerations > MAX_ALLOWABLE_GENERATIONS)
		{
			std::cout << "Max number of Generations " << iMaxNumberOfGenerations << " has been reached. No Solution was found" << std::endl;
			bFlag = true;
		}
	}
	std::cout<<"\n\n\n";
    }
	return 0;
}

string	GetRandomBits(int length)
{
	string bits;
	for (int i = 0; i<length; i++)
	{
		if (RANDOM_NUM > 0.5f)
			bits += "1";
		else
			bits += "0";
	}
	return bits;
}

int numberValue(string flt)
{
	int num = 0;
	int numToAdd = 1;
	for (int i = flt.length(); i > 0; i--)
	{
		if (flt.at(i-1) == '1')
        {
             num += numToAdd;
        }
		numToAdd *= 2;
	}
	return num;
}

void printGeneSymbol(int val)
{
	if (val < 10)
	{
		std::cout << val<" ";
	}
	else
	{
		switch (val)
		{
		case 10: std::cout << "+";break;
		case 11: std::cout << "-";break;
		case 12: std::cout << "*";break;
		case 13: std::cout << "/";break;
		}
		std::cout << " ";
	}
	return;
}

int parseBits(int* buffer, string chr)
{
	int buffLength = 0;
	bool isOper = true;
	int t_gene = 0;
	for (int i = 0; i < CHROMO_LENGTH; i += GENE_LENGTH)
	{
		t_gene = numberValue(chr.substr(i, GENE_LENGTH));
		if (isOper)
		{
			if (t_gene <10 || t_gene > 13)
			{
				continue;
			}
			else
			{
				isOper = false;
				buffer[buffLength++] = t_gene;
				continue;
			}
		}
		else
		{
			if (t_gene > 9)
				continue;
			else
			{
			    isOper = true;
				buffer[buffLength++] = t_gene;
				continue;
			}
		}
	}

	for (int i = 0; i < buffLength; i++)
	{
		if (buffer[i] == 13 && buffer[i + 1] == 0)
			buffer[i] = 10;
	}
	return buffLength;
}

float assignFitness(string chr, int target_value)
{
	int buffer[(int)CHROMO_LENGTH / GENE_LENGTH];
	int buffSize = parseBits(buffer,chr);
	float result = 0.0f;
	for (int i = 0; i < buffSize - 1; i += 2)
	{
		switch (buffer[i])
		{
		case 10: result += buffer[i + 1]; break;
		case 11: result -= buffer[i + 1]; break;
		case 12: result *= buffer[i + 1]; break;
		case 13: result /= buffer[i + 1]; break;
		}
	}
	if (result == (float)target_value)
	{
		return 999.0f;
	}
	else
	{
		return 1 / (float)fabs((double)(target_value - result));
	}
}

void printChromo(string bits)
{
	int buffer[(int)CHROMO_LENGTH / GENE_LENGTH];
	int numLen = parseBits(buffer, bits);
	for (int i = 0; i < numLen; i++)
	{
		printGeneSymbol(buffer[i]);
	}
}

void mutation(string &off)
{
	for (int i = 0; i < CHROMO_LENGTH; i++)
	{
		char& bitVal = off[i];
		if (RANDOM_NUM <= MUTATION_RATE)
		{
			if (bitVal == '0')
				off[i] = '1';
			else
				off[i] = '0';
		}
		else
			continue;
	}
	return;
}

string roulette(int total_fitness, chromo_type* Population)
{
	//generate a random number between 0 & total fitness count
	float Slice = (float)(RANDOM_NUM * total_fitness);
	//go through the chromosones adding up the fitness so far
	float FitnessSoFar = 0.0f;
	for (int i = 0; i<POP_SIZE; i++)
	{
		FitnessSoFar += Population[i].fitness;
		//if the fitness so far > random number return the chromo at this point
		if (FitnessSoFar >= Slice)
			return Population[i].bits;
	}
	return "";
}

void crossover(string &offspring1, string &offspring2)
{
	if (RANDOM_NUM < CROSSOVER_RATE)
	{
		//create a random crossover point
		int crossover = (int)(RANDOM_NUM * CHROMO_LENGTH);

		string t1 = offspring1.substr(0, crossover) + offspring2.substr(crossover, CHROMO_LENGTH);
		string t2 = offspring2.substr(0, crossover) + offspring1.substr(crossover, CHROMO_LENGTH);
		offspring1 = t1; offspring2 = t2;

	}

	return;

}
