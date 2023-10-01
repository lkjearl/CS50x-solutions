#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //prompting them for a positive integer between 1 and 8, inclusive; re-prompt until right
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    //create pyramid
    for (int rows = 0; rows < height; rows++)
    {
        for (int spaces = 0; spaces < height -rows-1; spaces++)
        {
            printf(" ");
        }
        for (int hash = height - rows - 1; hash < height; hash++)
        {
            printf("#");
        }
        printf("  ");
        for (int hash = height - rows - 1; hash < height; hash++)
        {
            printf("#");
        }
        printf("\n");
    }


}