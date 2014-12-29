/*
 experiments.cpp
 THIS IS NOT THE ORIGINAL experiments.cpp file from the NEAT source!
 Right now this is just a test for the Predator/Prey experiment.
 Things might change in the future.
 */

#include "experiments.h"
#include <cstring>
#include <iostream>
#include <string>

//#define NO_SCREEN_OUT

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* ------------------------------------------------------------------ */
/* Predator/Prey Experiment                                           */
/* ------------------------------------------------------------------ */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/* ------------------------------------------------------------------ */
/* Non-Communicating Predators                                        */
/* ------------------------------------------------------------------ */

//Setting starting pray type to 0. We do prey_num-7 to calcuate actual prey type.
double prey_num = 7; // this is not being used right now. 
double input_divide = 10.0;
int maxSteps = 15;

/*
 Runs a non-communication simulation for given generations.
 This function by itself reads/loads parameter files and starting network.
 Calls a function pred_prey_epoch_nocomm.
 */
Population *predprey_test_nocomm(int gens) {
    Population *pred_one=0;
    Population *pred_two=0;
    Population *pred_three=0;
    
    Genome *pred_one_start_genome;
    Genome *pred_two_start_genome;
    Genome *pred_three_start_genome;
	
    int gen;
	
    // Starter genome file names
    char *pred_one_starter="pred1startgenesnocomm";
    char *pred_two_starter="pred2startgenesnocomm";
    char *pred_three_starter="pred3startgenesnocomm";
    
    // Pulling in starter genome files
    ifstream pred_one_file(pred_one_starter,ios::in);
    ifstream pred_two_file(pred_two_starter,ios::in);
    ifstream pred_three_file(pred_three_starter,ios::in);
    
    
    cout<<"Reading in the start genome"<<endl;
    char header_one[20];
    char header_two[20];
    char header_three[20];
    
    int id_one;
    int id_two;
    int id_three;
    
    //Read in the start genomes
    pred_one_file>>header_one;
    pred_one_file>>id_one;
    pred_two_file>>header_two;
    pred_two_file>>id_two;
    pred_three_file>>header_three;
    pred_three_file>>id_three;
    
    //Build the start genomes
    pred_one_start_genome = new Genome(id_one,pred_one_file);
    pred_two_start_genome = new Genome(id_two,pred_two_file);
    pred_three_start_genome = new Genome(id_three,pred_three_file);
    pred_one_file.close();
    pred_two_file.close();
    pred_three_file.close();
    
    //Run the specified number of runs
    cout<<"Start Genomes"<<endl;
    int run;
    for (run=0;run<NEAT::num_runs;run++) {
        
        cout<<"RUN #"<<run<<endl;
        
        //Build the Populations from starter genes
        cout<<"Spawning Populations off starter genome"<<endl;
        pred_one = new Population(pred_one_start_genome,NEAT::pop_size);
        pred_two = new Population(pred_two_start_genome,NEAT::pop_size);
        pred_three = new Population(pred_three_start_genome,NEAT::pop_size);
        
        //Run for the specified number of generations
        for (gen=1;gen<=gens;gen++) {
            cout<<"Epoch "<<gen<<endl;
            predprey_epoch_nocomm(pred_one, pred_two, pred_three, gen);
        }
    }
    delete pred_one_start_genome;
    delete pred_two_start_genome;
    delete pred_three_start_genome;
    delete pred_two;
    delete pred_three;
    return pred_one;
}

/*
 This function runs a simulation for 1 generation.
 Each individual in each population is tested 10 times with randomly chosen partners.
 When the simulation for the generation is complete, this function chooses the best individual from each population and
 runs benchmark tests on them.
 At the end, this function writes out the result of the benchmark test.
 */
int predprey_epoch_nocomm(Population *pred_one, Population *pred_two, Population *pred_three, int generation) {

	//Evaluate each organism on a test
	vector<Organism*> pred_one_copy; //= pred_one->organisms;
	vector<Organism*> pred_two_copy; //= pred_two->organisms;
	vector<Organism*> pred_three_copy; //= pred_three->organisms;

	int i;
	double bestfitness_one = -20;
	double bestfitness_two = -20;
	double bestfitness_three = -20;
	Organism *best_pred_one = *(pred_one->organisms).begin();
	Organism *best_pred_two = *(pred_two->organisms).begin();
	Organism *best_pred_three = *(pred_three->organisms).begin();

	cout << "break1" << endl;

    // each organism gets tested 10 times.
    for (i=0;i<10;i++) {
        
        //duplicate all organisms
        pred_one_copy = pred_one->organisms;
        pred_two_copy = pred_two->organisms;
        pred_three_copy = pred_three->organisms;
        
        int j;
        for(j=0;j<pop_size;j++){
            
            // Pull out random predators
            int random_choice1 = rand() % pred_one_copy.size();
            Organism* pred_one_curorg = pred_one_copy.at(random_choice1);
            int random_choice2 = rand() % pred_two_copy.size();
            Organism* pred_two_curorg = pred_two_copy.at(random_choice2);
            int random_choice3 = rand() % pred_three_copy.size();
            Organism* pred_three_curorg = pred_three_copy.at(random_choice3);
            
            //Test them
            predprey_evaluate_nocomm(pred_one_curorg, pred_two_curorg, pred_three_curorg, i+1);
            cout<<"Test "<<(i*pop_size)+(j+1)<<"; Generation "<<generation<<endl;
            
            //erase the orgnisms that are already tested.
            pred_one_copy.erase(pred_one_copy.begin()+random_choice1);
            pred_two_copy.erase(pred_two_copy.begin()+random_choice2);
            pred_three_copy.erase(pred_three_copy.begin()+random_choice3);
        }
    }
    
	cout << "break2" << endl;
    
    //Find which predators performed the best
    vector<Organism*>::iterator iter;
    for(iter=(pred_one->organisms).begin();iter!=(pred_one->organisms).end();++iter) {
        if ((*iter)->fitness > bestfitness_one) {
            bestfitness_one = (*iter)->fitness;
            best_pred_one = (*iter);
        }
    }
    for(iter=(pred_two->organisms).begin();iter!=(pred_two->organisms).end();++iter) {
        if ((*iter)->fitness > bestfitness_two) {
            bestfitness_two = (*iter)->fitness;
            best_pred_two = (*iter);
        }
    }
    for(iter=(pred_three->organisms).begin();iter!=(pred_three->organisms).end();++iter) {
        if ((*iter)->fitness > bestfitness_three) {
            bestfitness_three = (*iter)->fitness;
            best_pred_three = (*iter);
        }
    }
    
    //Run the best set on the benchmark
    cout<<"Running benchmark for gen "<<generation<<endl;
    
    int counter = 0;
    
    // Call bench mark function with 9 different starting locations.
    for (i=0;i<9;i++){
        int caught = benchmark(i, 0, best_pred_one->net, best_pred_two->net, best_pred_three->net);
        counter += caught;
    }
    
    //Write out the result into a generation file.
    char filedist2[100];
    int TempNumOne=filedist.size();
    for (int a=0;a<=TempNumOne;a++){
        filedist2[a]=filedist[a];
    }
    
    char outstring[100];
    sprintf (outstring, filedist2, generation);
    cout << outstring << endl;
    std::ofstream oFile(outstring);
    oFile<<"Best_Fitness_1: "<<best_pred_one->fitness<<endl;
    oFile<<"Best_Fitness_2: "<<best_pred_two->fitness<<endl;
    oFile<<"Best_Fitness_3: "<<best_pred_three->fitness<<endl;
    oFile<<"Completed_Benchmarks: "<<counter<<endl;
    oFile<<"Prey_Type: "<<7-prey_num<<endl;
    best_pred_one->write_to_file(oFile);
    best_pred_two->write_to_file(oFile);
    best_pred_three->write_to_file(oFile);
    oFile.close();
    
    
    if (counter >= 7 && prey_num > 1) {
        prey_num -= 1;
        cout << "************Prey is now moving faster!!!!************" << endl;
    }
    
    //Create the next generation
    pred_one->epoch(generation);
    pred_two->epoch(generation);
    pred_three->epoch(generation);
    return 1;
}

