#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void printHelp()
{
	printf("\nHELP MENU: \n");
	printf("\"init\"                     - Restarts scheduler and recreates init process\n");
	printf("\"cr\" <name> <priority>     - Creates process\n");
	printf("\"print\"                    - Prints Processes\n");
	printf("\"printName\" <name>         - Print Process by name\n");
	printf("\"exit\"                     - Quit\n");
	printf("\"to\"                       - timeout\n");
	printf("\"de\" <name>                - destroy process by name\n");
	printf("\"req\" <resource> <num>     - request resource\n");
	printf("\"rel\" <resource> <num>     - release resource\n\n");
}

int parseInput(char *input, char *command, char *name, int *num)
{
	if (input == NULL || command == NULL || name == NULL) {
		printf("error ");
		verbose_print("HUH?\n");
		return -1;
	}

	/* Get rid of newline character to make our lives easier */	
	input[strlen(input) - 1] = '\0';
    
    if (strchr(input, '\r') != NULL)
        input[strlen(input) - 1] = '\0';

	if (input[0] == '\0') {
		verbose_print("No command entered\n");
		return -1;
	}

	const char s[2] = " ";
	char *token;

	/* get first word */
	token = strtok(input, s);
	strcpy(command, token);
	
	/* error checking! */
	if (strcmp(token, "help") == 0)
		goto nextEmpty;

	if (strcmp(token, "printName") == 0)
		goto word2;

	if (strcmp(token, "print") == 0)
		goto nextEmpty;

	if (strcmp(token, "init") == 0)
		goto nextEmpty;

	if (strcmp(token, "exit") == 0)
		goto nextEmpty;

	if (strcmp(token, "to") == 0)
		goto nextEmpty;

	if (strcmp(token, "cr") == 0)
		goto word2;

	if (strcmp(token, "de") == 0)
		goto word2;
	
	if (strcmp(token, "req") == 0)
		goto word2;

	if (strcmp(token, "rel") == 0)
		goto word2;

	printf("error ");
	verbose_print("\'%s\': not a valid command!\n", command);
	return -1;

word2:
	/* Getting the next word */
	token = strtok(NULL, s);
	if (token == NULL) {
		printf("error ");
		verbose_print("\'%s\': not enough arguments\n", command);
		return -1;
	}
	strcpy(name, token);
	
	if (strcmp(command, "de") == 0)
		goto nextEmpty;

	if (strcmp(command, "printName") == 0)
		goto nextEmpty;

	if (strcmp(command, "cr") == 0)
		goto word3;
	
	if (strcmp(command, "req") == 0)
		goto word3;

	if (strcmp(command, "rel") == 0)
		goto word3;

word3:
	token = strtok(NULL, s);
	if (token == NULL) {
		printf("error ");
		verbose_print("\'%s\' not enough arguments\n", command);
		return -1;
	}
	
	if ((token[1] == '\0') && isdigit(token[0])) {
		*num = atoi(token);
		goto nextEmpty;
	}
	
	printf("error ");
	verbose_print("\'%s\' not a valid argument (must be single digit integer)\n", token);
	return -1;


nextEmpty:
	/* Next token should be null */
	token = strtok(NULL, s);
	if (token == NULL) return 0;

	printf("error ");
	verbose_print("\'%s\' too many arguments\n", command);
	return -1;
}


int main()
{	
	/* Create state structure */
	State *current;

	verbose_print("Begin scheduler simulation\n");

	/* Populate state structure */
	current = setup();
	verbose_print("State structure created\n");

	/* Create init process */
	if(createInitProcess(current)) {
		return -1;
	}
	verbose_print("Init process created\n");
	verbose_print("Simulation ready\n");	

	/* Start simulation */
	char input[100];
	char command[30];
	int num;
	char name[30];

	while (1) {
		//verbose_print("Commands: exit, help, print, printName <name>,\n");
		//verbose_print("cr <name> <priority>, de <name>, to, init,\n");
		//verbose_print("req <resource> <num>, rel <resource> <num>\n");
		verbose_print("\nEnter Command: ");
		
		/* Recieve command */
		if(fgets(input, 100, stdin) == NULL) break;

		/* Parse, if error skip iteration... */
		if (parseInput(input, command, name, &num)) {
			verbose_print("\n");
			continue;
		}
		
		verbose_print("\nCommand: %s\n", command);
		verbose_print("Name: %s\n", name);
		verbose_print("Num: %d\n", num);
		if (strcmp(command, "help") == 0)
			printHelp();		
		else if (strcmp(command, "rel") == 0)
			releaseResource(current, name, num);
		else if (strcmp(command, "req") == 0)
			requestResource(current, name, num);
		else if (strcmp(command, "init") == 0) {
			printf("\n");
			createInitProcess(current);
		}
		else if (strcmp(command, "print") == 0)
			printAll(current);
		else if (strcmp(command, "printName") == 0)
			printPCB(current, name);
		else if (strcmp(command, "cr") == 0) 
			createProcess(current, name, num);
		else if (strcmp(command, "de") == 0)
			destroy(current, name);
		else if (strcmp(command, "to") == 0)
			timeout(current);
		else if (strcmp(command, "exit") == 0)
			break;	

		memset((void*) input, '\0', 100);
		memset((void*) name, '\0', 30);
		memset((void*) command, '\0',30);
		num = 0;
		fflush(stdout);
	}
	
	verbose_print("Exiting....\n");
	verbose_print("Cleaning Up\n");
	free(current);

	return 0;
}
