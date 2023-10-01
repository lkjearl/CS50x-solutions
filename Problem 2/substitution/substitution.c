#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    //checking argv1
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //caps key
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        argv[1][i] = toupper(argv[1][i]);
    }
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must contain letters only.\n");
            return 1;
        }
        for (int j = i+1; j < n; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must not contain duplicate characters.\n");
                return 1;
            }
        }
    }

    //ciphering
    string plain = get_string("plaintext:  ");
    string cipher = plain;
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        if (isalpha(plain[i]))
        {
            if (islower(plain[i]))
            {
                char j = plain[i];
                cipher[i] = tolower(argv[1][j-97]);
            }
            if (isupper(plain[i]))
            {
                char j = plain[i];
                cipher[i] = argv[1][j-65];
            }
        }
        else
        {
            cipher[i] = plain[i];
        }
    }
    printf("ciphertext: %s\n", cipher);
    return 0;
}