//-------------------
// New version of predprey_evaluate_nocomm.
// Average values are properly calculated in epoch_nocomm function by using this function.
// Evaluates a group of 3 predators 6 times and records the average fitness value.
//-------------------

bool predprey_evaluate_nocomm(Organism *pred_one, Organism *pred_two, Organism *pred_three, int counter) {
	
    //Try to catch the prey (run six times)
    double fitnesses_list[6];
    double newfitness;
	
    int i;
    for (i=0;i<6;i++){
        newfitness = hunt(pred_one->net, pred_two->net, pred_three->net); //LET THE HUNT BEGIN!!!!!
        fitnesses_list[i] = newfitness;
    }
    //Average fitness over six runs
    double fit_sum = 0.0;
	
    for (i=0;i<6;i++){
        fit_sum += fitnesses_list[i];
    }
    double average_fitness = fit_sum/6;
	
    //Assign the average fitness to organisms, averaging again with previously recorded firness values.
    //The number of runs that are already done is kept track by "counter".
    if (pred_one->fitness == 0){
        pred_one->fitness = average_fitness;
    }else{
        pred_one->fitness = (pred_one->fitness*(counter-1) + average_fitness)/counter;
    }
    if (pred_two->fitness == 0){
        pred_two->fitness = average_fitness;
    }else{
        pred_two->fitness = (pred_two->fitness*(counter-1) + average_fitness)/counter;
    }
    if (pred_three->fitness == 0){
        pred_three->fitness = average_fitness;
    }else{
        pred_three->fitness = (pred_three->fitness*(counter-1) + average_fitness)/counter;
    }
	
    return true;
}


/*
 Runs a communication simulation for given generations.
 This function by itself reads/loads parameter files and starting network.
 Calls a function pred_prey_epoch_comm.
 */

Population *predprey_test_comm(int gens){
    Population *pred_one=0;
    Population *pred_two=0;
    Population *pred_three=0;
    
    Genome *pred_one_start_genome;
    Genome *pred_two_start_genome;
    Genome *pred_three_start_genome;
    
    int gen;
    
    // Starter genome file names
    char *pred_one_starter="pred1startgenescomm";
    char *pred_two_starter="pred2startgenescomm";
    char *pred_three_starter="pred3startgenescomm";
    
    // Pulling in starter genome files
    ifstream pred_one_file(pred_one_starter,ios::in);
    ifstream pred_two_file(pred_two_starter,ios::in);
    ifstream pred_three_file(pred_three_starter,ios::in);
    
    
    cout<<"Reading in the start genome"<<endl;
    char header_one[20];
    char header_two[20];
    char header_three[20];
    
    int id_one;
    int id_two;
    int id_three;
    
    //Read in the start genomes
    pred_one_file>>header_one;
    pred_one_file>>id_one;
    pred_two_file>>header_two;
    pred_two_file>>id_two;
    pred_three_file>>header_three;
    pred_three_file>>id_three;
    
    //Build the start genomes
    pred_one_start_genome = new Genome(id_one,pred_one_file);
    pred_two_start_genome = new Genome(id_two,pred_two_file);
    pred_three_start_genome = new Genome(id_three,pred_three_file);
    pred_one_file.close();
    pred_two_file.close();
    pred_three_file.close();
    
    //Run the specified number of runs
    cout<<"Start Genomes"<<endl;
    int run;
    for (run=0;run<NEAT::num_runs;run++) {
        
        cout<<"RUN #"<<run<<endl;
        
        //Build the Populations from starter genes
        cout<<"Spawning Populations off starter genome"<<endl;
        pred_one = new Population(pred_one_start_genome,NEAT::pop_size);
        pred_two = new Population(pred_two_start_genome,NEAT::pop_size);
        pred_three = new Population(pred_three_start_genome,NEAT::pop_size);
        
        //Run for the specified number of generations
        for (gen=1;gen<=gens;gen++) {
            cout<<"Epoch "<<gen<<endl;
            predprey_epoch_comm(pred_one, pred_two, pred_three, gen);
        }
    }
    delete pred_one_start_genome;
    delete pred_two_start_genome;
    delete pred_three_start_genome;
    delete pred_two;
    delete pred_three;
    return pred_one;
}


/*
 This function runs a simulation for 1 generation.
 Each individual in each population is tested 10 times with randomly chosen partners.
 When the simulation for the generation is complete, this function chooses the best individual from each population and
 runs benchmark tests on them.
 At the end, this function writes out the result of the benchmark test.
 */

