/**
  @file catalog.c
  @author Shlok Dave (ssdave)
  This file acts as the source file for the catalog component. As the
  source file of the catalog component, it contains the function definition
  of the six specific functions, along with some helper functions.
  This specific catalog component contains information about reading and
  managing the parks read in at startup. The component in total has about six
  functions that are used.
*/

#include "catalog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/**
  This function is responsible for returning the distance in miles between two specific
  parks. The function computes the distance based on the park's global coordinates.
  This function contains the actual implementation.
  @param a points to the first park that is being compared to calculate the distance.
  @param b points to the second park that is being compared to calculate the distance.
  @return the distance between park a and park b in miles. It is returned as a double value.
*/
double distance(Park const *a, Park const *b)
{
  // Parks a and b are converted from degrees to radians, then converting them to Cartesian coordinates
  double park1[] = {cos(a->lon * M_PI / 180) * cos(a->lat * M_PI / 180),
                    sin(a->lon * M_PI / 180) * cos(a->lat * M_PI / 180),
                    sin(a->lat * M_PI / 180)};

  double park2[] = {cos(b->lon * M_PI / 180) * cos(b->lat * M_PI / 180),
                    sin(b->lon * M_PI / 180) * cos(b->lat * M_PI / 180),
                    sin(b->lat * M_PI / 180)};

  // Dot product is initialized
  double dotProduct = 0.0;

  // Dot product is calcualted between the two vectors. Dot product equals the cosine of the angle btween vectors.
  for (int idx = 0; idx < sizeof(park1) / sizeof(park1[0]); idx++)
    dotProduct += park1[idx] * park2[idx];

  // Calculates the angle between the two vectors
  double angleBetweenVectors = acos(dotProduct);

  // Returns the final distance between the two parks
  return 3959 * angleBetweenVectors;
}

/**
  This function dynamically allocates storage for the catalog. The function initializes
  its fields and returns a pointer to the new catalog. In similar terms, this function
  acts like a constructor that is part of Java. This function contains the actual
  implementation.
  @return a pointer to the new catalog structure that is created.
*/
Catalog *makeCatalog()
{
  // Memory is being allocated for the new Catalog object
  Catalog *newCatalogObject = (Catalog *)malloc(sizeof(Catalog));

  // Simple check if the memory allocation doesn't fail
  if (!newCatalogObject)
  {
    exit(1);
  }

  newCatalogObject->parks = (Park **)malloc(5 * sizeof(Park *));

  // Checks if the memory allocation doesn't fail
  if (!newCatalogObject->parks)
  {
    free(newCatalogObject);
    exit(1);
  }

  // Sets the fields in the catalog struct to 0 and 5 respectively
  newCatalogObject->count = 0;
  newCatalogObject->capacity = 5;

  return newCatalogObject;
}

/**
  This function's main responsibility is to free the memory that is used to store the given
  catalog. This includes freeing space for all the parks, the resizable array of pointers, and
  freeing the space for the struct of the catalog. This function contains the actual
  implementation.
  @param catalog points to the instance of the catalog that is being freed.
*/
void freeCatalog(Catalog *catalog)
{
  for (int i = 0; i < catalog->count; i++)
  {
    free(catalog->parks[i]);
  }
  free(catalog->parks);
  free(catalog);
}

