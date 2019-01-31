#ifdef _WIN32
#define WIN32_GARBAGE
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 * getNumberOfProcessors
 *
 * Returns the number of processors online or exits and returns
 * with -1 if they cannot be found.
 *
 * returns:
 *      long number of processors
 *
 */
long getNumberOfProcessors()
{
	long numberProcs = -1.0;
	/* Checking magic to deal with windows */
#ifdef _WIN32
#ifndef _SC_NPROCESSORS_ONLN
	SYSTEM_INFO info;
	GetSystemInfo(&info);
#define sysconf(a) info.dwNumberOfProcessors
#define _SC_NPROCESSORS_ONLN
#endif
#endif
#ifdef _SC_NPROCESSORS_ONLN
	numberProcs = sysconf(_SC_NPROCESSORS_ONLN);
    printf("numberProcs: %lu\n", numberProcs);
    printf("numberProcsMax %lu\n", sysconf(_SC_NPROCESSORS_CONF));

	if (numberProcs < 1)
	{
		fprintf(stderr, "Could not find the number of processors online\n");
		fprintf(stderr, "Error Code: (%s)\n", strerror(errno));
		exit(-1);
	}

	return numberProcs;

#else
		fprintf(stderr, "Could not find the number of processors online\n");
		fprintf(stderr, "Error Code: (%s)\n", strerror(errno));
		exit(-1);
#endif

}


/*
 * Check is a string is a valid number
 *
 *
 * Returns:
 *      -1 if it is a negative number,
 *      0 if it is a positive number,
 *      1 if it is not a number.
 *
 */
int isNumeric(char *number[])
{
    int i;
    /* Checking for negative Numbers */
    if (*number[0] == '-')
    {
        return -1;
    }
    /* Iterate through the entire number */

    i = 0;
    for (; number[i] != 0; i++)
    {

        if (!isdigit(*number[i]))
        {
            return 1;
        }
    }
    return 0;
}

/*
 * Prints a message when invalid syntax is used
 * with an example of valid syntax.
 */
void invalidSyntax(char *progname[])
{
		printf("Input syntax is invalid!\n");
		printf("Syntax: %s [Number of Processors]\n", *progname);
		exit(-2);
}


/*
 * Generates heavy CPU load on a thread
 */
void generateLoad()
{

    pthread_t thread_id;
    thread_id = pthread_self();

    int i;
    i = 0;
    for (; i < 20; i++)
    {
        printf("Thead: %d ; Count: %d \n", thread_id, i);
    }

}

void spawnUnixLoad(long numberProcs)
{
    /* Extra thread for text output */
	pthread_t threads[numberProcs + 1];

    int i;
    i = 0;

    int thread_id;

    for (; i < numberProcs; i++)
    {
        printf("Spawning Thread %d\n", i);
        thread_id = pthread_create(&threads[i], NULL,
                generateLoad, NULL);
        if (thread_id)
        {
            printf("failed to create thread\n");
            exit(-5);
        }
    }

    int j = 0;
    while ()
    {
        printf("I'm the main thread!\n");
        sleep(2);
        j++;
    }


}
/*
* Main method to execute program
*
* Syntax:
*       ./<program name>  [number of processors]
*
* Exit Codes:
*
*  0: Success
* -1: Failure to find number of processors
* -2: Given processor argument is not a valid number
* -3: Threading failed for some unknown reason
*/
int main(int argc, char *argv[])
{

	/* CPU's to load */
	long numberProcs = -1.0;
    long maxNumberProcs = getNumberOfProcessors();


    /* DEBUG */
    printf("argc: %d\n", argc);
    printf("argv: %s\n", argv[1]);

	if (argc == 1)
	{
		/* Defaulting to using all cores */
		numberProcs = maxNumberProcs;
	}
	else if (argc == 2)
    {
        /* Use specified number of processors */
       if (isNumeric(&argv[1]) == 0)
        {
            numberProcs = atol(argv[1]);

            if (numberProcs > maxNumberProcs)
            {
                printf("Number specified is more than avaliable!\n");
                printf("Using maximum avaliable instead.\n");
                numberProcs = maxNumberProcs;
            }
        }
       else
       {
           invalidSyntax(&argv[0]);
       }

	}
	else
	{
		/* Not a valid argument set */
        invalidSyntax(&argv[0]);
	}

    /* Begin spawning load and message thread */
    printf("Spawning Load with %lu processors\n", numberProcs);

	/* Checking magic to deal with windows */
#ifdef _WIN32
#ifndef _W_LOAD_THREADING
	/* TODO: Windows threading */
#define _W_LOAD_THREADING
#endif
#endif
#ifdef _SC_NPROCESSORS_ONLN
	/* TODO: Unix threading */
	spawnUnixLoad(numberProcs);


#else
	printf("Threading borked, please come back later\n");
		exit(-3);
#endif




    return 0;
}