int predprey_epoch_comm(Population *pred_one, Population *pred_two, Population *pred_three, int generation){
    
    //Evaluate each organism on a test
    vector<Organism*> pred_one_copy; //= pred_one->organisms;
    vector<Organism*> pred_two_copy; //= pred_two->organisms;
    vector<Organism*> pred_three_copy; //= pred_three->organisms;
    
    int i;
    double bestfitness_one = -20;
    double bestfitness_two = -20;
    double bestfitness_three = -20;
    Organism *best_pred_one = *(pred_one->organisms).begin();
    Organism *best_pred_two = *(pred_two->organisms).begin();
    Organism *best_pred_three = *(pred_three->organisms).begin();
    
	
    /*Test each individual for 10 times */
    for (i=0;i<10;i++) {
        
        //duplicate populations
        pred_one_copy = pred_one->organisms;
        pred_two_copy = pred_two->organisms;
        pred_three_copy = pred_three->organisms;
        
        int j;
        for(j=0;j<pop_size;j++){
            
            // Pull out random predators
            int random_choice1 = rand() % pred_one_copy.size();
            Organism* pred_one_curorg = pred_one_copy.at(random_choice1);
            int random_choice2 = rand() % pred_two_copy.size();
            Organism* pred_two_curorg = pred_two_copy.at(random_choice2);
            int random_choice3 = rand() % pred_three_copy.size();
            Organism* pred_three_curorg = pred_three_copy.at(random_choice3);
            
            //Test them
            predprey_evaluate_comm(pred_one_curorg, pred_two_curorg, pred_three_curorg, i+1);
            cout<<"Test "<<(i*pop_size)+(j+1)<<"; Generation "<<generation<<endl;
            
            //Erase the ones that are already tested.
            pred_one_copy.erase(pred_one_copy.begin()+random_choice1);
            pred_two_copy.erase(pred_two_copy.begin()+random_choice2);
            pred_three_copy.erase(pred_three_copy.begin()+random_choice3);
        }
    }
    
    
    //Find which predators performed the best
    vector<Organism*>::iterator iter;
    for(iter=(pred_one->organisms).begin();iter!=(pred_one->organisms).end();++iter) {
        if ((*iter)->fitness > bestfitness_one) {
            bestfitness_one = (*iter)->fitness;
            best_pred_one = (*iter);
        }
    }
    for(iter=(pred_two->organisms).begin();iter!=(pred_two->organisms).end();++iter) {
        if ((*iter)->fitness > bestfitness_two) {
            bestfitness_two = (*iter)->fitness;
            best_pred_two = (*iter);
        }
    }
    for(iter=(pred_three->organisms).begin();iter!=(pred_three->organisms).end();++iter) {
        if ((*iter)->fitness > bestfitness_three) {
            bestfitness_three = (*iter)->fitness;
            best_pred_three = (*iter);
        }
    }
    
    //Run the best set on the benchmark
    cout<<"Running benchmark for gen "<<generation<<endl;
    
    int counter = 0;
    
    for (i=0;i<9;i++){
        //run benchmark tests for 9 times with different prey starting location.
        int caught = benchmark(i, 0, best_pred_one->net, best_pred_two->net, best_pred_three->net);
        counter += caught;
    }
	
    //Record the results into a generation file.
    char filedist2[100];
    int TempNumOne=filedist.size();
    for (int a=0;a<=TempNumOne;a++){
        filedist2[a]=filedist[a];
    }
	
    char outstring[100];
    sprintf (outstring, filedist2, generation);
    cout << outstring << endl;
    std::ofstream oFile(outstring);
    
    oFile<<"Best_Fitness_1: "<<best_pred_one->fitness<<endl;
    oFile<<"Best_Fitness_2: "<<best_pred_two->fitness<<endl;
    oFile<<"Best_Fitness_3: "<<best_pred_three->fitness<<endl;
    oFile<<"Completed_Benchmarks: "<<counter<<endl;
    oFile<<"Prey_Type: "<<7-prey_num<<endl;
    best_pred_one->write_to_file(oFile);
    best_pred_two->write_to_file(oFile);
    best_pred_three->write_to_file(oFile);
    oFile.close();
    
    //If a group solves more than 7 banchmark test, increment the prey difficulty.
    if (counter >= 7 && prey_num > 1) {
        prey_num -= 1;
        cout << "************Prey is now moving faster!!!!************" << endl;
    }
    
    //Create the next generation
    pred_one->epoch(generation);
    pred_two->epoch(generation);
    pred_three->epoch(generation);
    return 1;
}

//-------------------
// New version of predprey_evaluate_comm.
// Average values are properly calculated in epoch_nocomm function by using this function.
// Evaluates a group of 3 predators 6 times and records the average fitness value.
//-------------------
bool predprey_evaluate_comm(Organism *pred_one, Organism *pred_two, Organism *pred_three, int counter){
	
    //Try to catch the prey (run six times)
    double fitnesses_list[6];
    double newfitness;
	
    int i;
    for (i=0;i<6;i++){
        newfitness = hunt(pred_one->net, pred_two->net, pred_three->net); //LET THE HUNT BEGIN!!!!!
        fitnesses_list[i] = newfitness;
    }
    //Average fitness over six runs
    double fit_sum = 0.0;
	
    for (i=0;i<6;i++){
        fit_sum += fitnesses_list[i];
    }
    double average_fitness = fit_sum/6;
	
    //Assign the average fitness value to organisms, averaging them again with previously recorded value.
    //The number of runs previously conducted is kept track by the counter variable.
    if (pred_one->fitness == 0){
        pred_one->fitness = average_fitness;
    }else{
        pred_one->fitness = (pred_one->fitness*(counter-1) + average_fitness)/counter;
    }
    if (pred_two->fitness == 0){
        pred_two->fitness = average_fitness;
    }else{
        pred_two->fitness = (pred_two->fitness*(counter-1) + average_fitness)/counter;
    }
    if (pred_three->fitness == 0){
        pred_three->fitness = average_fitness;
    }else{
        pred_three->fitness = (pred_three->fitness*(counter-1) + average_fitness)/counter;
    }
	
    return true;
}


