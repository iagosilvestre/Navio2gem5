#include "Icontroller.hpp"
#include <iostream>
#include <Eigen/Eigen>
#include "include/Sensor.h"
#include "include/SensorArray.h"
#include "math.h"
#include "franklin.cpp"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <memory>
#include <string>
#include <unistd.h>

struct timeval t0, t1;
struct timeval dtCon,debug;
std::vector<int> controlData;
unsigned long int auxCount=0;
int main()
	{
		
	simulator_msgs::SensorArray arraymsg;
	std::vector<double> out;
	std::vector<double> xref;
	std::vector<double> error;
	std::vector<double> x;
	int k=0;


//		Foo* foo1 = new Foo ();
//		std::cout << "test if main works" << std::endl;;
		vant2load_Franklin* control = new vant2load_Franklin();


	

		control->config();

		

//		simulator_msgs::Sensor msgstates;
//		arraymsg.header;
//		msgstates = arraymsg.values.at(0);

		while(k<50){
		
		gettimeofday(&t0, NULL);
		out=control->execute(arraymsg);
		gettimeofday(&t1, NULL);
		timersub(&t1, &t0, &dtCon);
		controlData.push_back(dtCon.tv_usec);
		
		k++;
		}
		
		FILE *fCON = fopen("control.txt", "w");
		fprintf(fCON, "count;dtCon\n");
		fclose(fCON);
		for (std::vector<int>::iterator it = controlData.begin() ; it != controlData.end(); ++it){
			auxCount++;
			FILE *fCON = fopen("control.txt", "a");
			fprintf(fCON, "%d;%lu\n",auxCount,*it);
			fclose(fCON);
		}
		return 0;
	}
