/*
 *  robotSimulator.cpp
 *  Created by Naozumi Hiranuma on 7/6/12.
 *  Updated on 2013/12/27
 */

#include "robotSimulator.h"
double unitsize = 10;
double gridnum = 15;
double worldsize = gridnum*unitsize;

//Model class (both preys and predators belong here) 
Model::Model(double x, double y, double a, double s, double as, string n){
  positionX = x;
  positionY = y;
  angle = a;
  speed = s;
  angularSpeed = as;
  name = n;
  stepNum = 0;
}

void Model::step(){
	//simply updates the position of a model according to its current location, angle and speed.

	positionX = positionX + cos(angle)*speed;
	positionY = positionY + sin(angle)*speed;

	//Since the world is assumed to be toroidal, when a model hits the wall, it warps to the other side of the map. 
	if (positionX < 0){ positionX += worldsize; }
	else if (positionX >= worldsize){ positionX -= worldsize; }
	if (positionY < 0){ positionY += worldsize; }
	else if (positionY >= worldsize){ positionY -= worldsize; }
	
	//incremeting the number of steps calculated.
	stepNum++;
}

//Simple get/set functions 
void Model::setPositionX(double x){positionX = x;}
void Model::setPositionY(double y){positionY = y;}
void Model::setAngle(double a){angle = a;}
void Model::setSpeed(double s){speed = s;}
void Model::setStepNum(int step){stepNum = step;}
void Model::setAngularSpeed(double as){angularSpeed = as;}
void Model::setName(string n){name = n;}
double Model::getPositionX(){return positionX;}
double Model::getPositionY(){return positionY;}
double Model::getAngle(){return angle;}
double Model::getSpeed(){return speed;}
int Model::getStepNum(){return stepNum;}
double Model::getAngularSpeed(){return angularSpeed;}
string Model::getName(){return name;}

//Prey
//Inherits the model class.
//Not really used lol. 
Prey::Prey(double x, double y, double a, double s, double as, string n) : Model(x,y,a,s,as,n){}
void Prey::update(){
}
void Prey::randomStep(){
}

//Predator
//Inherits the model class.
//Not really used lol. 
Predator::Predator(double x, double y, double a, double s, double as, string n) : Model(x,y,a,s,as,n){}
void Predator::update(){
}

//World
World::World(){
  width = worldsize;
  height = worldsize;
	
  //loading models (all of them starts at a random location)
  //The prey moves 3 unitsize per step.
  Prey* prey = new Prey((rand() % (int)gridnum)*unitsize, (rand() % (int)gridnum)*unitsize, 0, 3 * unitsize, 0, "prey");
  //The predators only move 1 unit per step. 
  Predator* pred1 = new Predator((rand() % (int)gridnum)*unitsize, (rand() % (int)gridnum)*unitsize, 0, unitsize, 0, "pred1");
  Predator* pred2 = new Predator((rand() % (int)gridnum)*unitsize, (rand() % (int)gridnum)*unitsize, 0, unitsize, 0, "pred2");
  Predator* pred3 = new Predator((rand() % (int)gridnum)*unitsize, (rand() % (int)gridnum)*unitsize, 0, unitsize, 0, "pred3");
	
  models[0] = prey;
  models[1] = pred1;
  models[2] = pred2;
  models[3] = pred3;
	
	//Calculate average initial distance.
  aveIniDist = getAveDist();
}

double World::getAveDist(){
  double sum = 0;
  for(int i=1; i<4; i++){
    sum += getDistBtw(models[0],models[i]);
  }
  double aveDist = sum/3;
  return aveDist;
}

double World::getPredFitness(bool caught){
	double fitness = 0;
	if(caught){
		fitness = (200-getDistBtw(models[0],getClosestPred()))/10;
	}else{
		fitness = (aveIniDist - getDistBtw(models[0],getClosestPred()))/10;
		}
	return fitness;
} 

void World::step(){
	//updating a prey
	//determining the new angle of a prey.
	
	double preyAngle = getPreyAngle3(getClosestPred(),getSecondClosestPred(),getFarthestPred());
	
	models[0]->setAngle(preyAngle);
	models[0]->step();
	
	//updating predators
	for(int i=1; i<4; i++){
		models[i]->step();
	}
}