/*
 Runs a simulation with a group of centrally controlled predators.
 This function by itself reads/loads parameter files and starting genomes.
 The actual simulation is done by epoch_central function called later in this function.
 */

Population *predprey_test_central(int gens){
    Population *pred=0;
    
    Genome *pred_start_genome;
    
    int gen;
    
    // Starter genome file names
    char *pred_starter="predstartgenescentral";
    
    // Pulling in starter genome files
    ifstream pred_file(pred_starter,ios::in);
    
    
    cout<<"Reading in the start genome"<<endl;
    char header[20];
    int id;
    
    //Read in the start genomes
    pred_file>>header;
    pred_file>>id;
    
    //Build the start genomes
    pred_start_genome = new Genome(id,pred_file);
    pred_file.close();
    
    //Run the specified number of runs
    cout<<"Start Genomes"<<endl;
    int run;
    for (run=0;run<NEAT::num_runs;run++) {
        
        cout<<"RUN #"<<run<<endl;
        
        //Build the Populations from starter genes
        cout<<"Spawning Populations off starter genome"<<endl;
        pred = new Population(pred_start_genome,NEAT::pop_size);
        
        //Run for the specified number of generations
        for (gen=1;gen<=gens;gen++) {
            cout<<"Epoch "<<gen<<endl;
            predprey_epoch_central(pred, gen);
        }
    }
    delete pred_start_genome;
    
    return pred;
}

/*
 This function runs a simulation for 1 generation.
 Each set of predators is tested 10 times.
 When the simulation for the generation is complete, this function chooses the best individual from each population and
 runs benchmark tests on them.
 At the end, this function writes out the result of the benchmark test.
 */

int predprey_epoch_central(Population *pred, int generation){
    //Evaluate each organism on a test
    vector<Organism*>::iterator curorg;
    int i;
    int j;
    for (j=0;j<10;j++){
        i = 0;
        for(curorg=(pred->organisms).begin();curorg!=(pred->organisms).end();++curorg){
            //Test them
            predprey_evaluate_central(*curorg, i+1);
            //cout<<"Test "<<(j*pop_size)+(i+1)<<"; Generation "<<generation<<endl;
            i++;
        }
    }
    
    //Find the best predators from each population and run them in the benchmark
    cout<<"Running benchmark for gen "<<generation<<endl;
    vector<Organism*>::iterator bestfinder;
    
    Organism *best_pred = *(pred->organisms).begin();
    for(bestfinder=(pred->organisms).begin();bestfinder!=(pred->organisms).end();++bestfinder) {
        if (best_pred->fitness<(*bestfinder)->fitness) best_pred = *bestfinder;
    }
    
    //Run the best set on the benchmark
    cout<<"Running benchmark for gen "<<generation<<endl;
    
    int counter = 0;
    
    for (i=0;i<9;i++){
        //Run benchmark tests 9 time, starting at different locations each time.
        int caught = benchmark(i, 0, best_pred->net);
        counter += caught;
    }
    
    //Writting out the result to a generatio file at a specified location.
    char outstring[50];
    sprintf (outstring, "/Users/hiranumn/test/generation_%i", generation);
    std::ofstream oFile(outstring);
    oFile<<"Best_Fitness: "<<best_pred->fitness<<endl;
    oFile<<endl;
    oFile<<endl;
    oFile<<"Completed_Benchmarks: "<<counter<<endl;
    oFile<<"Prey_Type: "<<7-prey_num<<endl;
    best_pred->write_to_file(oFile);
    
    if (counter >= 7 && prey_num > 1) {
        prey_num -= 1;
        cout << "************Prey is now moving faster!!!!************" << endl;
    }
    
    //Create the next generation
    pred->epoch(generation);
    
    return 1;
}

//-------------------
// New version of predprey_evaluate_central.
// Average values are properly calculated in epoch_central function by using this function.
// Evaluates a group of 3 predators 6 times and records the average fitness value.
//-------------------
bool predprey_evaluate_central(Organism *pred, int counter){
	
    //Get networks for predators
    Network *net;
    net = pred->net;
	
    //Try to catch the prey (run six times)
    double fitnesses[6];
    double newfitness;
	
    int i;
    for (i=0;i<6;i++){
        newfitness = hunt(pred->net); //LET THE HUNT BEGIN!!!!!
        fitnesses[i] = newfitness;
    }
	
    //Average fitness over six runs
    double fit_sum = 0.0;
	
    for (i=0;i<6;i++){
        fit_sum += fitnesses[i];
    }
	
    double average_fitness = fit_sum/6;
	
    //Assign the average fitness values to a group of predators, averaging it with the previously determined fitness values.
    //The number of runs already conducted is kept track by the counter variable.
    if (pred->fitness == 0) {
        pred->fitness = average_fitness;
    }else{
        pred->fitness = (pred->fitness*(counter-1) + average_fitness)/counter;
    }
    return true;
}

/*
 This function conducts a single simulation of 3 predators catching 1 prey.
 */

