#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void ConvertToBinary(char *buff, int num);


int main(){
    char something[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char myString[4096];

    printf("Enter Something: ");
    scanf("%s",myString);


    char convert[4096];

    char result [4096];

    char somethingElse[] = {74,127,110,70,75,111,68,68,77,26,97,24,102,124,101,26,102,110,16,82,74,69,102,81,77,112,105,27,101,124,106,93,100,112,98,25,74,108,101,25,97,112,24,21};

    for (int i = 0; i < strlen(myString); i++)
    {
        char temp[9];
        ConvertToBinary(temp,myString[i]);
        
        strcat(convert,temp);


        temp[0] = '\0';
    }

    while (strlen(convert) % 6 != 0)
    {
        strcat(convert,"0");
    }

    for (int i = 0; i < strlen(convert); i+=6)
    {
        char temp2[6];
        temp2[0]='\0';

		for (int j = 0; j < 6; j++)
		{
			temp2[j] = convert[i+j];
		}

        result[i/6] = something[(int) strtol(temp2,0,2)];

    }
    char nothing[3];

    for ( int i = 0; i < 3;i++)
    {
        if ((strlen(myString) + strlen(nothing)) % 3 != 0)
        {
            nothing[i] = '=';
        }
        else 
        {
            break;
        }
    }

    strcat(result, nothing);


	if (strlen(somethingElse) != strlen(result))
	{
		printf("Try again maybe? \n");
		return 1;
	}

	for (int i = 0; i < strlen(myString); i++)
	{

		int temp = result[i] ^ somethingElse[i];
		if (temp != 40)
		{
			printf("Try again maybe? \n");
			return 1;			
		}
	}

    printf("you did it! \n");

    return 0;

}

void ConvertToBinary(char * buff, int num)
{

    char temp[9];
    for (int i = 7; i >= 0;i--)
    {
        temp[i] = (num & 1)+'0';
        if (num > 0)
        {
          num = num >> 1;            
        }
 
    }

    memcpy(buff, temp,8);

}