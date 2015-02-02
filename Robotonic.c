#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define BUF_SIZE 8
#define SAMP_LENGTH 1024
#define RATE 8000.0
#define NOISE_LEVEL 0

int8_t buf[BUF_SIZE*SAMP_LENGTH];
int8_t mbuf[SAMP_LENGTH];
uint32_t wvtp;
uint16_t freq;
int i;
int8_t intrvl;
int8_t offset;

void gen_wave(int8_t*,uint8_t,uint16_t,uint8_t,uint8_t);
void cp_array(int8_t*,int8_t*,int);
void clear_mbuf(int8_t*);
void clear_buf(int8_t*);

int main(int argc,char** argv){
	wvtp=0;
	clear_buf(buf);
	while(1){
		switch (read(STDIN_FILENO,&wvtp,4)) {
			case 0:
				exit(0);
			break;
			case -1:
				write(STDERR_FILENO,"Read error\n",12);
			break;
		}
		freq=((wvtp >> 16) % 0x4000)+0x80;
		intrvl=(wvtp >> 5) % 0x08;
		intrvl=intrvl?intrvl:4;
		offset=wvtp % intrvl;
		clear_mbuf(mbuf);
		gen_wave(mbuf,(wvtp >> 30) % 0x04,freq,(wvtp >> 27) % 0x08,(wvtp >> 24) % 0x08);
		for (i=offset;i<BUF_SIZE;i+=intrvl){
			cp_array(mbuf,buf,i);
		}
		for (i=0;i<BUF_SIZE;i++){
			write(STDOUT_FILENO,&buf[i*SAMP_LENGTH],SAMP_LENGTH);
		}
	}
	return 0;
}

void gen_wave(int8_t* arr,uint8_t type,uint16_t freq, uint8_t lfo_type,uint8_t lfo_coef){
	int i;
	float x;
	double cv;
	for (i=0;i<SAMP_LENGTH;i++) {
		x=i*freq/RATE;
		switch(type){
			case 0:
				cv=(int)(x*2)%2?-2*x:2*x;
			break;
			case 1:
				cv=(int)(x*2)%2?1:0;
			break;
			case 2:
				cv=sin(x*6.28);
			break;
			case 3:
				//cv=drand48();
				cv=1-2*x;
			break;
		}
		switch(lfo_type){
			case 1:
				cv*=(1-i/(double)SAMP_LENGTH);
			break;
			case 2:
				cv*=(1-sqrt(-i/(double)SAMP_LENGTH));
			break;
			case 3:
				cv*=(1-sqrt(i*(double)(SAMP_LENGTH)));
			break;
		}
		arr[i]+=cv*128;
	}
}
void cp_array(int8_t* marr,int8_t* barr,int pos) {
	//memcpy(barr+pos*SAMP_LENGTH,marr,SAMP_LENGTH);
	int i;
	for (i=0;i<SAMP_LENGTH;i++)
		*(barr+pos*SAMP_LENGTH+i)=marr[i]+(int)(drand48()*NOISE_LEVEL);
}
void clear_mbuf(int8_t* buf){
	//memset(&buf,0,sizeof(buf));
	int i;
	for (i=0;i<SAMP_LENGTH;i++)
		buf[i]=0;
}
void clear_buf(int8_t* buf){
	//memset(&buf,0,sizeof(buf));
	int i;
	int j;
	for (i=0;i<BUF_SIZE;i++)
		for (j=0;j<SAMP_LENGTH;j++)
			*(buf+i*SAMP_LENGTH+j)=0;
}
