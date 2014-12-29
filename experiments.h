/*
 experiments.h
 Pretty Simple
*/
#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include "stdlib.h"
#include "neat.h"
#include "network.h"
#include "population.h"
#include "organism.h"
#include "genome.h"
#include "species.h"
#include "robotSimulator.h"
#include <windows.h>

using namespace std;

using namespace NEAT;

//The Predator/Prey Experiment *****************************************
Population *predprey_test_nocomm(int gens);
int predprey_epoch_nocomm(Population *pred_one, Population *pred_two, Population *pred_three, int generation);
bool predprey_evaluate_nocomm(Organism *pred_one, Organism *pred_two, Organism *pred_three, int counter);

Population *predprey_test_comm(int gens);
int predprey_epoch_comm(Population *pred_one, Population *pred_two, Population *pred_three, int generation);
bool predprey_evaluate_comm(Organism *pred_one, Organism *pred_two, Organism *pred_three, int counter);

Population *predprey_test_central(int gens);
int predprey_epoch_central(Population *pred, int generation);
bool predprey_evaluate_central(Organism *pred, int counter);

double hunt(Network *pred_one, Network *pred_two=NULL, Network *pred_three=NULL, int *count=NULL);

int benchmark(int counter, int viz, Network *pred_one, Network *pred_two=NULL, Network *pred_three=NULL);
int benchmarkOnePred(int counter, Network *pred_one);

double convertOutputToAngle(double output[]);

int replay(char* filename);
int replayOnePred(char* filename);
int superbench(char* filename);

#endif






