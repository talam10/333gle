/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"
#define BUF_LENGTH 1024

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
static void ProcessQueries(DocTable* dt, MemIndex* mi);
static int GetNextLine(FILE* f, char** ret_str);


//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  //
  // Note that you should make sure the fomatting of your
  // searchshell output exactly matches our solution binaries
  // to get full points on this part.

  // Declare some variables first
  DocTable* doc_table;
  MemIndex* index;

  // Crawl from a directory provided by argv[1] to produce and index
  printf("Indexing \'%s\'\n", argv[1]);
  // check if returns false
  if (CrawlFileTree(argv[1], &doc_table, &index) == 0) {
    Usage();
  }
  ProcessQueries(doc_table, index);
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}


static void ProcessQueries(DocTable* dt, MemIndex* mi) {
  // Prompt the user for a query and read the query from stdin, in a loop
  while (true) {
    printf("enter Query: \n");
    char *input = NULL;
    if (GetNextLine(stdin, &input) != 0) {
      char** query = (char **) malloc(BUF_LENGTH * sizeof(char*));
      Verify333(query != NULL);
      char *token;
      int element = 0;

      // convert to lower case
      while (input[element] != '\0') {
        input[element] = (char)tolower((int) input[element]);
        element++;
      }
      Verify333(query != NULL);
      uint8_t size = 0;
      char *str = input;
      while (1) {
        // Split a query into words
        token = strtok_r(str, " ", &str);
        if (token == NULL || *token == '\n') {
          break;
        }
        query[size] = token;
        size++;
      }
      // continue if no valid query was entered
      // like '\n' or only whitespace
      if (size == 0) {
        free(input);
        free(query);
        continue;
      }
      // replace last token's '\n' by '\0'
      // as fgets read '\n' into input
      char *nChar = strchr(query[size-1], '\n');
      if (nChar != NULL) {
        *nChar = '\0';
      }
      // search for the documents that contains
      // all the words in the query
      LinkedList *search = MemIndex_Search(mi, query, size);
      if (search == NULL) {
        // if our search is NULL, we free the query and continue
        free(query);
        free(input);
        continue;
      }
        // Now we start iterating through our search
      LLIterator *iter = LLIterator_Allocate(search);
      Verify333(iter != NULL);
      while (1) {
        // loop though result files and print out names and ranks
        SearchResult* sr_str;
        LLIterator_Get(iter, (LLPayload_t *)&sr_str);

        // print out all the documents under the directory
        // and the rank for the query
        printf("  %s (%u)\n",
                (char *) DocTable_GetDocName(dt, sr_str->doc_id),
                sr_str->rank);
        if (!LLIterator_Next(iter)) {
          break;
        }
      }
      // free the iterator and query now
      LLIterator_Free(iter);
      free(query);
      free(input);
      LinkedList_Free(search, &free);
    } else {
      // when user presses ctrl + D, it shuts down
      printf("shutting down...\n");
      break;
    }
  }
  DocTable_Free(dt);
  MemIndex_Free(mi);
}


static int GetNextLine(FILE *f, char **ret_str) {
  char line[BUF_LENGTH];
  if (fgets(line, BUF_LENGTH, f) == NULL) {
    return 0;
  }
  // checks the length of the character and compares
  line[strcspn(line, "\r\n")] = 0;

  // duplicates and stores the string in *ret_str
  *ret_str = strdup(line);
  return 1;
}