/**
  This function acts as a helper function to help parse a line from a park file. The function
  helps get all of the relevant information from the file and storing it in the struct of the
  park. The function handles the id, longitude, latitude, name, and the counties of the park. With
  any error that occurs, there is an error message that is printed out.
  @param firstLine the line that the function is parsing from, which contains all of the fields of the park.
  @param secondLane the next line of the park file.
  @param park points to the struct of the park that needs to contain all of the necessary information.
  @param name the file name of the file that is being read from.
*/
static void checkEachLine(char *firstLine, char *secondLine, Park *park, const char *name)
{
  // Token created to help parse each line
  char *lineTok;
  lineTok = strtok(firstLine, " ");

  // Checks to see if the token is found
  if (!lineTok)
  {
    fprintf(stderr, "Invalid park file: %s\n", name);
    exit(1);
  }

  // Token is converted to a string
  park->id = atoi(lineTok);
  lineTok = strtok(NULL, " ");

  // Check to see if the latitude is found
  if (!lineTok)
  {
    fprintf(stderr, "Invalid park file: %s\n", name);
    exit(1);
  }

  // Latitude is converted back to a number
  park->lat = atof(lineTok);

  // Next token is used which is the longitude
  lineTok = strtok(NULL, " ");
  if (!lineTok || strspn(lineTok, "-.0123456789") != strlen(lineTok))
  {
    fprintf(stderr, "Invalid park file: %s\n", name);
    exit(1);
  }

  // Converts the longitude back to a string
  park->lon = atof(lineTok);

  // Checks to see if there are counties present
  char *checkCounty = lineTok + strlen(lineTok) + 1;
  if (*checkCounty == '\0')
  {
    fprintf(stderr, "Invalid park file: %s\n", name);
    exit(1);
  }
  char *whiteSpaceCheck = checkCounty + strlen(checkCounty) - 1;

  // Whitespace is helped being trim
  while (whiteSpaceCheck > checkCounty && isspace((unsigned char)*whiteSpaceCheck))
  {
    whiteSpaceCheck--;
  }
  *(whiteSpaceCheck + 1) = '\0';

  char *parkPointer = park->numCounties;
  char *whiteSpace = park->numCounties + sizeof(park->numCounties) - 1;

  // Converts the list of counties to a format which includes commas
  while (*checkCounty && parkPointer < whiteSpace)
  {
    if (*checkCounty == ' ' && *(checkCounty + 1) != '\0')
    {
      *parkPointer++ = ',';
      while (*(checkCounty + 1) == ' ')
      {
        checkCounty++;
      }
    }
    else
    {
      *parkPointer++ = *checkCounty;
    }
    checkCounty++;
  }

  *parkPointer = '\0';

  // Comma count to help figure out how many counties are listed
  int countOfCommas = 0;
  for (char *parkPointer = park->numCounties; *parkPointer; parkPointer++)
  {
    if (*parkPointer == ',')
    {
      countOfCommas++;
    }
  }

  int countOfCounty = countOfCommas + 1;

  // Error checking to see if the maximum number of counties is reached
  if (countOfCounty > 5)
  {
    fprintf(stderr, "Invalid park file: %s\n", name);
    exit(1);
  }

  size_t sizeOfName = sizeof(park->name) - 1;

  // Error checking to see if the maximum name length is reached
  if (strlen(secondLine) > sizeOfName)
  {
    fprintf(stderr, "Invalid park file: %s\n", name);
    exit(1);
  }

  // From nextline, the name of the park is copied
  strncpy(park->name, secondLine, sizeof(park->name) - 1);
  park->name[sizeof(park->name) - 1] = '\0';
}

/**
  This function is responsible for reading all the parks with the given name. The function
  makes an instance of the Park struct for each one and stores a pointer to that Park in the
  resizable array in the catalog. This function contains the actual implementation.
  @param filename points to the array that is represented by the name of the file which holds the
  park data.
  @param catalog points to the instance of the catalog, which is where the parks are being read
  from.
*/
void readParks(char const *filename, Catalog *catalog)
{
  FILE *readFile = fopen(filename, "r");

  // Checks to see if the file can be opened
  if (!readFile)
  {
    fprintf(stderr, "Can't open file: %s\n", filename);
    exit(1);
  }

  // Character arrays that are used to store the first and second lines in the file
  char firstLine[300];
  char secondLine[300];

  // Loop used to read the file until the EOF
  while (fgets(firstLine, sizeof(firstLine), readFile))
  {
    firstLine[strcspn(firstLine, "\n")] = 0;

    // Second line is read
    if (fgets(secondLine, sizeof(secondLine), readFile))
    {
      secondLine[strcspn(secondLine, "\n")] = 0;
    }
    else
    {
      strcpy(secondLine, "");
    }

    // Memory is dynamically allocated for the struct of the park
    Park *park = (Park *)malloc(sizeof(Park));
    if (!park)
    {
      fclose(readFile);
      freeCatalog(catalog);
      exit(1);
    }

    // Calls helper function to help parse the lines in the file
    checkEachLine(firstLine, secondLine, park, filename);

    // Duplicate check for park
    for (int i = 0; i < catalog->count; i++)
    {
      if (catalog->parks[i]->id == park->id)
      {
        fprintf(stderr, "Invalid park file: %s\n", filename);
        free(park);
        fclose(readFile);
        freeCatalog(catalog);
        exit(1);
      }
    }

    // Field in park struct of capacity is increased if needed
    if (catalog->count >= catalog->capacity)
    {
      catalog->capacity *= 2;
      Park **temp = realloc(catalog->parks, catalog->capacity * sizeof(Park *));
      if (!temp)
      {
        free(park);
        fclose(readFile);
        freeCatalog(catalog);
        exit(1);
      }
      catalog->parks = temp;
    }

    // Park is added to the catalog, while increasing the count
    catalog->parks[catalog->count] = park;
    catalog->count++;
  }

  fclose(readFile);
}

