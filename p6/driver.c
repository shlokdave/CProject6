/**
  @file driver.c
  @author Shlok Dave (ssdave)
  This file acts as the main source file that lets the user
  interact with a map by typing in commands. This file contains the main
  function along with several static functions that help the user
  with the commands. In general, this class contains the main function and
  truly acts as the "brain" of the entire program.
*/

#include "map.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Size of the hash table. */
#define MAP_SIZE 100

/** Command line argument for the set command. */
#define SET_COMM 3

/** Command line argument for the get command. */
#define GET_COMM 3

/** Command line argument for the remove command. */
#define REM_COMM 6

/** Command line argument for the size command. */
#define SIZE_COMM 4

/** Command line argument for the quit command. */
#define QUIT_COMM 4

/**
  This function is a helper function responsible for parsing either a key or a value from a given
  string. The function zeros out for the Value structure that is provided and then
  is able to fill the rest of it in with the parsed result. The key is represented as
  in the format of a string.
  @param value pointer to the value structure for where the result of the parsing process
  will be stored.
  @param str pointer to the string that will be passed to be able to be parsed.
  @param hasK boolean representation to check if there is a key. It is true if the function
  passes a string as a key, but false if it parses it as a value instead.
  @return the function returns an integer that will indicate the success or failure of the function.
  The function returns the result of the parsing process.
*/
static int detKeyOrVal(Value *value, char *str, bool hasK)
{
    // Check if the string or value is null.
    if (str == NULL || value == NULL)
    {
        return 0;
    }

    memset(value, 0, sizeof(Value));
    int parseResult = 0;

    // Parse as string here for the key.
    if (hasK)
    {
        parseResult = parseString(value, str);
    }
    else
    {
        // Check whether it should be a string or an integer.
        if (str[0] == '\"')
        {
            // If the value starts with a quote, parse as a string.
            parseResult = parseString(value, str);
        }
        else
        {
            // Otherwise, parse as an integer.
            parseResult = parseInteger(value, str);
        }
    }

    return parseResult;
}

/**
  This function is a helper function responsible for handling the command specified to "set" for
  the map. It breaks down the command string to get the key and the value, while parsing them and
  adding them to the map. The key can be a string or an integer and the detKeyOrVal is used to help
  parse the key and the value from the command string. For any reason, if the command string is not
  in the proper format, an error exits the map.
  @param map pointer to the map that the key and values will be set onto.
  @param comm the command represented as a string.
*/
static void commSet(Map *m, char *comm)
{
    // Splits the string into a key and a value.
    strtok(comm, " ");
    char *sepKey = strtok(NULL, " ");

    // Check if the key is present or not.
    if (!sepKey)
    {
        printf("ERROR: Key Missing\n");
        return;
    }

    // Initializing key for Val struct.
    Value key = {0};

    // Check if the Key is a string with quotes.
    if (sepKey[0] == '\"')
    {
        // Find the end of the string.
        char *endOfStr = strchr(sepKey + 1, '\"');
        if (!endOfStr)
        {
            printf("ERROR: Key is not in proper format\n");
            return;
        }

        // Perform null termination for the key.
        *(endOfStr + 1) = '\0';

        // Parse the key with the string format.
        if (!detKeyOrVal(&key, sepKey, true))
        {

            return;
        }
    }
    else
    {
        // Parsing the key for an integer format.
        if (!detKeyOrVal(&key, sepKey, false))
        {
            return;
        }
    }

    char *sepVal = strtok(NULL, "");

    // Check if the value is present or not.
    if (!sepVal)
    {
        printf("ERROR: Value Missing\n");
        key.empty(&key);
        return;
    }

    // Initializing value for Val struct.
    Value value = {0};

    // Parse as either a string or an integer format.
    if (!detKeyOrVal(&value, sepVal, false))
    {
        key.empty(&key);
        return;
    }

    // Set the parsed key and value onto the map.
    mapSet(m, &key, &value);
}

