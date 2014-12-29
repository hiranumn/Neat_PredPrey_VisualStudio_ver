/*
 *  robotSimulator.h
 *  test
 *
 *  Created by Naozumi Hiranuma on 7/6/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "math.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#ifndef __linux__
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#endif

using namespace std;

#define PI 3.1415926

extern double worldsize;
extern int loop;
extern double robotDiameter;
extern double simulationSize;

class World;

class Model {
protected:
	double positionX;
	double positionY;
	double angle;
	double speed;
	double angularSpeed;
	int stepNum;
public:
	string name;
	Model(double x, double y, double a, double s, double as, string n);
	void step();
	void setPositionX(double x);
	void setPositionY(double y);
	void setAngle(double a);
	void setSpeed(double s);
	void setStepNum(int step);
	void setAngularSpeed(double as);
	void setName(string n);
	double getPositionX();
	double getPositionY();
	double getAngle();
	double getSpeed();
	int getStepNum();
	double getAngularSpeed();
	string getName();
};

class Prey: public Model {
public:
	Prey(double x, double y, double a, double s, double as, string n);
	void randomStep();
	void update();
};

class Predator: public Model {
public:
	Predator(double x, double y, double a, double s, double as, string n);
	void update();
};

class World {
public:
	double width;
	double height;
	double aveIniDist;
	Model* models[4];
	World();
	void step();
	double getDistBtw(Model* a, Model* b);
	Model* getClosestPred();
	Model* getSecondClosestPred();
	Model* getFarthestPred();
	double getPreyAngle1(Model* pred);
	double getPreyAngle2old(Model* pred1, Model* pred2);
	double getPreyAngle2(Model* pred1, Model* pred2);
	double getPreyAngle3(Model* pred1, Model* pred2, Model* pred3);
	double getAveDist();
	double getPredFitness(bool caught);
	bool checkCaught();
	double getXOffset(Model* a, Model* b);
	double getYOffset(Model* a, Model* b);
};


double convertVectorToAngle(vector<double> vec);
vector<double> convertAngleToVector(double angle);
double rtod(double rad);
double angleIn2PIrange(double angle);
double gaussianRand(double m);
