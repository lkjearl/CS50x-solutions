#include <cs50.h>
#include <stdio.h>
#include <math.h>


int main(void)
{
    //get number from user
    long num = get_long("Number: ");

    // get digit length
    int len = floor(log10(num)) + 1;

    // convert to array
    long numbackup1 = num;
    int numbers[len];
    int n = len-1;
    while (numbackup1 > 0)
    {
        int tempnum = numbackup1 % 10;
        numbackup1 /= 10;
        numbers[n] = tempnum;
        n--;
    }
    // checksum
    // declare variable to store value, temp is value that needs to *2
    long numbackup2 = num;
    int sum = 0;
    int pos = 0;
    while (numbackup2 != 0)
    {
        if(pos % 2 != 0)
        {
            int temp = 2 * (numbackup2 %10);
            if (temp > 9)
            {
                // mod(%) for remainder, divide(/) for the value in tenth place
                sum += (temp % 10) + (temp / 10);
            }
            else {
                sum += temp;
            }
        }
        else
        {
            sum += numbackup2 % 10;
        }
        numbackup2 = numbackup2 / 10;
        pos++;
    }

    if (sum % 10 == 0)
    {
        if (len == 15 && numbers[0] == 3 && (numbers[1] == 4 || numbers[1] == 7))
        {
            printf("AMEX\n");
        }
        else if (len == 16 && numbers[0] == 5 && numbers[1] >= 1 && numbers[1] <= 5)
        {
            printf("MASTERCARD\n");
        }
        else if ((len == 13 || len == 16) && numbers[0] == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}