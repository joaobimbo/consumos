#include <math.h>

#include <fftw3.h>
int main ()
{
    double pi=3.14;
    double t[100000];
    double x[100000];
    int cont=1;
    //sampling rate
    double sr =  100000;
    //sampling interval
    double ts = 1/sr;
    printf("ts %f\r\n",ts); 


    while (t[cont]<=1) 
    {
        t[cont]=t[cont-1]+ts;
        x[cont] = 3*sin(2*pi*10*t[cont])+sin(2*pi*200*t[cont])+ 0.5*sin(2*pi*700*t[cont]);
        cont=cont+1;
    }
    printf("fim while \n");
    int freq = 10;
    //fft(x);
        fftw_complex *in, *out;
    fftw_plan p;
    ...
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    ...
    fftw_execute(p); /* repeat as needed */
    ...
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
    

    freq = 200;
    //x = x + ;

    freq = 700;
    //x = x ;

    return 0;
}





