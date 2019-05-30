/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Get pressure from MS5611 barometer onboard of Navio shield for Raspberry Pi

To run this example navigate to the directory containing it and run following commands:
make
./Barometer
*/

#define _GNU_SOURCE
#include "lib/profiler.h"
#include <sched.h>
#include <string>
#include <stdio.h>
#include <memory>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <vector>


#define ARM_MATH_CM4
#include "pv_module_co.h"
//#include "c_control_lqrArthur.h"
#include "pv_typedefs.h"
#include "c_control_lqrArthur_vel.h"
#include "c_control_lqrArthur_vel.c"
#include "lib/arm_mat_init_f32.c"
#include "lib/arm_mat_sub_f32.c"
#include "lib/arm_mat_mult_f32.c"
#include "lib/arm_math.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MODULE_PERIOD	 12//ms
#define ESC_ON           1
#define SERVO_ON         1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//portTickType lastWakeTime;
pv_msg_input iInputData;
pv_msg_controlOutput oControlOutputData;
pv_type_datapr_position iRefData;
pv_type_datapr_position pos_ref;

/* Inboxes buffers */
pv_type_actuation    iActuation;
/* Outboxes buffers*/
struct timeval t0, t1;
struct timeval dtCon,debug;
std::vector<int> controlData;
//std::vector<int> servoRightData;
//std::vector<int> servoLeftData;
//std::vector<int> escRightNewtonsData;
//std::vector<int> escLeftNewtonsData;

void module_co_init()
{

  /* Inicializar os servos */
  //feito no module in

  /*Inicializar o tipo de controlador*/


  /* Pin for debug */
  //LED5 = c_common_gpio_init(GPIOD, GPIO_Pin_14, GPIO_Mode_OUT); //LD5

  //pv_interface_co.iInputData          = xQueueCreate(1, sizeof(pv_msg_input));
  //pv_interface_co.oControlOutputData  = xQueueCreate(1, sizeof(pv_msg_controlOutput));
  //pv_interface_co.iRefData  = xQueueCreate(1, sizeof(pv_type_datapr_attitude));
  //c_control_lqrArthur_init();
  c_control_lqrArthur_vel_init();
  pos_ref.x = 2.0;
  pos_ref.y = 0.0;
  pos_ref.z = 1.5;
}

void module_co_run()
{
//	unsigned int heartBeat=0;


	/* Inicializa os dados da attitude*/
	oControlOutputData.actuation.servoRight = 0;
	oControlOutputData.actuation.servoLeft  = 0;
	oControlOutputData.actuation.escRightNewtons = 10.2751;
	oControlOutputData.actuation.escLeftNewtons = 10.2799;

  for( int k = 0; k < 10000; k += 1 ){

//  {
	/* Variavel para debug */
	  int heartBeat=0;
	  gettimeofday(&t0, NULL);
	  heartBeat+=1;
	/* Leitura do numero de ciclos atuais */
	//lastWakeTime = xTaskGetTickCount();

	if ((heartBeat%10)==0)
	{
		/*pos_ref.x += iRefData.x;
		pos_ref.y += iRefData.y;
		pos_ref.z += iRefData.z;*/
		/*pos_ref.x = iRefData.x;
		pos_ref.y = iRefData.y;
		pos_ref.z = iRefData.z;
		pos_ref.dotX = iRefData.dotX;
		pos_ref.dotY = iRefData.dotY;
		pos_ref.dotZ = iRefData.dotZ;*/
		pos_ref.x += rand() % 20;
		pos_ref.y += rand() % 20;
		pos_ref.z += rand() % 20;
		pos_ref.dotX += rand() % 20;
		pos_ref.dotY += rand() % 20;
		pos_ref.dotZ += rand() % 20;

	}


	/* Passa os valores davariavel compartilha para a variavel iInputData */
	if (1)
	{
		iInputData.position_reference.x = pos_ref.x;
		iInputData.position_reference.y = pos_ref.y;
		iInputData.position_reference.z = pos_ref.z;
		iInputData.position_reference.dotX = pos_ref.dotX;
		iInputData.position_reference.dotY = pos_ref.dotY;
		iInputData.position_reference.dotZ = pos_ref.dotZ;
		iInputData.position.x = pos_ref.x;
		iInputData.position.y = pos_ref.y;
		iInputData.position.z = pos_ref.z;
		iInputData.position.dotX = pos_ref.dotX;
		iInputData.position.dotY = pos_ref.dotY;
		iInputData.position.dotZ = pos_ref.dotZ;
		oControlOutputData.actuation = c_control_lqrArthur_vel_controller(iInputData);

//		servoRightData.push_back(oControlOutputData.actuation.servoRight);
//		servoLeftData.push_back(oControlOutputData.actuation.servoLeft);
//		escRightNewtonsData.push_back(oControlOutputData.actuation.escRightNewtons);
//		escLeftNewtonsData.push_back(oControlOutputData.actuation.escLeftNewtons);

		//printf("\n escRightNewtons = %f\n",oControlOutputData.actuation.escRightNewtons);
		//printf("\n escLeftNewtons = %f\n",oControlOutputData.actuation.escLeftNewtons);
		//printf("\n servoRight = %f\n",oControlOutputData.actuation.servoRight);
		//printf("\n servoLeft = %f\n",oControlOutputData.actuation.servoLeft);
		//oControlOutputData.actuation = c_control_lqrArthur_controller(iInputData);
	}

	/* toggle pin for debug */
	//c_common_gpio_toggle(LED5);


	  //xQueueOverwrite(pv_interface_co.oControlOutputData, &oControlOutputData); Sending Data

	/* A thread dorme ate o tempo final ser atingido */
	//vTaskDelayUntil( &lastWakeTime, MODULE_PERIOD / portTICK_RATE_MS);

		//sleep(1);
	gettimeofday(&t1, NULL);
	timersub(&t1, &t0, &dtCon);
	controlData.push_back(dtCon.tv_usec);
	printf("\n escRightNewtons = %f\n",oControlOutputData.actuation.escRightNewtons);
	printf("\n escLeftNewtons = %f\n",oControlOutputData.actuation.escLeftNewtons);
	printf("\n servoRight = %f\n",oControlOutputData.actuation.servoRight);
	printf("\n servoLeft = %f\n",oControlOutputData.actuation.servoLeft);
	}
}

int main()
{
	unsigned long int auxCount=0;
	//ProfilerStart("ControlOneExecution.log");
//	gettimeofday(&t0, NULL);
	module_co_init();
//	printf("\n Modulo de Controle Iniciado\n");
	module_co_run();
//	printf("\n Thread de controle executada uma vez!\n");
//	gettimeofday(&t1, NULL);
//	timersub(&t1, &t0, &dtCon);
	gettimeofday(&t0, NULL);
	usleep(200000);
	gettimeofday(&t1, NULL);
	timersub(&t1, &t0, &debug);
	printf("200.000 us:%lu\n",debug.tv_usec);
	//ProfilerStop();


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
