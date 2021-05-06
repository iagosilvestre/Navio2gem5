#include "Icontroller.hpp"
#include <iostream>
#include <Eigen/Eigen>
#include "include/Sensor.h"
#include "include/SensorArray.h"
#include "math.h"
#include "lqr.cpp"
#include "m5op.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void print(std::vector<auto> const &input)
{
	for (int i = 0; i < input.size(); i++) {
		std::cout << input.at(i) << ' ';
	}
}

int main()
	{
	//m5_reset_stats(0,0);
	simulator_msgs::SensorArray arraymsg;
	std::vector<double> out;
	std::vector<double> xref;
	std::vector<double> error;
	std::vector<double> x;
	int k=0;

//		m5_dump_stats(0,0);
//		m5_reset_stats(0,0);
//		Foo* foo1 = new Foo ();
//		std::cout << "test if main works" << std::endl;;
		teste* control = new teste();
	
//		m5_dump_stats(0,0);
//		m5_reset_stats(0,0);

		control->config();

//		m5_dump_stats(0,0);
//		simulator_msgs::Sensor msgstates;
//		arraymsg.header;
//		msgstates = arraymsg.values.at(0);
		m5_reset_stats(0,0);
		while(k<10000){
		out=control->execute(arraymsg);
//		print(out);
//		std::cout << "\n";
		k++;
		}
		m5_dump_stats(0,0);
//m5_dump_stats(0,0);
		return 0;
	}