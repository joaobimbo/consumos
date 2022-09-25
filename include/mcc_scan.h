#ifndef _MCC_SCAN_H
#define _MCC_SCAN_H

#define buff_size 2048
#define channels_read 4


struct DATA_SENT
{
    double voltage[channels_read][buff_size];
    double delta_time_ms[buff_size];

};

void data_split(double *buff, struct DATA_SENT *dest);
int write_to_file(struct DATA_SENT data);


#endif