double hunt(Network *pred_one, Network *pred_two, Network *pred_three, int *count) {
    World* world;
    world = new World();
    
    double output1[2];
    double output2[2];
    double output3[2];
    
    //l keeps track of the number of steps.
    int l = 0;
    
    //Simulation runs for 475 steps.
    while (l < maxSteps) {
        //Loading NEAT with inputs.
        //For centrally controlled predators. Since this option is not used currently i havent changed its network structure for the limited sight experiments.
        if (pred_two == NULL && pred_three == NULL) {
            double input[19];
            input[0] = world->getXOffset(world->models[1],world->models[0])/input_divide;
            input[1] = world->getYOffset(world->models[1],world->models[0])/input_divide;
            input[2] = world->getXOffset(world->models[1],world->models[2])/input_divide;
            input[3] = world->getYOffset(world->models[1],world->models[2])/input_divide;
            input[4] = world->getXOffset(world->models[1],world->models[3])/input_divide;
            input[5] = world->getYOffset(world->models[1],world->models[3])/input_divide;
            input[6] = world->getXOffset(world->models[2],world->models[0])/input_divide;
            input[7] = world->getYOffset(world->models[2],world->models[0])/input_divide;
            input[8] = world->getXOffset(world->models[2],world->models[1])/input_divide;
            input[9] = world->getYOffset(world->models[2],world->models[1])/input_divide;
            input[10] = world->getXOffset(world->models[2],world->models[3])/input_divide;
            input[11] = world->getYOffset(world->models[2],world->models[3])/input_divide;
            input[12] = world->getXOffset(world->models[3],world->models[0])/input_divide;
            input[13] = world->getYOffset(world->models[3],world->models[0])/input_divide;
            input[14] = world->getXOffset(world->models[3],world->models[1])/input_divide;
            input[15] = world->getYOffset(world->models[3],world->models[1])/input_divide;
            input[16] = world->getXOffset(world->models[3],world->models[2])/input_divide;
            input[17] = world->getYOffset(world->models[3],world->models[2])/input_divide;
            input[18] = 0.5;
            pred_one->load_sensors(input);
            pred_one->activate();
            vector<NNode*>:: iterator curout;
            int i = 0;
            //Receiving outputs.
            for(curout = pred_one->outputs.begin(); curout != pred_one->outputs.end(); curout++){
                if (i<2){
                    output1[i]=(*(curout))->activation;
                } else if (i < 4) {
                    output2[i-2]=(*(curout))->activation;
                } else {
                    output3[i-4]=(*(curout))->activation;
                }
                i++;
            }
        }
        
        else if(pred_one->inputs.size() == 7) {
            int randinput = 0;
            //Loading a network with pred1 information.
            double input1[7];
            
			//prey
			input1[0] = world->getXOffset(world->models[1],world->models[0])/input_divide;
			input1[1] = world->getYOffset(world->models[1],world->models[0])/input_divide;
            
            //pred1
            input1[2] = world->getXOffset(world->models[1],world->models[2])/input_divide;
            input1[3] = world->getYOffset(world->models[1],world->models[2])/input_divide;
            
            //pred2
            input1[4] = world->getXOffset(world->models[1],world->models[3])/input_divide;
            input1[5] = world->getYOffset(world->models[1],world->models[3])/input_divide;
            
            //bias
            input1[6] = .5;
            
            pred_one->load_sensors(input1);
            pred_one->activate();
            vector<NNode*>:: iterator curout1;
            int i = 0;
            //Receiving outputs.
            for(curout1 = pred_one->outputs.begin(); curout1!=pred_one->outputs.end();curout1++){
                output1[i]=(*(curout1))->activation;
                i++;
            }
            
            //Loading another network with pred2 information.
            double input2[7];
            
            //if the prey is in the vision range
            input2[0] = world->getXOffset(world->models[2],world->models[0])/input_divide;
            input2[1] = world->getYOffset(world->models[2],world->models[0])/input_divide;
            
            //pred1
            input2[2] = world->getXOffset(world->models[2],world->models[1])/input_divide;
            input2[3] = world->getYOffset(world->models[2],world->models[1])/input_divide;
            
            //pred2
            input2[4] = world->getXOffset(world->models[2],world->models[3])/input_divide;
            input2[5] = world->getYOffset(world->models[2],world->models[3])/input_divide;
            
            //bias
            input2[6] = .5;
            
            pred_two->load_sensors(input2);
            pred_two->activate();
            vector<NNode*>:: iterator curout2;
            i = 0;
            //Receiving outputs.
            for(curout2 = pred_two->outputs.begin(); curout2!=pred_two->outputs.end();curout2++){
                output2[i]=(*(curout2))->activation;
                i++;
            }
            
            //Loading a network with pred3 information.
            double input3[7];
            
            //if the prey is in the vision range
            input3[0] = world->getXOffset(world->models[3],world->models[0])/input_divide;
            input3[1] = world->getYOffset(world->models[3],world->models[0])/input_divide;
            
            //pred1
            input3[2] = world->getXOffset(world->models[3],world->models[1])/input_divide;
            input3[3] = world->getYOffset(world->models[3],world->models[1])/input_divide;
            
            //pred2
            input3[4] = world->getXOffset(world->models[3],world->models[2])/input_divide;
            input3[5] = world->getYOffset(world->models[3],world->models[2])/input_divide;
            
            //bias
            input3[6] = .5;
            
            pred_three->load_sensors(input3);
            pred_three->activate();
            vector<NNode*>:: iterator curout3;
            i = 0;
            //Receiving outputs.
            for(curout3 = pred_three->outputs.begin(); curout3!=pred_three->outputs.end();curout3++){
                output3[i]=(*(curout3))->activation;
                i++;
            }
        }
        //For non-communicating predators.
        else if (pred_one->inputs.size() == 3) {
            int randinput = 0;
            //Loading a network with pred1 information.
            double input1[3];
            
            //if the prey is in the vision range
            //if(world->getDistBtw(world->models[1],world->models[0]) < world->predatorVisionRange){

            input1[0] = world->getXOffset(world->models[1],world->models[0])/input_divide;
            input1[1] = world->getYOffset(world->models[1],world->models[0])/input_divide;
            input1[2] = .5;
            
            
            ///WHAT IS THIS DOING. TEST WITHOUT THIS LATER.
            for (int foo = 0; foo < 3; foo++) { ///////GET THIS OUT (and other two)!
                //cout << input1[foo] << endl;
                if (input1[foo] != input1[foo]) exit(1);
            }
            pred_one->load_sensors(input1);
            pred_one->activate();
            
            //Receiving outputs
            vector<NNode*>:: iterator curout1;
            int i = 0;
            for(curout1 = pred_one->outputs.begin(); curout1!=pred_one->outputs.end();curout1++){
                output1[i]=(*(curout1))->activation;
                //cout << output1[i] << endl;
                //if (output1[i] != output1[i]) exit(8);
                i++;
            }
            
            
            //Loading a network with pred2 information
            double input2[3];
            
            //if the prey is in the vision range
            input2[0] = world->getXOffset(world->models[2],world->models[0])/input_divide;
            input2[1] = world->getYOffset(world->models[2],world->models[0])/input_divide;
            input2[2] = .5;
            
            for (int foo = 0; foo < 3; foo++) {
                //cout << input2[foo] << endl;
                if (input2[foo] != input2[foo]) exit(1); ///
            }
            pred_two->load_sensors(input2);
            pred_two->activate();
            
            //Receiving outputs
            vector<NNode*>:: iterator curout2;
            i = 0;
            for(curout2 = pred_two->outputs.begin(); curout2!=pred_two->outputs.end();curout2++){
                output2[i]=(*(curout2))->activation;
                //cout << output2[i] << endl;
                //if (output2[i] != output2[i]) exit(8);
                i++;
            }
			
            //Loading a network with pred3 information
            double input3[3];
            //if the prey is in the vision range
            input3[0] = world->getXOffset(world->models[3],world->models[0])/input_divide;
            input3[1] = world->getYOffset(world->models[3],world->models[0])/input_divide;
            input3[2] = .5;
            
            for (int foo = 0; foo < 3; foo++) {
                //cout << input3[foo] << endl;
                if (input3[foo] != input3[foo]) exit(1); ///
            }
            pred_three->load_sensors(input3);
            pred_three->activate();
			
            //Reveiving inputs.
            vector<NNode*>:: iterator curout3;
            i = 0;
            for(curout3 = pred_three->outputs.begin(); curout3!=pred_three->outputs.end();curout3++){
                output3[i]=(*(curout3))->activation;
                //cout << output3[i] << endl;
                //if (output3[i] != output3[i]) exit(8);
                i++;
            }
        }
        
		vector<double> vec1;
		vector<double> vec2;
		vector<double> vec3;

		vec1.push_back(output1[0]);
		vec1.push_back(output1[1]);
		vec2.push_back(output2[0]);
		vec2.push_back(output2[1]);
		vec3.push_back(output3[0]);
		vec3.push_back(output3[1]);

        //Setting speed and angle for each predator.
		double angle_one = convertVectorToAngle(vec1);
        double angle_two = convertVectorToAngle(vec2);
        double angle_three = convertVectorToAngle(vec3);
        
        world->models[1]->setAngle(angle_one);
        world->models[2]->setAngle(angle_two);
        world->models[3]->setAngle(angle_three);
        
		//Luke et al gives a constant speed to the models, which is a 1 unit size. 
        
        //Use the correct step() function for the prey type
		world->step();
		
		
        //Check if the prey is caught
        if(world->checkCaught()){
            //cout << "Caught!!!" << endl;
            double fitness = world->getPredFitness(true);
            if(count!=NULL){
                //cout << "hello" << endl;
                *count = *count + 1;
            }
            //cout << fitness << endl;
            for (int k = 0; k < 4; k++){
                delete world->models[k];
            }
            delete world;
            return fitness;
        }
        l++;
    }
    double fitness = world->getPredFitness(false);
    //cout << fitness << endl;
    for (int k = 0; k < 4; k++){
        delete world->models[k];
    }
    delete world;
    return fitness;
}