/**
  This function's main purpose is to act as a comparison helper, as it is used for sorting.
  The function compares the two parks by their name.
  @param park1 points to the first park that is being compared.
  @param park2 points to the second park that is being compared.
  @return the specific value of the comparison that is resulted after the comparison. It uses
  its functionality by comparing the two parks. It returns -1 if the second id is greater, returns
  1 if the first id is greater, and 0 if they are identical. The function uses the ternary
  operator.
*/
static int checkId(const void *park1, const void *park2)
{
  // Dereferencing pointers to the parks to point to park struct
  int idFirstPark = (*(Park **)park1)->id;
  int idSecondPark = (*(Park **)park2)->id;

  // Comparing the id of the parks
  return (idFirstPark < idSecondPark) ? -1 : (idFirstPark > idSecondPark) ? 1
                                                                          : 0;
}

/**
  This function's main purpose is to act as a comparison helper, as it is used for sorting.
  The function compares the two parks by their name and then their id. Overall, the main
  purpose of this function is to act as a helper function.
  @param park1 points to the first park that is being compared.
  @param park2 points to the second park that is being compared.
  @return the specific value of the comparison. If the integer is greater than 0, less than 0,
  or equal to 0, it is less than the second park in terms of its id. If the names of the two parks
  are identical, then the comparison result of the two parks is returned.
*/
static int checkName(const void *park1, const void *park2)
{
  // Dereferencing pointers to the parks to point to park struct
  Park *firstPark = *(Park **)park1;
  Park *secondPark = *(Park **)park2;

  int compareByName = strcmp(firstPark->name, secondPark->name);

  // Names are same, proceed to compare their ID
  if (compareByName == 0)
  {
    return checkId(park1, park2);
  }

  return compareByName;
}

/**
  This function's main purpose is to sort the parks from the given catalog. The function
  uses the qsort() function with the function pointer parameter to order the parks in the
  correct way. This function contains only the declaration of the function as it is
  part of the header file. This function contains the actual implementation.
  @param catalog points to the catalog which contains all of the parks that need to be sorted.
  @param compare points to the comparison function which helps sort the parks in the necessary
  matter. It returns a negative integer if the first element is before the second in the ordering
  of the catalog. It returns 1 if the park is given by the first parameter and comes after the second
  one. It finally returns 0 if they are considered equal.
*/
void sortParks(Catalog *catalog, int (*compare)(void const *va, void const *vb))
{
  qsort(catalog->parks, catalog->count, sizeof(Park *), compare);
}

/**
  This function is responsible for printing all or some of the parks. This function uses
  the function pointer parameter together with the str (string) to figure out which parks
  are to be printed. The function also is used for the specific commands that are processed.
  In all, this function is responsible for printing the parks. This function contains the actual
  implementation.
  @param catalog points to the catalog which contains all of the parks that need to be listed.
  @param test points to the test function that helps with choosing the correct parks to list.
  @param str string representation that is used for the test function. This is mainly used to gather
  specific parks from the catalog.
*/
void listParks(Catalog *catalog, bool (*test)(Park const *park, char const *str), char const *str)
{
  // Checks to see if the catalog is empty
  if (catalog == NULL)
  {
    fprintf(stderr, "Catalog is empty.\n");
    return;
  }

  // Test is set to the sortParks function if it is null, otherwise set to compare
  if (test == NULL)
  {
    sortParks(catalog, checkId);
  }
  else
  {
    sortParks(catalog, checkName);
  }

  // Go through each park in the catalog while printing its details
  for (int i = 0; i < catalog->count; i++)
  {
    Park *checkedPark = catalog->parks[i];
    if (test == NULL || test(checkedPark, str))
    {
      printf("%-3d %-41s %7.3lf %8.3lf %s\n",
             checkedPark->id, checkedPark->name, checkedPark->lat, checkedPark->lon, checkedPark->numCounties);
    }
  }

  printf("\n");
}