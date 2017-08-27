#include <stdio.h>

/*
Because lcd and serial don't support printf, and its very costly, and all we need
is simple formating with a certain number of digits and precision, this ftoa is enough.
If digits is negative, it will pad left.
*/
#define  BUF_LEN 100
char buf[BUF_LEN]; //need a buffer to hold formatted strings to send to LCD
#define absq(amt) ((amt)<0?0-(amt):(amt))

int ftoa(	char * str, 	//buffer to hold result.
	float f,    	//input value
	char digits,	//total number of digits
	char precision	//digits right of decimal
	) {
long pow10[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
char i=0,k,l=0;
unsigned int a,c;
unsigned char b;
char decimal='.';
char pad=' '; //could be a parameter to support custom padding character.

  if((sizeof(pow10)/sizeof(pow10[0]))<=absq(digits)) {//larger pow10 table needed
  	str[i++]='O';
  	str[i]='\0';
  	return i;
  	}; 
  // check for negative float
  if(f<0.0) { //is it negative?
    str[i++]='-'; //indicate
    f*=-1; //make it positive
    (0<digits?digits--:digits++); //optional, steal digit for sign, keep length
    }
  a=(unsigned int)f;// extracting whole number
  f-=a;	// extracting decimal part
  k = digits;
  // number of digits in whole number
  while(k>0) { //note this doesn't happen if digits was negative: Padding.
    c = pow10[k];
    c = a/c;
    if(c>0) { break; }
    k--;
    } // number of digits in whole number are k+1
/*
extracting most significant digit i.e. right most digit , and concatenating to string
obtained as quotient by dividing number by 10^k where k = (number of digit -1)
*/
  for(l=absq(k);l>=0;l--){
    c = pow10[l];	//get the next power
    b = a/c;	//get the next digit
    if (b>0) pad='0';	//stop padding after first digit.
    if (b>10) b='e'-'0';//overflow
    str[i++]=(l&&!b?pad:b+'0'); //digit or pad, l&& adds leading zero for fractions
    a%=c;   	//modulo by power
    }
  if (precision) {str[i++] = decimal;};
/* extracting decimal digits till precision */
  if (0>precision) {k=0; precision=0-precision;}
  for(l=precision;0<l;l--) {
    f*=(float)10.0;
    b = (int)(f); //math floor so round half way
    str[i++]=b+'0'; //convert to  ASCII
    f-=(int)f;
    if (k && 0.00001>f) { break; } //nothing left, save chars.
    //0==f won't work if there are any floating point errors.
    }
  str[i]='\0'; //null terminate the buffer
  return i;
  }


int main(void) {
int i;
float f;
	i=0;
	for (f=0.0001;f<0.00091;f+=0.0001) {
		ftoa(buf+i, f, 5, -4);buf[i+6]=' ';i+=7;buf[i]=0;
		}
	puts(buf);
	i=0;
	for (f=0.001;f<0.0091;f+=0.001) {
		ftoa(buf+i, f, 5, -4);buf[i+6]=' ';i+=7;buf[i]=0;
		}
	puts(buf);
	i=0;
	for (f=0.01;f<0.091;f+=0.01) {
		ftoa(buf+i, f, 5, -4);buf[i+6]=' ';i+=7;buf[i]=0;
		}
	puts(buf);
	i=0;
	for (f=0.1;f<0.91;f+=0.1) {
		ftoa(buf+i, f, 5, -4);buf[i+6]=' ';i+=7;buf[i]=0;
		}
	puts(buf);
	ftoa(buf, .12345, 0, 4);buf[6]=' ';
	ftoa(buf+7, .12345, 0, 3);buf[12]=' ';
	ftoa(buf+13, .12345, 0, 2);buf[17]=' ';
	ftoa(buf+18, .12345, 0, 1);buf[21]=' ';
	ftoa(buf+22, .12345, 0, 0);puts(buf);
	ftoa(buf, -4404.322, -8, -4);puts(buf);
	ftoa(buf, 4000123.1014, -3, 3);puts(buf);
	ftoa(buf, 4000123.1014, 9, 3);puts(buf);
	return 0;
}
