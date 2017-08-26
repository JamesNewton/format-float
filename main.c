#include <stdio.h>

/*
Because lcd and serial don't support printf, and its very costly, and all we need
is simple formating with a certain number of digits and precision, this ftoa is enough.
If digits is negative, it will pad left.
*/
#define  BUF_LEN 20
char buf[BUF_LEN]; //need a buffer to hold formatted strings to send to LCD
#define absq(amt) ((amt)<0?0-(amt):(amt))

int ftoa(	char * str, 	//buffer to hold result.
	float f,    	//input value
	char digits,	//total number of digits
	char precision	//digits right of decimal
	) {
long pow10[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
char i=0,k,l=0;
long a,c;
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
  a=(int)f;	// extracting whole number
  f-=a;	// extracting decimal part
  k = digits;
  // number of digits in whole number
  while(k>0) { //note this doesn't happen if digits was negative: Padding.
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
  for(l=absq(k);l>=0;l--){
    c = pow10[l];	//get the next power
    b = a/c;	//get the next digit
    if (b>0) pad='0';	//stop padding after first digit.
    str[i++]=(l&&!b?pad:b+48); //digit or pad, l&& adds leading zero for fractions
    a%=c;   	//modulo by power
    }
  if (precision) {str[i++] = decimal;};
/* extracting decimal digits till precision */
  if (0>precision) {k=0; precision=abs(precision);}
  for(l=0;l<precision;l++) {
    f*=10.0;
    b = (int)(f+0.5); //math floor so round half way
    str[i++]=b+48; //48 is ASCII 0
    f-=(float)b;
    if (!k && 0==f) { break; } //nothing left, save chars.
    //won't work if there are any floating point errors.
    }
  str[i]='\0'; //null terminate the buffer
  return i;
  }


int main(void) {
	ftoa(buf, .12345, 2, 4);
	puts(buf);
	ftoa(buf, -4404.444, -8, 3);
	puts(buf);
	ftoa(buf, 4000123.1014, 6, 3);
	puts(buf);
	return 0;
}
