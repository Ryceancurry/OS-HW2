#include "diskSim.h"
#include <iostream>
using namespace std;

int parseInput (char *input, char *command, char *sargs, int *index, int *skPos, char *writechar)
{
    if (input == NULL || command == NULL || sargs == NULL) {
        DEBUG("HUH?");
        return -1;
    }
    input[strlen(input) - 1] = '\0';
    
    if (strchr(input, '\r') != NULL)
        input[strlen(input) - 1] = '\0';
    
    if (input[0] == '\0') {
        DEBUG("No Command entered" << endl);
        return -1;
    }
    
    const char s[2] = " ";
    char *token;
    
    token = strtok(input, s);
    strcpy(command, token);
    
    if (strcmp(token, "exit") == 0)
        goto nextEmpty;
    
    if (strcmp(token, "dump") == 0)
        goto nextEmpty;
    
    if (strcmp(token, "cr") == 0)
        goto word2;
    
    cout << "error";
    DEBUG(token << "not a valid command");
    return -1;
    
word2:
    token = strtok(NULL, s);
    if (token == NULL) {
        cout << "error";
        DEBUG("Not enough arguments for " << command);
        return -1;
    }
    
    if (strcmp(command, "cr") == 0) {
        strcpy(sargs, token);
        goto nextEmpty;
    }
    
nextEmpty:
    
    token = strtok(NULL, s);
    if (token == NULL) return 0;
    
    cout << "error";
    DEBUG(command << "too many arguments" << endl);
    return -1;
}

int main()
{
    FileSystemSim simulation;
    
    char input[100];
    char command[30];
    int index;
    char sargs[30];
    int skPos;
    char writechar;
    
    DEBUG("Begin Simulation:" << endl);
    
    while (1)
    {
        /* Reset command data */
        memset((void*) input, '\0', 100);
        memset((void*) command, '\0', 30);
        memset((void*) sargs, '\0', 30);
        index = 0;
        skPos = 0;
        writechar = 0;
        
        /* Get line and parse */
        if (fgets(input, 100, stdin) == NULL) break;
        if (parseInput(input, command, sargs, &index, &skPos, &writechar)) {
            DEBUG("\n");
            continue;
        }
        
        DEBUG("Command: " << command << endl);
        DEBUG("Sargs: " << sargs << endl);
        DEBUG("Index: " << index << endl);
        DEBUG("skPos: " << skPos << endl);
        DEBUG("WriteChar: " << writechar << endl);
        
        if (strcmp(command, "exit") == 0)
            break;
        
        if (strcmp(command, "dump") == 0)
            simulation.dump();
        
        if (strcmp(command, "cr") == 0)
            simulation.createFile(sargs);
        
        
    }

	return 0;
}
