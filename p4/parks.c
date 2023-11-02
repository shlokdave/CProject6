/**
  @file parks.c
  @author Shlok Dave (ssdave)
  This file contains the main function and other helper functions that help
  with the program. It is the top-level component, containing the main function
  and to parse the user commands. This file contains the information about both
  of the other components and helps call the other functions that are part of those
  components. Overall, this file contains the main function of the program and acts
  as the "brain" of the entire program.
*/

#include "catalog.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/** Static array used to store the park structure pointers while representing a trip of parks. */
static Park *newTrip[100];

/** Static variable represented as an integer to keep count of the total parks that are part of the trip. */
static int countOfTrip = 0;

/** Static variable represented as a string to hold the last command that is executed. */
static char executeCurrentCommand[50];

/**
  The function acts as a helper function to search the trip array for a park that is
  represented by a specific id. With a park being found with the id, the function removes
  the park from the array while shifting all of the other parks to the left in the array.
  If the park is not found, an error message is printed.
  @param id the id of the park that is searched in the trip array.
*/
static void removeFromTrip(int id)
{
  bool checkTrip = false;

  // Check to see if there are trips
  if (countOfTrip <= 0)
  {
    fprintf(stderr, "There are no trips.\n");
  }
  else
  {
    // Searches the array of the trips
    for (int idx = 0; idx < countOfTrip; idx++)
    {
      // Check for a trip to have the same id as the id needed
      if (newTrip[idx]->id == id)
      {
        checkTrip = true;

        // Elements are shifted in the array to the left
        for (int idx2 = idx; idx2 < countOfTrip - 1; idx2++)
        {
          newTrip[idx2] = newTrip[idx2 + 1];
        }

        countOfTrip--;
        break;
      }
    }

    // Error checking to see if the id of the park is not found in the trip
    if (!checkTrip)
    {
      fprintf(stderr, "Id of the park was not found in the trip.\n");
    }
  }
}

/**
  The function is responsible for acting as a helper function to provide the catalog
  for the park with its id. The function helps add a park to the catalog. The size of the
  trip is increased once the park is added to the catalog. If the trip is full,
  an error message will be presented to display the full trip size. If the park is not
  found, it will also display an error message.
  @param catalog points to the catalog which contains all of the parks.
  @param id the park id that is being checked to see if it can be added to the catalog.
  @return the park name that is added to the trip or null if it cannot be added to the trip.
*/
static const char *addToTrip(Catalog *catalog, int id)
{
  // Simpele check to see if the catalog is null
  if (catalog == NULL || catalog->parks == NULL)
  {
    fprintf(stderr, "Invalid catalog provided.\n");
  }

  // Searches to find the park within the catalog that needs to be added
  for (int idx = 0; idx < catalog->count; idx++)
  {
    // Checks if the catalog is not null and if the park matches the required id
    if (catalog->parks[idx] != NULL && catalog->parks[idx]->id == id)
    {
      // Simple check to see if the trip has space in it
      if (countOfTrip < 100)
      {
        newTrip[countOfTrip] = catalog->parks[idx];
        countOfTrip++;
        return "Park added.";
      }
      else
      {
        fprintf(stderr, "Trip is full.\n");
        return "Trip is full";
      }
    }
  }

  fprintf(stderr, "Park with ID %d not found in the catalog.\n", id);
  return NULL;
}

/**
  The function is responsible for acting as a helper function to go through the park array
  each of the id and name of the park. The function also calculates the distance from the
  start of the trip to the end. The distance function is used here that was implemented in
  the catalog component.
*/
static void helperPrintEntireTrip()
{
  double d = 0.0;

  // Prints the header of the trip details
  printf("cmd> trip\nID  Name                                     Distance\n");

  // Loop that iterates through each trip to calculate distance
  for (int idx = 0; idx < countOfTrip; idx++)
  {
    // Check to calculate every trip distance besides the first one
    if (idx > 0)
    {
      d += distance(newTrip[idx - 1], newTrip[idx]);
    }

    // Print the details of the trip that is currently being checked
    printf("%-3d %-40s %8.1f\n", newTrip[idx]->id, newTrip[idx]->name, d);
  }
  printf("\n");
}

/**
  The function is responsible for acting as a helper function to open the file by its specific
  file name and load all the park data. Once it loads the data, it is implemented to read the data
  using the readParks function which is implemented in the catalog component.
  @param catalog points to the catalog that the function is reading from.
  @param name the name of the file that is being opened.
*/
static void loadFile(Catalog *catalog, const char *name)
{
  readParks(name, catalog);
}

/**
  The function is responsible for acting as a helper function to help always return true
  no matter the input. It is designed as a placeholder that helps return true everytime the
  function is called.
  @param park points to the struct of the park.
  @param parkString points to the string that is recognized as a character array.
  @return true for every time the function is called no matter the input.
*/
static bool helperTrue(Park const *park, char const *parkString)
{
  return true;
}

