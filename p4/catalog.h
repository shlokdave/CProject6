/**
  @file catalog.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the catalog component. As the
  header file, this file is responsible for containing the declarations
  of variables and functions that are used in the other source files.
  This specific catalog component contains information about reading and
  managing the parks read in at startup. The component in total has about six
  functions that are used.
*/

#include <stdbool.h>

/** Defines the maximum length for a name of a single park. */
#define PARK_NAME_LENGTH 41

/** Defines the maximum length for the numCounties array. */
#define NUM_COUNTIES_LENGTH 101

/**
  This structure is created to help hold all of the information about a single park. The struct
  contains all of the fields that relate to a park that could be added to the catalog. It is
  used to store all of the detailed information about parks. The id and number of counties are
  stored as integers, the latitude and longitude as doubles, and the names as a string. The county
  names will be stored as an array of strings.
  @param id integer representation of the id, or unique identifier, of the park.
  @param name character array representation, which is a string, that is used to store the name of the
  park. Keeps a maximum size of 41 characters.
  @param lat double representation of the latitude of the park.
  @param lon double presentation of the longitude of the park.
  @param numCounties character array representation, which is a string, that is used to store all of the
  names of the counties for a specific park to be in. Keeps a maximum size of 101 characters.
*/
typedef struct
{
  int id;
  char name[PARK_NAME_LENGTH];
  double lat;
  double lon;
  char numCounties[NUM_COUNTIES_LENGTH];
} Park;

/**
  This structure is represented as the catalog struct. The struct contains fields that are
  needed to store the resizable array of pointers to the park instances. The main purpose
  of this struct is to hold the collection of parks. The struct contains the specific fields
  of the count and capacity needed to grow the array.
  @param parks points to the array of pointers that are in the park struct. This array helps
  add the parks in the catalog.
  @param count integer representation of the current number of parks that are part of the catalog.
  @param capacity integer representation of the capacity of the array of parks.
*/
typedef struct
{
  Park **parks;
  int count;
  int capacity;
} Catalog;

/**
  This function is responsible for returning the distance in miles between two specific
  parks. The function computes the distance based on the park's global coordinates. The variables
  are defined to help figure out the radius of the Earth. This function contains only the declaration
  of the function as it is part of the header file.
  @param a points to the first park that is being compared to calculate the distance.
  @param b points to the second park that is being compared to calculate the distance.
  @return the distance between park a and park b in miles. It is returned as a double value.
*/
double distance(Park const *a, Park const *b);

/**
  This function dynamically allocates storage for the catalog. The function initializes
  its fields and returns a pointer to the new catalog. In similar terms, this function
  acts like a constructor that is part of Java. This function contains only the declaration
  of the function as it is part of the header file.
  @return a pointer to the new catalog structure that is created.
*/
Catalog *makeCatalog();

/**
  This function's main responsibility is to free the memory that is used to store the given
  catalog. This includes freeing space for all the parks, the resizable array of pointers, and
  freeing the space for the struct of the catalog. This function contains only the declaration
  of the function as it is part of the header file.
  @param catalog points to the instance of the catalog that is being freed.
*/
void freeCatalog(Catalog *catalog);

/**
  This function is responsible for reading all the parks with the given name. The function
  makes an instance of the Park struct for each one and stores a pointer to that Park in the
  resizable array in the catalog. This function contains only the declaration of the function
  as it is part of the header file.
  @param filename points to the array that is represented by the name of the file which holds the
  park data.
  @param catalog points to the instance of the catalog, which is where the parks are being read
  from.
*/
void readParks(char const *filename, Catalog *catalog);

/**
  This function's main purpose is to sort the parks from the given catalog. The function
  uses the qsort() function with the function pointer parameter to order the parks in the
  correct way. This function contains only the declaration of the function as it is
  part of the header file.
  @param catalog points to the catalog which contains all of the parks that need to be sorted.
  @param compare points to the comparison function which helps sort the parks in the necessary
  matter. It returns a negative integer if the first element is before the second in the ordering
  of the catalog. It returns 1 if the park is given by the first parameter and comes after the second
  one. It finally returns 0 if they are considered equal.
*/
void sortParks(Catalog *catalog, int (*compare)(void const *va, void const *vb));

/**
  This function is responsible for printing all or some of the parks. This function uses
  the function pointer parameter together with the str (string) to figure out which parks
  are to be printed. The function also is used for the specific commands that are processed.
  In all, this function is responsible for printing the parks. This function contains only the
  declaration of the function as it is part of the header file.
  @param catalog points to the catalog which contains all of the parks that need to be listed.
  @param test points to the test function that helps with choosing the correct parks to list.
  @param str string representation that is used for the test function. This is mainly used to gather
  specific parks from the catalog.
*/
void listParks(Catalog *catalog, bool (*test)(Park const *park, char const *str), char const *str);