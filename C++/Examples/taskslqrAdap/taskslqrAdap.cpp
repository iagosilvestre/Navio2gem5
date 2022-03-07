#include "Icontroller.hpp"
#include <iostream>
#include <Eigen/Eigen>
#include "include/Sensor.h"
#include "include/SensorArray.h"
#include "math.h"
//#include "hinfinity.cpp"
#include "adap.cpp"
#include "lqr.cpp"
#include "m5op.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h>
#include <ios>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <pthread.h>
#include <sys/resource.h>
int countAdap=0;

void* adapThread(void *)
{
	rusage ru_adap;
	simulator_msgs::SensorArray arraymsg;
	std::vector<double> outA;
	std::vector<double> xref;
	std::vector<double> error;
	std::vector<double> x;

	vant3_adaptiveMixCtrl2* controlA = new vant3_adaptiveMixCtrl2();

	controlA->config();

	while(countAdap<100){
	//m5_reset_stats(0,0);
	outA=controlA->execute(arraymsg);
	getrusage(RUSAGE_THREAD,&ru_adap);
	printf("suco de maracuja\n");
	//printf("Adaptive Thread voluntary context switches :%ld\n",ru_adap.ru_nvcsw);
	//printf("Adaptive Thread involuntary context switches :%ld\n",ru_adap.ru_nivcsw);
	//m5_dump_stats(0,0);
	countAdap++;
	//std::cout << k << " control executed" << std::endl;
	}
    pthread_exit(NULL);			/* terminate the thread */
}
void* lqrThread(void *)
{
	rusage ru_lqr;
	simulator_msgs::SensorArray arraymsg;
	std::vector<double> outL;
	std::vector<double> xref;
	std::vector<double> error;
	std::vector<double> x;

	teste* controlL = new teste();

	controlL->config();

	while(countAdap<100){
	//m5_reset_stats(0,0);
	outL=controlL->execute(arraymsg);
	//(0,0);

	//std::cout << k << " control executed" << std::endl;
	}
    pthread_exit(NULL);			/* terminate the thread */
}
int main()
	{
	//m5_reset_stats(0,0);
	pthread_t threadL, threadA;
	int iret1,iret2;

	iret1 = pthread_create( &threadA, NULL, adapThread,NULL);
  iret2 = pthread_create( &threadL, NULL, lqrThread,NULL);
	//std::cout << "control configured" << std::endl;

	(void) pthread_join(threadA, NULL);
	(void) pthread_join(threadL, NULL);
	//m5_dump_stats(0,0);
	pthread_exit(0);
}
