#include "Icontroller.hpp"
#include <iostream>
#include <Eigen/Eigen>
#include "include/Sensor.h"
#include "include/SensorArray.h"
#include "math.h"
#include "adap.cpp"
#include "hinfinity.cpp"
#include <algorithm>
#include <chrono>
#include <vector>
#include <memory>
#include <string>
//#include "m5op.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <pthread.h>
#include <sys/resource.h>

int count=0;
simulator_msgs::SensorArray arraymsg;
simulator_msgs::SensorArray arraymsg2;
std::vector<double> outA;
std::vector<double> outH;
std::vector<double> xref;
std::vector<double> error;
std::vector<double> x;

void *adap( void *ptr );
void *hinf( void *ptr );
vant3_adaptiveMixCtrl2* controlA = new vant3_adaptiveMixCtrl2();
hinfinity* controlH = new hinfinity();

int stick_this_thread_to_core(int core_id) {
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   if (core_id < 0 || core_id >= num_cores)
      return EINVAL;

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);

   pthread_t current_thread = pthread_self();    
   return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}



main()
{
     pthread_t thread1, thread2;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;
     

		 controlA->config();
		 controlH->config();
    /* Create independent threads each of which will execute function */

     iret1 = pthread_create( &thread1, NULL, hinf, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, adap, (void*) message2);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL);


     pthread_exit(NULL);
}

void *adap( void *ptr )
{	
	rusage ru_adap;
	rusage ru_adapO;
	stick_this_thread_to_core(0);
	std::vector<int> controlData;
	std::vector<long> vincData;
	std::vector<long> invincData;
	unsigned long int auxCount=0;	
	long vinc=0;
	long invinc=0;

	getrusage(RUSAGE_SELF,&ru_adapO);
	while(count<100){
   //m5_reset_stats(0,0);
	   	auto start = std::chrono::high_resolution_clock::now();	
		outA=controlA->execute(arraymsg);
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		//printf("preempt count is %d\n",current_thread_info()->preempt_count);
		getrusage(RUSAGE_THREAD,&ru_adap);
		//vinc=ru_adap2.ru_nvcsw-ru_adap.ru_nvcsw;
		//vincData.push_back(vinc);
		invinc=ru_adap.ru_nivcsw-ru_adapO.ru_nivcsw;
		invincData.push_back(invinc);
		getrusage(RUSAGE_SELF,&ru_adapO);
		long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		controlData.push_back(microseconds);
		//m5_dump_stats(0,0);
   //m5_dump_stats(0,0);
   }
	FILE *fCON = fopen("adap.txt", "w");
				fprintf(fCON, "count;dtCon\n");
				fclose(fCON);
				for (std::vector<int>::iterator it = controlData.begin() ; it != controlData.end(); ++it){
					auxCount++;
					FILE *fCON = fopen("adap.txt", "a");
					fprintf(fCON, "%d;%lu;%lu\n",auxCount,*it,invincData[auxCount-1]);
					fclose(fCON);
				}
   pthread_exit(NULL);				/* terminate the thread */
}

void *hinf( void *ptr )
{
		stick_this_thread_to_core(0);
		std::vector<int> controlData;
		unsigned long int auxCount=0;
		while(count<100){
		auto start = std::chrono::high_resolution_clock::now();
		//m5_reset_stats(0,0);
		outH=controlH->execute(arraymsg);
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		//printf("preempt count is %d\n",current_thread_info()->preempt_count);
		long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		controlData.push_back(microseconds);
		//m5_dump_stats(0,0);
		count++;
		}

		FILE *fCON = fopen("hinf.txt", "w");
				fprintf(fCON, "count;dtCon\n");
				fclose(fCON);
				for (std::vector<int>::iterator it = controlData.begin() ; it != controlData.end(); ++it){
					auxCount++;
					FILE *fCON = fopen("hinf.txt", "a");
					fprintf(fCON, "%d;%lu\n",auxCount,*it);
					fclose(fCON);
				}
		pthread_exit(NULL);	/* terminate the thread */
}
