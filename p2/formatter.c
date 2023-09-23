/**
   @file formatter.c
   @author Shlok Dave (ssdave)
   This file acts as the overall "brain" of the program. The
   file is responisbile for outputting all of the correct information
   to a new file. The line starts with the individual's name in standard
   format, then their formatted birthdate, and then their unique SSN. This
   file provides the final summary of information regarding the specific
   individual.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "name.h"
#include "date.h"
#include "ssn.h"

#define AGE_CUTOFF "2002-09-22"

/**
  This is the main part of the program and it is responsible for processing and
  displaying all of the information about the individuals. The function has the
  main responsbility of gathering all of the information and presenting it. It
  extracts and formats the name, birthdate, and social security number of that
  individual. The main function also counts the individuals who are above 21, the
  total number of individuals in the input, the maximum name length, and the number
  of individuals without a social security nunber.
*/
int main()
{
  int countAllPeople = 0;
  int finalNameSize = 0;
  int peopleOver21_or_over = 0;
  int peopleWithoutSsn = 0;
  char name[FIELD_MAX + 1];
  char date[FIELD_MAX + 1];
  char ssn[FIELD_MAX + 1];
  char temp[FIELD_MAX + 1];

  int readSsn = 0;

  // Loops to process each line of the input file.
  while (fgets(temp, sizeof(temp), stdin))
  {
    if (!readSsn)
    {
      // Delimeter used to parse the line.
      char *tempSymbol = strtok(temp, ":");
      // Processes the name part of the file
      if (tempSymbol)
      {
        strncpy(name, tempSymbol, FIELD_MAX);
        name[FIELD_MAX] = '\0';

        size_t sizeOfString = strlen(name);
        if (sizeOfString > 0 && name[sizeOfString - 1] == '\n')
        {
          name[sizeOfString - 1] = '\0';
        }

        // Formats the name to the standard format.
        fix_name(name);
        char *com = strchr(name, ',');
        if (com)
        {
          int checkForSpace = 0;
          char *commaCounter = com + 1;
          while (*commaCounter)
          {
            if (*commaCounter == ' ')
              checkForSpace++;
            commaCounter++;
          }

          if (checkForSpace > 1)
          {
            exit(101);
          }
          if (*commaCounter == ' ')
          {
            exit(101);
          }
        }

        // Keeps record of the length.
        int sizeOfName_name_length = strlen(name);
        if (sizeOfName_name_length > finalNameSize)
        {
          finalNameSize = sizeOfName_name_length;
        }

        // Date part gets parsed now.
        tempSymbol = strtok(NULL, ":");
        if (tempSymbol)
        {
          strncpy(date, tempSymbol, FIELD_MAX);
          date[FIELD_MAX] = '\0';
          fix_date(date);
        }
        else
        {
          continue;
        }

        // SSN part gets parsed now.
        tempSymbol = strtok(NULL, ":");
        if (tempSymbol)
        {
          strncpy(ssn, tempSymbol, FIELD_MAX);
          ssn[FIELD_MAX] = '\0';

          if (ssn[strlen(ssn) - 1] == '\n')
          {
            ssn[strlen(ssn) - 1] = '\0';
          }

          // Makes sure the SSN is not blank.
          if (strcmp(ssn, "N/A") == 0)
          {
            countAllPeople++;
            printf("%-30s %-10s         N/A\n", name, date);
            peopleWithoutSsn++;
            continue;
          }

          // Here we handle the valid SSN input.
          if (strlen(ssn) == 11 && ssn[3] == '-' && ssn[6] == '-')
          {
            char formatted_ssn[FIELD_MAX + 1];
            snprintf(formatted_ssn, sizeof(formatted_ssn), "%c%c%c-%c%c-%c%c%c%c",
                     ssn[0], ssn[1], ssn[2], ssn[4], ssn[5], ssn[7], ssn[8], ssn[9], ssn[10]);
            strcpy(ssn, formatted_ssn);
          }

          if (ssn[strlen(ssn) - 1] != '\n' && strlen(ssn) < FIELD_MAX - 1)
          {
            readSsn = 1;
            continue;
          }
          else if (ssn[strlen(ssn) - 1] == '\n')
          {
            ssn[strlen(ssn) - 1] = '\0';
          }
        }
      }
      else
      {
        continue;
      }
    }
    else
    {
      // This is to see the SSN between two lines.
      if (temp[strlen(temp) - 1] == '\n')
      {
        temp[strlen(temp) - 1] = '\0';
      }
      strncat(ssn, temp, FIELD_MAX - strlen(ssn) - 1);
      readSsn = 0;
    }

    // Formats the SSN.
    fix_ssn(ssn);

    countAllPeople++;

    if (strcmp(date, AGE_CUTOFF) <= 0)
    {
      peopleOver21_or_over++;
    }

    if (strcmp(ssn, SSN_NA) == 0)
    {
      peopleWithoutSsn++;
    }

    // Prints the correct output of the input.
    printf("%-30s %-10s %-11s\n", name, date, ssn);
  }

  // Prints the summary of the information from the input.
  printf("\nSummary\n");
  printf("-----------------------------------\n");
  printf("%-26s %8d\n", "Total individuals", countAllPeople);
  printf("%-26s %8d\n", "Maximum name length", finalNameSize);
  printf("%-26s %8d\n", "Individuals 21 or over", peopleOver21_or_over);
  printf("%-26s %8d\n", "Individuals without an SS#", peopleWithoutSsn);

  return 0;
}