/*
 This function conducts a single benchmark test on a group of given predators.
 Starting location of the prey can be specified by the counter variable.
 The user can also choose to visualize the process or not.
 */

int benchmark(int counter, int viz, Network *pred_one, Network *pred_two, Network *pred_three) {
    World* world;
    world = new World();
    //double defSpeed = .3;
    
#ifndef __linux__
    sf::RenderWindow* App;
    sf::CircleShape Prey;
    sf::CircleShape Predator1;
    sf::CircleShape Predator2;
    sf::CircleShape Predator3;
#endif
    
#ifndef __linux__
    // If using a visualizer, instantiate windows and predator tokens.
    if (viz == 1){
		App = new sf::RenderWindow(sf::VideoMode(5 * world->width, 5 * world->height, 32), "SFML Graphics");
        Prey = sf::CircleShape(10);
		Prey.setFillColor(sf::Color(51, 102, 153));
        
		Predator1 = sf::CircleShape(10);
		Predator1.setFillColor(sf::Color(255, 0, 0));
        
		Predator2 = sf::CircleShape(10);
		Predator2.setFillColor(sf::Color(255, 102, 0));
        
		Predator3 = sf::CircleShape(10);
		Predator3.setFillColor(sf::Color(204, 0, 102));
        //sexy pink
    }
#endif
    
    //Set Prey starting position for current benchmark test
	world->models[0]->setPositionX(world->width / 6 + (world->width / 3) * (counter / 3));
	world->models[0]->setPositionY(world->height / 6 + (world->width / 3) * (counter % 3));
    
    //world->models[0]->setSpeed(defSpeed);
    
    double output1[2];
    double output2[2];
    double output3[2];
    
    //This variable keeps track of the number of steps
    int l = 0;
    
    //Simulation runs for 475 steps.
    while (l < maxSteps) {
#ifndef __linux__
        if (viz == 1){
            Sleep(300);
        }
#endif
        
        //For centrally controlled predators.
        if (pred_one->inputs.size() == 19) {
            double input[19];
            //load a network with inputs.
            input[0] = world->getXOffset(world->models[1],world->models[0])/input_divide;
            input[1] = world->getYOffset(world->models[1],world->models[0])/input_divide;
            input[2] = world->getXOffset(world->models[1],world->models[2])/input_divide;
            input[3] = world->getYOffset(world->models[1],world->models[2])/input_divide;
            input[4] = world->getXOffset(world->models[1],world->models[3])/input_divide;
            input[5] = world->getYOffset(world->models[1],world->models[3])/input_divide;
            input[6] = world->getXOffset(world->models[2],world->models[0])/input_divide;
            input[7] = world->getYOffset(world->models[2],world->models[0])/input_divide;
            input[8] = world->getXOffset(world->models[2],world->models[1])/input_divide;
            input[9] = world->getYOffset(world->models[2],world->models[1])/input_divide;
            input[10] = world->getXOffset(world->models[2],world->models[3])/input_divide;
            input[11] = world->getYOffset(world->models[2],world->models[3])/input_divide;
            input[12] = world->getXOffset(world->models[3],world->models[0])/input_divide;
            input[13] = world->getYOffset(world->models[3],world->models[0])/input_divide;
            input[14] = world->getXOffset(world->models[3],world->models[1])/input_divide;
            input[15] = world->getYOffset(world->models[3],world->models[1])/input_divide;
            input[16] = world->getXOffset(world->models[3],world->models[2])/input_divide;
            input[17] = world->getYOffset(world->models[3],world->models[2])/input_divide;
            input[18] = 0.5;
            pred_one->load_sensors(input);
            pred_one->activate();
            vector<NNode*>:: iterator curout;
            int i = 0;
            //Receive outputs
            for(curout = pred_one->outputs.begin(); curout != pred_one->outputs.end(); curout++){
                if (i<2){
                    output1[i]=(*(curout))->activation;
                } else if (i < 4) {
                    output2[i-2]=(*(curout))->activation;
                } else {
                    output3[i-4]=(*(curout))->activation;
                }
                i++;
            }
        }
        //For communicating predators.
        else if(pred_one->inputs.size() == 7) {
            int randinput = 0;
            //Loading a network with pred1 information.
            double input1[7];
            
            //if the prey is in the vision range
            //if(world->getDistBtw(world->models[1],world->models[0]) < world->predatorVisionRange){
            input1[0] = world->getXOffset(world->models[1],world->models[0])/input_divide;
            input1[1] = world->getYOffset(world->models[1],world->models[0])/input_divide;
            
            //pred1
            input1[2] = world->getXOffset(world->models[1],world->models[2])/input_divide;
            input1[3] = world->getYOffset(world->models[1],world->models[2])/input_divide;
            
            //pred2
            input1[4] = world->getXOffset(world->models[1],world->models[3])/input_divide;
            input1[5] = world->getYOffset(world->models[1],world->models[3])/input_divide;
            
            //bias
            input1[6] = .5;
            
            pred_one->load_sensors(input1);
            pred_one->activate();
            vector<NNode*>:: iterator curout1;
            int i = 0;
            //Receiving outputs.
            for(curout1 = pred_one->outputs.begin(); curout1!=pred_one->outputs.end();curout1++){
                output1[i]=(*(curout1))->activation;
                i++;
            }
            
            //Loading another network with pred2 information.
            double input2[7];
            
            //if the prey is in the vision range
            input2[0] = world->getXOffset(world->models[2],world->models[0])/input_divide;
            input2[1] = world->getYOffset(world->models[2],world->models[0])/input_divide;

            //pred1
            input2[2] = world->getXOffset(world->models[2],world->models[1])/input_divide;
            input2[3] = world->getYOffset(world->models[2],world->models[1])/input_divide;
            
            //pred2
            input2[4] = world->getXOffset(world->models[2],world->models[3])/input_divide;
            input2[5] = world->getYOffset(world->models[2],world->models[3])/input_divide;
            
            //bias
            input2[6] = .5;
            
            pred_two->load_sensors(input2);
            pred_two->activate();
            vector<NNode*>:: iterator curout2;
            i = 0;
            //Receiving outputs.
            for(curout2 = pred_two->outputs.begin(); curout2!=pred_two->outputs.end();curout2++){
                output2[i]=(*(curout2))->activation;
                i++;
            }
            
            //Loading a network with pred3 information.
            double input3[7];
            
            //if the prey is in the vision range
            input3[0] = world->getXOffset(world->models[3],world->models[0])/input_divide;
            input3[1] = world->getYOffset(world->models[3],world->models[0])/input_divide;

            //pred1
            input3[2] = world->getXOffset(world->models[3],world->models[1])/input_divide;
            input3[3] = world->getYOffset(world->models[3],world->models[1])/input_divide;
            
            //pred2
            input3[4] = world->getXOffset(world->models[3],world->models[2])/input_divide;
            input3[5] = world->getYOffset(world->models[3],world->models[2])/input_divide;
            
            //bias
            input3[6] = .5;
            
            pred_three->load_sensors(input3);
            pred_three->activate();
            vector<NNode*>:: iterator curout3;
            i = 0;
            //Receiving outputs.
            for(curout3 = pred_three->outputs.begin(); curout3!=pred_three->outputs.end();curout3++){
                output3[i]=(*(curout3))->activation;
                i++;
            }
        }
        else if (pred_one->inputs.size() == 3) {
            int randinput = 0;
            //Loading a network with pred1 information.
            double input1[3];

            input1[0] = world->getXOffset(world->models[1],world->models[0])/input_divide;
            input1[1] = world->getYOffset(world->models[1],world->models[0])/input_divide;
            input1[2] = .5;
            
            for (int foo = 0; foo < 3; foo++) { 
                if (input1[foo] != input1[foo]) exit(1);
            }
            pred_one->load_sensors(input1);
            pred_one->activate();
            
            //Receiving outputs
            vector<NNode*>:: iterator curout1;
            int i = 0;
            for(curout1 = pred_one->outputs.begin(); curout1!=pred_one->outputs.end();curout1++){
                output1[i]=(*(curout1))->activation;
                //cout << output1[i] << endl;
                //if (output1[i] != output1[i]) exit(8);
                i++;
            }
            //Loading a network with pred2 information
            double input2[3];
            
            //if the prey is in the vision range
            input2[0] = world->getXOffset(world->models[2],world->models[0])/input_divide;
            input2[1] = world->getYOffset(world->models[2],world->models[0])/input_divide;
			input2[2] = .5;
            
            for (int foo = 0; foo < 3; foo++) {
                //cout << input2[foo] << endl;
                if (input2[foo] != input2[foo]) exit(1); ///
            }
            pred_two->load_sensors(input2);
            pred_two->activate();
            
            //Receiving outputs
            vector<NNode*>:: iterator curout2;
            i = 0;
            for(curout2 = pred_two->outputs.begin(); curout2!=pred_two->outputs.end();curout2++){
                output2[i]=(*(curout2))->activation;
                //cout << output2[i] << endl;
                //if (output2[i] != output2[i]) exit(8);
                i++;
            }
            
            //Loading a network with pred3 information
            double input3[3];
            //if the prey is in the vision range
            input3[0] = world->getXOffset(world->models[3],world->models[0])/input_divide;
            input3[1] = world->getYOffset(world->models[3],world->models[0])/input_divide;
            input3[2] = .5;
            
            for (int foo = 0; foo < 3; foo++) {
                //cout << input3[foo] << endl;
                if (input3[foo] != input3[foo]) exit(1); ///
            }
            pred_three->load_sensors(input3);
            pred_three->activate();
            
            //Reveiving inputs.
            vector<NNode*>:: iterator curout3;
            i = 0;
            for(curout3 = pred_three->outputs.begin(); curout3!=pred_three->outputs.end();curout3++){
                output3[i]=(*(curout3))->activation;
                //cout << output3[i] << endl;
                //if (output3[i] != output3[i]) exit(8);
                i++;
            }
        }
        
		vector<double> vec1;
		vector<double> vec2;
		vector<double> vec3;

		vec1.push_back(output1[0]);
		vec1.push_back(output1[1]);
		vec2.push_back(output2[0]);
		vec2.push_back(output2[1]);
		vec3.push_back(output3[0]);
		vec3.push_back(output3[1]);

		//Setting speed and angle for each predator.
		double angle_one = convertVectorToAngle(vec1);
		double angle_two = convertVectorToAngle(vec2);
		double angle_three = convertVectorToAngle(vec3);
        
        world->models[1]->setAngle(angle_one);
        world->models[2]->setAngle(angle_two);
        world->models[3]->setAngle(angle_three);
        
        //world->models[1]->setSpeed(defSpeed*output1[4]);
        //world->models[2]->setSpeed(defSpeed*output2[4]);
        //world->models[3]->setSpeed(defSpeed*output3[4]);
        
        //Use the correct step() function for the prey type
		world->step();
        
#ifndef __linux__
        //If you are using a visualizer, move robots to the proper positions.
        if (viz == 1) {
            ///Press p to pause
            
			Prey.setPosition(5 * world->models[0]->getPositionX(), 5 * world->height - 5 * world->models[0]->getPositionY());
            
			Predator1.setPosition(5 * world->models[1]->getPositionX(), 5 * world->height - 5 * world->models[1]->getPositionY());
            
			Predator2.setPosition(5 * world->models[2]->getPositionX(), 5 * world->height - 5 * world->models[2]->getPositionY());
            
			Predator3.setPosition(5 * world->models[3]->getPositionX(), 5 * world->height - 5 * world->models[3]->getPositionY());
            
            App->clear(sf::Color(255,255,255));
            
            // Display sprite in our window
            App->draw(Prey);
            App->draw(Predator1);            
            App->draw(Predator2);           
            App->draw(Predator3);
            
            //App->Draw(line1);
            //App->Draw(line2);
            //App->Draw(line3);
            //App->Draw(line4);
            // Display window contents on screen
            App->display();
        }
#endif
        
        //Check if the prey is caught
        if(world->checkCaught()){
            cout << "Caught!!!" << endl;
            for (int k = 0; k < 4; k++){
                delete world->models[k];
            }
            delete world;
#ifndef __linux__
            if (viz == 1) delete App;
#endif
            return 1;
        }
        l++;
    }
    for (int k = 0; k < 4; k++){
        delete world->models[k];
    }
    delete world;
#ifndef __linux__
    if (viz == 1) delete App;
#endif
    return 0;
}