double World::getDistBtw(Model*a, Model* b){
  //calculates a distance between two models using pointers to the models. 
  double dist = 0;
	
  double xdiff = a->getPositionX() - b->getPositionX();
  double ydiff = a->getPositionY() - b->getPositionY();
  double wrapXdiffA = b->getPositionX() + worldsize - a->getPositionX();
  double wrapYdiffA = b->getPositionY() + worldsize - a->getPositionY();
  double wrapXdiffB = a->getPositionX() + worldsize - b->getPositionX();
  double wrapYdiffB = a->getPositionY() + worldsize - b->getPositionY();
  double deltaX = min(min(abs(xdiff),abs(wrapXdiffA)),abs(wrapXdiffB));
  double deltaY = min(min(abs(ydiff),abs(wrapYdiffA)),abs(wrapYdiffB));
  dist = sqrt(pow(deltaX,2)+pow(deltaY,2));
	
  return dist;
}

bool World::checkCaught(){
  bool caught = false;
  for(int i=1; i<4; i++){
    if(getDistBtw(models[0],models[i])<unitsize){
      caught = true;
      break;
    }
  }
  return caught;
}

Model* World::getClosestPred(){
  //returns a pointer to the closest predator from the prey.
  Model* closestPred;
  double curDist = 10e6; 
  //assuming that the prey is the first element in the array contained in the world object.
  for(int i=1; i<4; i++){
    if(getDistBtw(models[0],models[i])<curDist){
      curDist = getDistBtw(models[0],models[i]);
      closestPred = models[i];
    }
  }
  return closestPred;
}

Model* World::getSecondClosestPred(){
	Model* closestPred = NULL;
	Model* secondClosestPred = NULL;
	double curDist1 = 10e6;
	double curDist2 = 10e6;
	for(int i=1; i<4; i++){
		if(getDistBtw(models[0],models[i])<curDist1){
			curDist2=curDist1;
			curDist1=getDistBtw(models[0],models[i]);
			secondClosestPred = closestPred;
			closestPred = models[i];
		}else if(getDistBtw(models[0],models[i])<curDist2){
			curDist2 = getDistBtw(models[0], models[i]);
			secondClosestPred = models[i];
		}
	}
	return secondClosestPred;
}

Model* World::getFarthestPred(){
	Model* farthestPred;
	double curDist = 0;
	
	for(int i=1; i<4; i++){
		if(getDistBtw(models[0],models[i])>curDist){
			curDist = getDistBtw(models[0], models[i]);
			farthestPred = models[i];
		}
	}
	return farthestPred;
}

double World::getPreyAngle1(Model* pred){
  //This function is written based on the assumption that the fisrt element in the models array is a prey.
  double xdiff = pred->getPositionX() - models[0]->getPositionX();
  double ydiff = pred->getPositionY() - models[0]->getPositionY();
  double wrapXdiffA = models[0]->getPositionX() + worldsize - pred->getPositionX();
  double wrapYdiffA = models[0]->getPositionY() + worldsize - pred->getPositionY();
  double wrapXdiffB = pred->getPositionX() + worldsize - models[0]->getPositionX();
  double wrapYdiffB = pred->getPositionY() + worldsize - models[0]->getPositionY();
  double deltaX = min(min(abs(xdiff),abs(wrapXdiffA)),abs(wrapXdiffB));
  double deltaY = min(min(abs(ydiff),abs(wrapYdiffA)),abs(wrapYdiffB));
	
  //defaulting to the first quadrant.
  bool xCoordinate = true; //positive --> ture, negative --> false.
  bool yCoordinate = true;
  double angle;
/*
	if(deltaX == abs(xdiff)){
		cout << "using xdiff" << endl;
	}else if(deltaX == abs(wrapXdiffA)){
		cout << "using wrapXdiffA" << endl;
	}else{
		cout << "using wrapXdiffB" << endl;
	}
	
	if(deltaY == abs(ydiff)){
		cout << "using ydiff" << endl;
	}else if(deltaY == abs(wrapYdiffA)){
		cout << "using wrapYdiffA" << endl;
	}else{
		cout << "using wrapYdiffB" << endl;
	}
	*/
  //determining in which quadrant the angle lies.
  if (xdiff == 0 && ydiff > 0 ) angle = PI/2;
  else if (xdiff ==0 && ydiff <= 0 ) angle = 3*PI/2;
  else{
    angle = atan(deltaY/deltaX);
    if (deltaX == abs(wrapXdiffA) || (deltaX == abs(xdiff) && xdiff < 0)){
      xCoordinate = false;
    }
    if (deltaY == abs(wrapYdiffA) || (deltaY == abs(ydiff) && ydiff < 0)){
      yCoordinate = false;
    }
    if(xCoordinate == false && yCoordinate == false) angle += PI;
    else if(yCoordinate == false) angle = (-1)*angle;
    else if(xCoordinate == false) angle = PI-angle;
  }
	
  double runAwayAngle = angle + PI;
  return runAwayAngle;
}