/**
  This function is a helper function that is responsible for getting the value that is
  associated with the specified key on the map. First, the function parses the command string
  to extract the key and then it gets the value that is corresponding to the key. The key can
  either be a string or an integer and it uses detKeyOrVal to parse the key that is directly
  from the command line.
  @param map pointer to the map that the value is getting retrieved from.
  @param comm pointer to the command that is represented as a string.
*/
static void commGet(Map *m, char *comm)
{
    // Splits the string into a key and a value.
    strtok(comm, " ");
    char *sepKey = strtok(NULL, " ");

    // Check if the key is present or not.
    if (!sepKey)
    {
        printf("ERROR: Key Missing\n");
        return;
    }

    // Initializing key for Val struct.
    Value key = {0};

    // Check if the Key is a string with quotes.
    if (sepKey[0] == '\"')
    {
        // Find the end of the string.
        char *endOfStr = strchr(sepKey + 1, '\"');
        if (!endOfStr)
        {
            printf("ERROR: Key is not in proper format\n");
            return;
        }
        // Perform null termination for the key.
        *(endOfStr + 1) = '\0';

        // Parse the key with the string format.
        if (!detKeyOrVal(&key, sepKey, true))
        {
            return;
        }
    }
    else
    {
        // Parse the key with the integer format.
        if (!detKeyOrVal(&key, sepKey, false))
        {
            return;
        }
    }

    // Initializing value for Val struct.
    Value *value = mapGet(m, &key);

    if (value && value->print)
    {
        value->print(value);
        printf("\n");
    }
    else
    {
        printf("Undefined\n");
    }

    key.empty(&key);
}

/**
   This function is a helper function responsible for removing the value from its
   corresponding key. The key can either be a string or an integer. The function also
   uses detKeyorVal to parse the key from the command string. For any reason, if the
   key is not found, error messages are presented towards the standard output.
   @param m pointer to the map that the value will be removed from.
   @param comm pointer to the command that is represented as a string.
*/
static void commRemove(Map *m, char *comm)
{
    // Splits the string into a key and a value.
    strtok(comm, " ");
    char *sepKey = strtok(NULL, " ");

    // Check if the key is present or not.
    if (!sepKey)
    {
        printf("ERROR: Key Missing\n");
        return;
    }

    // Initializing key for Val struct.
    Value key = {0};

    // Check if the Key is a string with quotes.
    if (sepKey[0] == '\"')
    {
        // Find the end of the string.
        char *endOfStr = strchr(sepKey + 1, '\"');
        if (!endOfStr)
        {
            printf("ERROR: Key is not in proper format\n");
            return;
        }
        *(endOfStr + 1) = '\0';

        // Parse the key with the string format.
        if (!detKeyOrVal(&key, sepKey, true))
        {
            return;
        }
    }
    else
    {
        // Parse the key with the integer format.
        if (!detKeyOrVal(&key, sepKey, false))
        {
            return;
        }
    }

    // Remove the key-value pair from the map.
    if (!mapRemove(m, &key))
    {
        printf("ERROR: Pair not\n");
    }

    key.empty(&key);
}

/**
  This is a helper function that is responsible for handling the command to get the size of
  the map. The function prints out the size of the map to the standard output.
  @param map pointer to the map for which the size command is being used from.
*/
static void commSize(Map *map)
{
    printf("%d\n", mapSize(map));
}

/**
  This function acts as the main function of the entire program. This function acts as the "brain"
  of the entire program. It is represented as the entry point of the program. The function initializes
  a map and processes commands that are directly from the standard input. The program uses the other
  helper functions to process these user commands.
  @return 0 if the function exits properly without any problems and 1 if there are any errors.
*/
int main()
{
    Map *newMap = makeMap(MAP_SIZE);

    // Check if the map cannot be created for any reason.
    if (!newMap)
    {
        fprintf(stderr, "ERROR: Map cannot be created\n");
        return 1;
    }

    // Declare variables to read the line and flag for current command.
    char *lineRead = NULL;
    bool firComm = true;

    // Traverse whiole true to process all commands
    while (true)
    {
        // Prints line to new line.
        if (!firComm)
        {
            printf("\n");
        }

        printf("cmd> ");

        // Reads the line of input from the user.
        lineRead = readLine(stdin);

        // Checks if the function of readLine is null.
        if (lineRead == NULL)
        {
            free(lineRead);
            break;
        }

        firComm = false;

        // Command entered back to the user.s
        printf("%s\n", lineRead);

        // Go through all the commands in the loop.
        if (strncmp(lineRead, "set", SET_COMM) == 0)
        {
            commSet(newMap, lineRead);
        }
        else if (strncmp(lineRead, "get", GET_COMM) == 0)
        {
            commGet(newMap, lineRead);
        }
        else if (strncmp(lineRead, "remove", REM_COMM) == 0)
        {
            commRemove(newMap, lineRead);
        }
        else if (strncmp(lineRead, "size", SIZE_COMM) == 0)
        {
            commSize(newMap);
        }
        else if (strncmp(lineRead, "quit", QUIT_COMM) == 0)
        {
            break;
        }
        else
        {
            printf("Invalid command\n");
        }
        free(lineRead);
        lineRead = NULL;
    }
    free(lineRead);

    freeMap(newMap);
    return 0;
}