//Converts Output form a neural netowks to an angle.
double convertOutputToAngle(double output[]) {
    
    double N = output[0];
    double E = output[1];
    double S = output[2];
    double W = output[3];
    
    double deltaY = N-S;
    double deltaX = E-W;
    double angle = 0;
    
    if( deltaX == 0){
        if(deltaY > 0){
            angle = PI/2;
        }else if(deltaY < 0){
            angle = -PI/2;
        }
    }else{
        angle = atan(deltaY/deltaX);
        if( deltaX < 0){
            angle = angle + PI;
        }
    }
    return angle; 
}

//This function can replay the movement of the best predators in a given generation file.
//Calls the benchmark function above with viz parameter 1.
int replay(char* filename){
    Genome* pred_one_genome = NULL;
    Genome* pred_two_genome = NULL;
    Genome* pred_three_genome = NULL;
    
    // Pulling in starter genome files
    ifstream file(filename,ios::in);
    
    char header_one[20];
    char header_two[20];
    char header_three[20];
    
    int id_one;
    int id_two;
    int id_three;
    
    int counter = 0;
    int i;
    
    //Read in the start genomes
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),' ');
    int temp;
    file>>temp;
    prey_num = 7-temp;
    
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file>>header_one;
    file>>id_one;
    pred_one_genome = new Genome(id_one,file);
    
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    
    if (file.good()) {
        file>>header_two;
        file>>id_two;
        pred_two_genome = new Genome(id_two,file);
        
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        file>>header_three;
        file>>id_three;
        pred_three_genome = new Genome(id_three,file);
        
        
        //Run a benchmark test 9 times with 9 different prey starting locations. Visualization turned ON.
        for (i=0;i<9;i++){
            int caught = benchmark(i, 1, pred_one_genome->genesis(1), pred_two_genome->genesis(2), pred_three_genome->genesis(3));
            counter += caught;
        }
    }
    
    else {
        for (i=0;i<9;i++){
            int caught = benchmark(i, 1, pred_one_genome->genesis(1));
            counter += caught;
        }
    }
    
    file.close();
    
    return counter;
}

