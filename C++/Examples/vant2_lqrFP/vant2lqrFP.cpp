#include "Icontroller.hpp"
#include <iostream>
#include <Eigen/Eigen>
#include "include/Sensor.h"
#include "include/SensorArray.h"
#include "math.h"
#include "lqr.cpp"
#include <algorithm>
#include <chrono>
#include <vector>
#include <memory>
#include <string>
//#include "m5op.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>



int main()
	{
	std::vector<int> controlData;
	unsigned long int auxCount=0;

	simulator_msgs::SensorArray arraymsg;
	std::vector<double> out;
	std::vector<double> xref;
	std::vector<double> error;
	std::vector<double> x;
	int k=0;

		teste* control = new teste();

		control->config();

		while(k<100){

		out=control->execute(arraymsg);
		for (auto i = out.begin(); i != out.end(); ++i)
    	std::cout << *i << ' ';
		k++;
		std::cout << " \n";	
		}

		return 0;
	}
