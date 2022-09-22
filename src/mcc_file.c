#include <stdio.h>
#include <mcc_scan.h>

int write_to_file(struct DATA_SENT data)
{
    FILE *fp;
    fp = fopen("data.txt", "a+");
    
    for(int i=0;i<sizeof(data.voltage[0])/sizeof(data.voltage[0][0]);i++){
        if(data.voltage[0][i]<20 && data.voltage[1][i]<20 && data.voltage[2][i]<20 && data.voltage[3][i]<20){
            fprintf(fp,"%f %f %f %f\n",data.voltage[0][i],data.voltage[1][i],data.voltage[2][i],data.voltage[3][i]);            
        }
        
    } 
    fclose(fp);

}
