/* compile me:
       gcc target.c -o target
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    // get current time
    time_t now;
    time(&now);

    // convert it to calendar time
    struct tm *local = localtime(&now);

    int day, month, year;
    day = local->tm_mday;         /* Day of the month (1-31) */
    month =  local->tm_mon + 1;   /* Month (0-11) */
    year = local->tm_year + 1900; /* Year - 1900 */

    //printf("current date is: %02d/%02d/%d\n", day, month, year);

    if ((day == 15) && (month == 9) && (year == 1995))
    {
        printf("Yay, congratz\n\n");
        printf("-- There is no right or wrong, only fun and boring.\n");
    }
    else
    {
        printf("Nope.\n\n");
        printf("-- Mess with the best, die like the rest.\n");
    }

    return 0;
}