/**
  The function is responsible for acting as a helper function to help check if the park is in the
  correct county based off of the county name. The function compares each token of the counties to the
  given park.
  @param park points to the park struct that contains information about each park.
  @param name points to the name of the county which is represented as a string.
  @return true if the park is matched with the county based off of the name of the county. Returns false
  if the park is not in the county.
*/
static bool checkParkInCounty(Park const *park, char const *name)
{
  // Size of the string that is being checked
  size_t sizeOfCounty = strlen(park->numCounties);
  char newCounty[sizeOfCounty + 1];

  // Copy the string of the counties
  strncpy(newCounty, park->numCounties, sizeOfCounty);
  newCounty[sizeOfCounty] = '\0';

  // Delimiter used to help separate the commas
  char *separateCommaTok = strtok(newCounty, ",");
  while (separateCommaTok != NULL)
  {
    if (strcmp(separateCommaTok, name) == 0)
    {
      return true;
    }
    separateCommaTok = strtok(NULL, ",");
  }
  return false;
}

/**
  The function is responsible for acting as the main function of the entire program. This function
  is the starting point for the program to be executed properly. The main program helps control the
  program execution by correctly directing calls that are made by other functions in the rest of the
  program. In all, the main function allows the user to be able to see the catalog and the parks through
  different commands that are entered. Some of the functions that the program does include adding a parks to
  a trip, removing parks from a trip, and loading files that contain parks.
  @param argc the command-line arguments that are entered by the user represented as an integer.
  @param argv array of pointers to the arrays of character objects in the program.
  @return 0 if the program is run through successfully with no error messages being printed. Program
  returns 1 if the number of command line arguments are incorrect or if there is an error that happened in
  the program with the parks.
*/
int main(int argc, char *argv[])
{
  // Simple check to see if the command-line arguments is less than 2
  if (argc < 2)
  {
    fprintf(stderr, "usage: parks <park-file>*\n");
    return 1;
  }

  Catalog *newCatalogObj = makeCatalog();

  // Parks are loaded from the file
  for (int idx = 1; idx < argc; idx++)
  {
    loadFile(newCatalogObj, argv[idx]);
  }

  bool quitOrExitCommand = false;

  // Loop to have the program running until EOF
  while (true)
  {
    char *userInput = readLine(stdin);
    if (!userInput)
    {
      printf("cmd> ");
      break;
    }

    userInput[strcspn(userInput, "\n")] = 0;

    // Condition to see if user input has the words "exit" or "quit"
    if (strcmp(userInput, "exit") == 0 || strcmp(userInput, "quit") == 0)
    {
      quitOrExitCommand = true;
      free(userInput);
      break;
    }

    // Condition to see if user input has the words "list parks", while printing out contents
    else if (strcmp(userInput, "list parks") == 0)
    {
      strcpy(executeCurrentCommand, "list parks");
      printf("cmd> %s\nID  Name                                          Lat      Lon Counties\n", executeCurrentCommand);
      listParks(newCatalogObj, NULL, NULL);
    }

    // Condition to see if user input has the words "list names", while printing out contents
    else if (strcmp(userInput, "list names") == 0)
    {
      strcpy(executeCurrentCommand, "list names");
      printf("cmd> %s\nID  Name                                          Lat      Lon Counties\n", executeCurrentCommand);
      listParks(newCatalogObj, helperTrue, NULL);
    }

    // Condition to see if user input has the words "list county", while printing out contents
    else if (strncmp(userInput, "list county ", 12) == 0)
    {
      strcpy(executeCurrentCommand, userInput);
      char *county = userInput + 12;
      printf("cmd> %s\nID  Name                                          Lat      Lon Counties\n", executeCurrentCommand);
      listParks(newCatalogObj, checkParkInCounty, county);
    }

    // Condition to see if user input has the words "add", while printing out contents
    else if (strncmp(userInput, "add ", 4) == 0)
    {
      strcpy(executeCurrentCommand, userInput);
      char *addPark = userInput + 4;
      int id = atoi(addPark);

      // Checks for the id of the park to be a suitable id
      if (id == 0 && strcmp(addPark, "0") != 0)
      {
        fprintf(stderr, "Id of park is invalid.\n");
      }
      else
      {
        const char *parkName = addToTrip(newCatalogObj, id);
        if (parkName != NULL)
        {
          printf("cmd> %s\n\n", userInput); // Print the add command confirmation
        }
      }
    }

    // Condition to see if user input has the words "trip", while printing out contents
    else if (strcmp(userInput, "trip") == 0)
    {
      strcpy(executeCurrentCommand, "trip");
      helperPrintEntireTrip();
    }

    // Condition to see if user input has the words "remove", while printing out contents
    else if (strncmp(userInput, "remove ", 7) == 0)
    {
      strcpy(executeCurrentCommand, userInput);
      char *removePark = userInput + 7;
      int id = atoi(removePark);

      // Checks for the id of the park to be a suitable id
      if (id == 0 && strcmp(removePark, "0") != 0)
      {
        fprintf(stderr, "Invalid park ID. Please enter a valid integer.\n");
      }
      else
      {
        removeFromTrip(id);
        printf("cmd> %s\n\n", userInput);
      }
    }

    // All of the memory is freed from the input
    free(userInput);
  }

  // Condition to check if the quit or exit command was entered by the user
  if (quitOrExitCommand)
  {
    printf("cmd> quit\n"); // Print quit message here, after the loop has ended
  }

  // All of the memory is freed from the catalog
  freeCatalog(newCatalogObj);
  return 0;
}