double World::getPreyAngle2(Model* pred1, Model* pred2){
	double angle1 = angleIn2PIrange(getPreyAngle1(pred1));
	double angle2 = angleIn2PIrange(getPreyAngle1(pred2));
	//cout << "angle1: " << rtod(angle1) << pred1->name << " angle2: " << rtod(angle2) << pred2->name << endl;
	if(abs(angle2-angle1)>PI){
		if(angle1 < angle2){
			angle1 += 2*PI;
		}else {
			angle2 += 2*PI;
		}

	}
	double d1 = getDistBtw(models[0], pred1);
	double d2 = getDistBtw(models[0], pred2);
	
	double c1 = d2/(d1+d2);
	double c2 = d1/(d1+d2);
	
	double resultAngle;
	
	resultAngle = angle1*c1 + angle2*c2;

	//cout << "resultAngle: " << rtod(resultAngle) << endl;
	return resultAngle;
}

double World::getPreyAngle3(Model* pred1, Model* pred2, Model* pred3){
	vector<double> vec1;
	vector<double> vec2;
	vector<double> vec3;
	vector<double> result;
	double maxdist = sqrt((width / 2)*(width / 2) + (height / 2)*(height / 2));

	vec1 = convertAngleToVector(getPreyAngle1(pred1));
	vec2 = convertAngleToVector(getPreyAngle1(pred2));
	vec3 = convertAngleToVector(getPreyAngle1(pred3));
	
	double d1 = getDistBtw(models[0], pred1);
	double d2 = getDistBtw(models[0], pred2);
	double d3 = getDistBtw(models[0], pred3);

	//using the function described in the Luke et al paper. 
	result.push_back(vec1[0] * (maxdist - d1) + vec2[0] * (maxdist - d2) + vec3[0] * (maxdist - d3));
	result.push_back(vec1[1] * (maxdist - d1) + vec2[1] * (maxdist - d2) + vec3[1] * (maxdist - d3));
	
	double ret = 0;
	ret = convertVectorToAngle(result);
	return ret;
}

double World::getXOffset(Model* a, Model* b){
	
  double xdiff = a->getPositionX() - b->getPositionX();
  double wrapXdiffA = b->getPositionX() + worldsize - a->getPositionX();
  double wrapXdiffB = a->getPositionX() + worldsize - b->getPositionX();
  double deltaX = min(min(abs(xdiff),abs(wrapXdiffA)),abs(wrapXdiffB));
	
  if (deltaX == abs(wrapXdiffB) || (deltaX == abs(xdiff) && xdiff > 0)){
    deltaX = -1*deltaX;
  }
  return deltaX;
}

double World::getYOffset(Model* a, Model* b){
	
  double ydiff = a->getPositionY() - b->getPositionY();
  double wrapYdiffA = b->getPositionY() + worldsize - a->getPositionY();
  double wrapYdiffB = a->getPositionY() + worldsize - b->getPositionY();
  double deltaY = min(min(abs(ydiff),abs(wrapYdiffA)),abs(wrapYdiffB));
	
  if (deltaY == abs(wrapYdiffB) || (deltaY == abs(ydiff) && ydiff > 0)){
    deltaY = -1*deltaY;
  }
  return deltaY;
}

double convertVectorToAngle(vector<double> vec){
	//cout << vec[0] << " " << vec[1] << endl;
	double angle = 0;
	if(vec[0]==0){
		if(vec[1] >= 0){
			angle = PI/2;
		}else{
			angle = PI/2*3;
		}
	}else{
		angle = atan(vec[1]/vec[0]);
		if(vec[1]<0&&vec[0]<0){
			angle += PI;
			//cout << "hello" << endl;
		}else if(vec[0]<0){
			angle += PI;
			//cout << "hi" << endl;
		}
	}
	//cout << angle <<  endl;
	return angle;
}

double rtod(double rad){
	double deg = rad/(2*PI)*360;
	return deg;
}

double angleIn2PIrange(double angle){
	while(!(angle>=0&&angle<2*PI)){
		if(angle<0){
			angle += 2*PI;
		}else if(angle>=2*PI){
			angle -= 2*PI;
		}
	}
	return angle;		
}

vector<double> convertAngleToVector(double angle){
	vector<double> vec;
	double x = cos(angle);
	double y = sin(angle);
	vec.push_back(x);
	vec.push_back(y);
	//cout<< vec[0] << " " << vec[1] << endl;
	return vec;
}

double gaussianRand(double m){
	double num1 = rand() % 99;
	double num2 = rand() % 99;
	num1 = (num1+1)/100;
	num2 = (num2+1)/100;
	//cout << num1 << " " << num2 << endl;
	double ret;
	ret = m*(sqrt(-2*log(num1))*cos(2*PI*num2));
	//cout << ret << endl;
	return ret;
}

