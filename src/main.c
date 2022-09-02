#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include <time.h>
#include "ADS1263.h"
#include "stdio.h"
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define REF 5.08 // Modify according to actual voltage

ADS ads1, ads2;

static inline double ADC2double(UDOUBLE in)
{ // conversion of data

    float out;
    int dec;
    memcpy(&dec, &in, 4);
    out = (double)dec * 1.164153218e-9;

    /*
        if((in>>31) == 1) {
        out = - ((double) REF*2 - (double) in/ 2147483648.0 * (double)REF);
        }
        else{
        out = in/2147483647.0 * (double)REF;
        }
        */
    return (out);
}

void udp_send(ADS placa) // script to send info to the server
{
}

void *thread_func(void *arg)
{
    ADS *a = arg;
    struct timespec start = {0, 0}, finish = {0, 0};
    UDOUBLE temp; // raw hex data before conversion
    for (size_t j = 0; j < N_BUFFERS; j++)
    {
        a->data[j] = malloc(sizeof(double) * N_SAMPLES);
        a->tempo[j] = malloc(sizeof(double) * N_SAMPLES);
    }

    int i;
    a->buf_select = 0;

    // while (1)
    {
        a->buf_status[a->buf_select] = BUF_STATE_EMPTY;
        for (i = 0; i < N_SAMPLES; i++)
        {
            printf("pino: %d\n", a->DEV_CS_PIN);
            
            temp = ADS1263_GetChannelValue(1, *a);
            // a->data[a->buf_select][i] = ADS1263_GetChannelValue(1, *a);
            a->data[a->buf_select][i] = ADC2double(temp);
            clock_gettime(CLOCK_REALTIME, &finish);
            a->tempo[a->buf_select][i] = (double)(finish.tv_sec - start.tv_sec) * 1000.0 + (double)(finish.tv_nsec - start.tv_nsec) / 1000000.0;
        }
        a->buf_status[a->buf_select] = BUF_STATE_FULL;
        (a->buf_select++) % N_BUFFERS;
    }
    return NULL;
}

void Handler(int signo)
{
    // System Exit
    printf("\r\n END \r\n");
    DEV_Module_Exit(ads1);
    DEV_Module_Exit(ads2);
    exit(0);
}

int main(void)
{

    signal(SIGINT, Handler);

    ads1.DEV_RST_PIN = 25;  // 25;//18;
    ads1.DEV_CS_PIN = 23;   // 23;//22;
    ads1.DEV_DRDY_PIN = 24; // 24;//17;
    ads1.ads_identifier = 0;

    ads2.DEV_RST_PIN = 18;
    ads2.DEV_CS_PIN = 22;
    ads2.DEV_DRDY_PIN = 17;
    ads2.ads_identifier = 1;

    DEV_Module_Init(ads1);
    DEV_Module_Init(ads2);
    ADS1263_SetMode(1);

    if (ADS1263_init_ADC1(ADS1263_38400SPS, ads1) == 1)
    {
        printf("\r\n END \r\n");
        DEV_Module_Exit(ads1);
        exit(0);
    }

    if (ADS1263_init_ADC1(ADS1263_38400SPS, ads2) == 1)
    {
        printf("\r\n END \r\n");
        DEV_Module_Exit(ads2);
        exit(0);
    }

    UDOUBLE ADC[10];
    UDOUBLE raw[N_SAMPLES];

    // calibrate(ads1);
    //  calibrate(ads2);

    ADS1263_SetDiffChannel(1, ads2);

    ADS1263_SetDiffChannel(1, ads1);

    // printf("calib register : %d %d\n", ADS_read_calib_reg(ads1), ADS_read_calib_reg(ads2));

    double measures[N_SAMPLES], times[N_SAMPLES], measures2[N_SAMPLES];

    // getchar();

    struct timespec start = {0, 0}, finish = {0, 0};
    clock_gettime(CLOCK_REALTIME, &start);

    pthread_t thr1, thr2, udp;
    void *res1, *res2; // thread results from func

    // struct thread_result* out;

    pthread_create(&thr1, NULL, &thread_func, &ads1);
    printf("thread 1 init\n");
    pthread_create(&thr2, NULL, &thread_func, &ads2);

    pthread_join(thr1, &res1);
    printf("thread 1 end\n");
    pthread_join(thr2, &res2);
    /*
        for (int i = 0; i < N_SAMPLES; i++)
        {

            ADC[0] = ADS1263_GetChannelValue(1, ads1);
            ADC[1] = ADS1263_GetChannelValue(1, ads2);

            clock_gettime(CLOCK_REALTIME, &finish);
            times[i] = (double)(finish.tv_sec - start.tv_sec) * 1000.0 + (double)(finish.tv_nsec - start.tv_nsec) / 1000000.0;

            measures[i] = ADC2double(ADC[0]);
            measures2[i] = ADC2double(ADC[1]);
            raw[i] = ADC[0];

            // printf("%d %d\n",ADC[0],ADC[1]);
        }
    */
    FILE *fp = fopen("test1.txt", "w");
    for (int i = 0; i < N_SAMPLES; i++)
    {
        fprintf(fp, "%d %f %f %f %x\n", i, times[i], measures[i], measures2[i], raw[i]);
    }
    /*
        FILE *fp1 = fopen("thread_test.txt", "w");
        for (int j = 0; j < N_SAMPLES; j++)
        {
            fprintf(fp, "%d %f %f %f\n", j, times[j], ads1.data[j], ads2.data[j]);
        }
        return (0);
        */
}
