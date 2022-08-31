#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "ADS1263.h"
#include "stdio.h"
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


ADS ads1,ads2;


#define N 100000
#define REF			5.08		//Modify according to actual voltage

void* thread_func1(void* arg) //ADS placa?
{   
    ADS* a = arg;
    a->data=malloc(sizeof(double)*N);
    int i;
    //double* b = malloc(sizeof * b);
    for(i=0;i<N;i++){
        a->data[i] = ADS1263_GetChannelValue(1, *a);
    }    
    return a;
}


void* thread_func2(void* arg)
{
    double* a = arg;
    //double* b = malloc(sizeof * b);
    *a = ADS1263_GetChannelValue(1, ads2);

    return a;
}

static inline double ADC2double(UDOUBLE in){ //conversion of data

    float out;
    int dec;
    memcpy(&dec,&in,4);
    out = (double) dec* 1.164153218e-9;
    
/*
    if((in>>31) == 1) {
	out = - ((double) REF*2 - (double) in/ 2147483648.0 * (double)REF);
    }
    else{
 	out = in/2147483647.0 * (double)REF;
    }
    */
    return(out);
}

void  Handler(int signo) 
{
    //System Exit
    printf("\r\n END \r\n");
    DEV_Module_Exit(ads1);
    DEV_Module_Exit(ads2);
    exit(0);
}

int main(void) {


    signal(SIGINT, Handler);


    ads1.DEV_RST_PIN     = 25;//25;//18;
	ads1.DEV_CS_PIN      = 23;//23;//22;
	ads1.DEV_DRDY_PIN    = 24;//24;//17;

    ads2.DEV_RST_PIN     = 18;
	ads2.DEV_CS_PIN      = 22;
	ads2.DEV_DRDY_PIN    = 17;

    DEV_Module_Init(ads1);
    DEV_Module_Init(ads2);
	ADS1263_SetMode(1);
    
	
	if(ADS1263_init_ADC1(ADS1263_38400SPS,ads2) == 1) {
		printf("\r\n END \r\n");
		DEV_Module_Exit(ads2);
		exit(0);
	}

	if(ADS1263_init_ADC1(ADS1263_38400SPS,ads1) == 1) {
		printf("\r\n END \r\n");
		DEV_Module_Exit(ads1);
		exit(0);
	}


	UDOUBLE ADC[10];
    UDOUBLE raw[N];
       ADS1263_SetDiffChannel(1,ads2);

    ADS1263_SetDiffChannel(1,ads1);

    //calibrate(ads1);
    //calibrate(ads2);



    double measures[N], times[N],measures2[N];

 
    struct timespec start={0, 0}, finish={0, 0}; 
    clock_gettime(CLOCK_REALTIME, &start);


    pthread_t thr1,thr2,udp;
    //struct thread_args in = { .a = 10, .b = 3.141592653 };
    void* out_1;
    void* out_2;
    //struct thread_result* out;
    //double *a=NULL, *b=NULL;

    //pthread_create(&thr1, NULL, &thread_func1, &ads1);
    //pthread_create(&thr2, NULL, &thread_func2, &ads2);

    //pthread_join(thr1, &out_1);
    //pthread_join(thr2, &out_2);


    //printf("out -> x = %ld\tout -> b = %f\n", &a, &b);
    //put a into array?




    for (int i=0; i<N; i++) {

	ADC[0]=ADS1263_GetChannelValue(1,ads1);
        ADC[1]=ADS1263_GetChannelValue(1,ads2);

        clock_gettime(CLOCK_REALTIME, &finish);
	times[i] =  (double)(finish.tv_sec-start.tv_sec)*1000.0 + (double)(finish.tv_nsec-start.tv_nsec)/1000000.0;

        measures[i]=ADC2double(ADC[0]);
        measures2[i]=ADC2double(ADC[1]);
        raw[i]=ADC[0];

        //printf("%x %x\n",ADC[0],ADC[1]);

        
    }
FILE *fp=fopen("test1.txt","w");
    for(int i=0; i<N; i++) {
	    fprintf(fp,"%d %f %f %f %d\n",i,times[i],measures[i],measures2[i],raw[i]);
    }

return(0);
}
