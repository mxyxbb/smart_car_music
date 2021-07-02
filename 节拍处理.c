#include <stdio.h>
#include <math.h>

#define note_lenth 8


char hello[]="NOTE_G5,NOTE_F5,NOTE_E5,NOTE_D5,NOTE_E5,NOTE_C5,NOTE_C5,NOTE_C5,NOTE_C5,NOTE_G4,NOTE_G4,\
NOTE_C5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_B4,NOTE_G4,NOTE_A4,NOTE_A4,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_A5,NOTE_A5,\
NOTE_G5,NOTE_C5,NOTE_F5,NOTE_E5,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_C5,NOTE_B4,NOTE_C5,\
NOTE_A4,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_F5,NOTE_E5,NOTE_D5,NOTE_E5,NOTE_C5,0";
int hello_lenth=sizeof(hello)/sizeof(char);
char dura[]={3,1,2,4,2,4,3,1,4,3,1,
4,4,4,3,1,4,4,2,4,2,12,3,1,4,4,4,3,1,
4,4,2,4,2,12,2,2,8,4,3,1,4,4,4,4,
12,2,2,8,4,3,1,4,4,2,4,2,12,4};
int dura_lenth=sizeof(dura)/sizeof(char);
int dura_sum=0;
char result[8000];

char a[note_lenth];
int count=0;
int main(void) { 
    for(int i=0;i<dura_lenth;i++)
    {
        dura_sum+=dura[i];
    }
    printf("hello_lenth:%d,dura_lenth:%d\n\r",hello_lenth,dura_lenth);
    printf("dura_sum:%d\n\r",dura_sum);
    
    for(int i=0;i<dura_lenth;i++)
    {
        for(int j=0;j<note_lenth;j++)
        {
            a[j]=hello[i*note_lenth+j];
        }
        printf("note to be copy:\"%s\",copy times:%d\n\r",a,dura[i]);
        for(int k=0;k<dura[i];k++)
        {
            for(int j=0;j<note_lenth;j++)
            {
                result[(count)*note_lenth+j]=a[j];
            } 
            count++;
        }
        printf("note count:%d\n\r",count);
        printf("result now:%s\n\r",result);
    }
    for(int i=0;i<dura_sum;i++)
    {
        printf("%c,",'1');
    }
	return 0;
}