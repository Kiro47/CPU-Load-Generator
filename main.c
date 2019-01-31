#include <stdio>


/*
 * getNumberOfProcessors
 *
 * Returns the number of processors online or exits and returns
 * with -1 if they cannot be found.
 *
 * return long number of processors
 *
 */
long getNumberOfProcessors()
{
	long numberProcs = NULL;
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
	if (numberProcs < 1)
	{
		fprintf(stderr, "Could not find the number of processors online\n");
		fprintf(stderr, "Error Code: (%s)\n", strerror(errno));
		exit(EXIT_FAILURE);
		return -1;
	}

	return numberProcs;

#else
		fprintf(stderr, "Could not find the number of processors online\n");
		fprintf(stderr, "Error Code: (%s)\n", strerror(errno));
		exit(EXIT_FAILURE);
		return -1;
#endif

}


int main(int argc, char **argv[])
{

	/* CPU's to load */
	long numberProcs = NULL;


	if (argc == 1)
	{
		/* Defaulting to using all cores */
		numberProcs = getNumberOfProcessors();
	}
	else if (argc == 2)
	{
		/* Use specified number of processors */
	}
	else
	{
		/* Not a valid argument set */
		printf("Input syntax is invalid!\n");
		printf("Syntax: %s [Number of Processors]\n", argv[0]);
		exit(-10);
	}

}
