#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <mcc_scan.h>




void data_split(double *buff, struct DATA_SENT *dest)
{

    for (size_t j = 0; j< channels_read; j++)
    {
        int k = 0 ;

        for (int i = j; i < buff_size*channels_read; i = i + channels_read)
        {
            dest->voltage[j][k]=(float) buff[i];
            k++;
        }
        
    }
    
}


