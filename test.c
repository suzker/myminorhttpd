#include <stdio.h>
#include <time.h>
#include <string.h>

#define MST (-7)
#define UTC (0)
#define CCT (+8)

void _int_mon2int_(int * monint, char monstr[]){
   switch (*monint){
       case 0:
            strcpy(monstr, "Jan");
            break;
       case 1:
            strcpy(monstr, "Feb");
            break;
       case 2:
            strcpy(monstr, "Mar");
            break;
       case 3:
            strcpy(monstr, "Apr");
            break;
       case 4:
            strcpy(monstr, "May");
            break;
       case 5:
            strcpy(monstr, "Jun");
            break;
       case 6:
            strcpy(monstr, "Jul");
            break;
       case 7:
            strcpy(monstr, "Aug");
            break;
       case 8:
            strcpy(monstr, "Sep");
            break;
       case 9:
            strcpy(monstr, "Oct");
            break;
       case 10:
            strcpy(monstr, "Nov");
            break;
       case 11:
            strcpy(monstr, "Dec");
            break;
       defaut:
            strcpy(monstr, "n/a");
            break;
   } 
   return;
}

int main ()
{
      time_t rawtime;
        struct tm * ptm;

          time ( &rawtime );

            ptm = gmtime ( &rawtime );

              puts ("Current time around the World:");
                printf ("Phoenix, AZ (U.S.) :  %02d:%02d\n", (ptm->tm_hour+MST)%24, ptm->tm_min);
                  printf ("Reykjavik (Iceland) : %2d:%02d\n", (ptm->tm_hour+UTC)%24, ptm->tm_min);
                    printf ("Beijing (China) :     %2d:%02d\n", (ptm->tm_hour+CCT)%24, ptm->tm_min);
                    char monstr[4];
                    _int_mon2int_(&(ptm->tm_mon), monstr);
                                        printf("[%02d/%s/%d:%02d:%02d:%02d - 0000]\n", ptm->tm_mday, monstr, 1900+ptm->tm_year, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
            
            
            
            printf("=======================\n");          
            char 
                        return 0;
}
