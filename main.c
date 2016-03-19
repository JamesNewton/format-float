#include <stdio.h>

/*
Because sprintf, is very costly, and all we need
is simple formating with a certain number of digits and precision, this ftoa is enough.
If digits is negative, it will pad left. 
If precision is negative, it should stop when zero
*/
#define  BUF_LEN 20
char buf[BUF_LEN]; //need a buffer to hold formatted strings to send to LCD
 
int ftoa(char * str, float f, char digits, char precision) {
char i=0,k,l=0;
long a,c;
long pow10[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
unsigned char b;
char decimal='.';
 
  if(digits>=10) {return 0;};
  // check for negative float
  if(f<0.0) {
    str[i++]='-';
    f*=-1;
    (0<digits?digits--:digits++);
    }
  a=(int)f;	// extracting whole number
  f-=a;	// extracting decimal part
  k = digits;
  // number of digits in whole number
  while(k>=0)	{
    c = pow10[k];
    c = a/c;
    if(c>0) { break; }
    k--;
    } // number of digits in whole number are k+1
  if (0<k && digits==k && c>10) { //overflow
    decimal = 'e';
    }
/*
extracting most significant digit i.e. right most digit , and concatenating to string
obtained as quotient by dividing number by 10^k where k = (number of digit -1)
*/
  for(l=abs(k);l>=0;l--){
    c = pow10[l];
    b = a/c;
    str[i++]=(l&&!b?' ':b+48); //digit or pad
    a%=c;
    }
  if (precision) {str[i++] = decimal;};
/* extracting decimal digits till precision */
  if (0>precision) {k=0; precision=abs(precision);}
  for(l=0;l<precision;l++) {
    f*=10.0;
    b = (int)f; //math floor
    str[i++]=b+48; //48 is ASCII 0
    f-=(float)b;
    if (!k && 0==f) { break; } //nothing left, save chars.
    //won't work if there are any floating point errors.
    }
  str[i]='\0';
  return i;
  }
 
 
int main(void) {
	ftoa(buf, -4444.444, -8, 3);
	puts(buf);
	return 0;
}