//Runs 100 benchmark tests with random prey starting locations
//Right now it does not work with a set of centrally controlled predators.  
int superbench(char* filename){
    Genome* pred_one_genome = NULL;
    Genome* pred_two_genome = NULL;
    Genome* pred_three_genome = NULL;
	
    // Pulling in starter genome files
    ifstream file(filename,ios::in);
	
    char header_one[20];
    char header_two[20];
    char header_three[20];
	
    int id_one;
    int id_two;
    int id_three;
	
    int counter = 0;
    int i;
	
    //Read in the start genomes
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),' ');
    int temp;
    file>>temp;
    prey_num = 7-temp;
	
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file>>header_one;
    file>>id_one;
    pred_one_genome = new Genome(id_one,file);
	
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    
    file>>header_two;
    file>>id_two;
    pred_two_genome = new Genome(id_two,file);
	
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    file>>header_three;
    file>>id_three;
    pred_three_genome = new Genome(id_three,file);
    
    
    //Run a benchmark test 100 times with random prey starting locations. Visualization turned OFF.
    double sum = 0;
    int NumCaught = 0;
    int runs = 1000;
    for (i=0;i<runs;i++){
        //cout << NumCaught <<endl;
        double fitness = hunt(pred_one_genome->genesis(1), pred_two_genome->genesis(2), pred_three_genome->genesis(3), &NumCaught);
        //cout << fitness << endl;
        sum += fitness;
    }
    cout << "# of successful hunts: " << NumCaught << "/" << runs << endl;
    sum = sum/runs;
    cout << "Fitness=" << sum << endl;
    //We are assuing that the predators are never centrally controlled
	
    file.close();
	
    return 0;
}
