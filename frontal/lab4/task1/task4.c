#include <stdio.h>

int count_digits(char *str)
{
    int count = 0;

    while (*str)
    {

        if (*str >= '0' && *str <= '9')
        {
            count++;
        }
        str++;
    }

    return count;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }

    char *input_str = argv[1];
    int num_digits = count_digits(input_str);

    printf("The number of digits in the string is: %d\n", num_digits);

    return 0;
}
