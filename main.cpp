#include "diskSim.h"
#include <iostream>
using namespace std;

int parseInput (char *input, char *command, char *sargs, int *index, int *count, char *writechar)
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
    
    if (strcmp(token, "op") == 0)
        goto word2;
    
    if (strcmp(token, "cl") == 0)
        goto word2;
    
    if (strcmp(token, "rd") == 0)
        goto word2;
    
    if (strcmp(token, "wr") == 0)
        goto word2;
    
    if (strcmp(token, "sk") == 0)
        goto word2;
    
    if (strcmp(token, "de") == 0)
        goto word2;
    
    if (strcmp(token, "dr") == 0)
        goto nextEmpty;
    
    if (strcmp(token, "sv") == 0)
        goto word2;
    
    if (strcmp(token, "in") == 0)
        goto word2;
    
    cout << "error ";
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
    
    if (strcmp(command, "de") == 0) {
        strcpy(sargs, token);
        goto nextEmpty;
    }
    
    if (strcmp(command, "op") == 0) {
        strcpy(sargs, token);
        goto nextEmpty;
    }
    
    if (strcmp(command, "sv") == 0) {
        strcpy(sargs, token);
        goto nextEmpty;
    }
    
    if (strcmp(command, "in") == 0) {
        strcpy(sargs, token);
        goto nextEmpty;
    }
    
    if (strcmp(command, "rd") == 0) {
        *index = atoi(token);
        if((*index < 4) && (*index > 0))
            goto word3;
        cout << "error ";
        DEBUG("Invalid FD: read\n");
        return -1;
    }
    
    if (strcmp(command, "wr") == 0) {
        *index = atoi(token);
        if((*index < 4) && (*index > 0))
            goto word3;
        cout << "error ";
        DEBUG("Invalid FD: write\n");
        return -1;
    }
    
    if (strcmp(command, "sk") == 0) {
        *index = atoi(token);
        if((*index < 4) && (*index > 0))
            goto word3;
        cout << "error ";
        DEBUG("Invalid FD: seek\n");
        return -1;
    }
    
    if (strcmp(command, "cl") == 0) {
        *index = atoi(token);
        if((*index < 4) && (*index > 0))
            goto nextEmpty;
        cout << "error ";
        DEBUG("Invalid FD: close\n");
        return -1;
    }
    
word3:
    token = strtok(NULL, s);
    if (token == NULL) {
        cout << "error";
        DEBUG("Not enough arguments for " << command);
        return -1;
    }
    
    if(strcmp(command, "rd") == 0) {
        *count = atoi(token);
        goto nextEmpty;
    }
    
    if(strcmp(command, "sk") == 0) {
        *count = atoi(token);
        goto nextEmpty;
    }
    
    if(strcmp(command, "wr") == 0) {
        *writechar = token[0];
        goto word4;
    }
    
word4:
    token = strtok(NULL, s);
    if (token == NULL) {
        cout << "error";
        DEBUG("Not enough arguments for " << command);
        return -1;
    }
    
    if(strcmp(command, "wr") == 0) {
        *count = atoi(token);
        goto nextEmpty;
    }
    
nextEmpty:
    
    token = strtok(NULL, s);
    if (token == NULL) return 0;
    
    cout << "error ";
    DEBUG(command << "too many arguments" << endl);
    return -1;
}

int main()
{
    FileSystemSim simulation;
    
    char input[100];
    char command[30];
    int index;
    int count;
    char sargs[30];
    char writechar;
    
    DEBUG("Begin Simulation:" << endl);
    
    while (1)
    {
        /* Reset command data */
        memset((void*) input, '\0', 100);
        memset((void*) command, '\0', 30);
        memset((void*) sargs, '\0', 30);
        index = 0;
        count = 0;
        writechar = 0;
        
        /* Get line and parse */
        if (fgets(input, 100, stdin) == NULL) break;
        if (parseInput(input, command, sargs, &index, &count, &writechar)) {
            DEBUG("\n");
            continue;
        }
        
        DEBUG("Command: " << command << endl);
        DEBUG("Sargs: " << sargs << endl);
        DEBUG("Index: " << index << endl);
        DEBUG("Count: " << count << endl);
        DEBUG("WriteChar: " << writechar << endl);
        
        if (strcmp(command, "exit") == 0)
            break;
        
        if (strcmp(command, "dump") == 0)
            simulation.dump();
        
        if (strcmp(command, "op") == 0)
            simulation.open(sargs);
        
        if (strcmp(command, "cr") == 0)
            simulation.createFile(sargs);
        
        if (strcmp(command, "de") == 0)
            simulation.del(sargs);
        
        if ((strcmp(command, "cl") == 0) && (index != 0))
            simulation.close(index);
        
        if (strcmp(command, "rd") == 0)
            simulation.read(index, count);
        
        if (strcmp(command, "wr") == 0)
            simulation.write(index, writechar, count);
        
        if (strcmp(command, "sk") == 0)
            simulation.seek(index, count);
        
        if (strcmp(command, "dr") == 0)
            simulation.printFiles();
        
        if (strcmp(command, "sv") == 0)
            simulation.save(sargs);
        
        if (strcmp(command, "in") == 0)
            simulation.load(sargs);
        
    }

	return 0;
}
