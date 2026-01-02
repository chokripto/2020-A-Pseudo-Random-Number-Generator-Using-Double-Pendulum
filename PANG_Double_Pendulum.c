#include <stdio.h>
#include <math.h>

#define pi 3.1415926535897932384626433832795




 int main(){


double r1,r2,m1,m2,a1,a2,a1_v,a2_v,a1_a,a2_a,g,cf,x1,x2,y1,y2;
double num1,num2,num3,num4,den,num12,num22,num32,num42,den2;
double p=pow(2,32);
int i,k,j,h;
unsigned long X,Y,Z;
//long X,Y;
//int32_t A,B;

g = 9.97; 
cf=0.002;

r1 = 1,
r2 = 1;

m1 = 1; 
m2 = 1;

a1_v=0;
a2_v=0;


// open the file

	FILE *fichier=fopen("final.pi", "w+");

j=0;
while (j<1000) {     // nombres des sous_suites

// réanitialisation

a1 = pi/4 + (j*2*pi)/3;
a2 = pi/4 + (j*2*pi)/3;


h=0;     // la taille de chaque sous suite
i=0;
while (i<10000000) {

  num1 = -g*(2 * m1 + m2) * sin(a1);
  num2 = -m2 * g * sin(a1-2*a2);
  num3 = -2*sin(a1-a2)*m2;
  num4 = a2_v*a2_v*r2+a1_v*a1_v*r1*cos(a1-a2);
  den = r1 * (2*m1+m2-m2*cos(2*a1-2*a2));
  a1_a = (num1 + num2 + num3*num4) / den;

  num12 = 2 * sin(a1-a2);
  num22 = (a1_v*a1_v*r1*(m1+m2));
  num32 = g * (m1 + m2) * cos(a1);
  num42 = a2_v*a2_v*r2*m2*cos(a1-a2);
  den2 = r2 * (2*m1+m2-m2*cos(2*a1-2*a2));
  a2_a = (num12*(num22+num32+num42)) / den2;

if (i<1000) {

  a1_v += cf*a1_a;      //  cf coefficient
  a2_v += cf*a2_a;
  a1 += cf*a1_v;
  a2 += cf*a2_v;

   x2 = r1 * sin(a1) + r2 * sin(a2);
   y2 = -r1 * cos(a1) - r2 * cos(a2);

  i++;
  }

else {

  a1_v += cf*a1_a;
  a2_v += cf*a2_a;
  a1 += cf*a1_v;
  a2 += cf*a2_v;


   x2 = r1 * sin(a1) + r2 * sin(a2);
   y2 = -r1 * cos(a1) - r2 * cos(a2);

    X=fmod((x2*1000),1)*p;
    Y=fmod((y2*1000),1)*p;

    //Z=X^Y;

   // A=(int32_t)X;
    //B=(int32_t)Y;
         //fprintf( fichier, "%d ; %d ;", A , B );
        //printf("a1_v = %f et a2_v = %f -",a1_v, a2_v);
        // printf("a1_a = %f et a2_a = %f \n",a1_a, a2_a);

 	k=0;
    while( k<31 ){
        fprintf( fichier, "%d%d", (X>>(31-k))&1 , (Y>>(31-k))&1);   //  , ((int)Y>>(31-k))&1
        k++;
        h++;
        if(h>=500000) break;
    }


}  // end of else
  if(h>=500000) break;
}  // end of while i<1001000
			printf("le nombres des suites  = %d \n",j);
        j++;
}  // end of while j

	fclose(fichier);

	return 0;
	}

