# Bug 1

## A) How is your program acting differently than you expect it to?

- I am getting a segmentation fault when running the test_memindex.cc tests.

## B) Brainstorm a few possible causes of the bug

- I am incorrectly inserting the WordPostings data structure into the index HashTable in the AddPostingList method, such that dereferencing a HTKeyValue_t value attribute results in a segfault.
- I am trying to access a WordPostings struct for a word that does not exist in the memindex table.

## C) How you fixed the bug and why the fix was necessary

- I added an if-statement to check the return value of HashTable_Find in MemIndex_Search (step 4) that verifies the word was found in the index hash table before trying to access its associated WordPosting struct. This fixed the bug by ensuring that I did not dereference an output parameter pointer from a call to HashTableFind that was filled with mystery data.

# Bug 2

## A) How is your program acting differently than you expect it to?

- I am getting the error "Verify333(iter->node != NULL) failed in file LinkedList.c, line 235" when running the Test_MemIndex.Integration test. This happens on the call to LLIterator_Get when iterating through the SearchResults in ret_list to see if they are still valid matches for query[i]. This is in part 6 of MemIndex.c at line 246 of the MemIndex_Search method.

## B) Brainstorm a few possible causes of the bug

- I am iterating past when the LLIterator becomes invalid.
- I am incorrectly calling LLIterator_Remove when a SearchResult does not match query[i].
- I am incorrectly calling LLIterator_Next to advance the iterator state.

## C) How you fixed the bug and why the fix was necessary

- I moved the call to LLIterator_Next to only execute when the current SearchResult matches query[i]. In the case no match is found, calling LLIterator_Remove automatically advances the LLIterator, so an additional LLIterator_Next call advanced the iterator too far and led to an error when calling LLIterator_Get since the iterator was invalid.

# Bug 3

## A) How is your program acting differently than you expect it to?

- Calls to CrawlFileTree.c are executing without immediate error and HandleDir is correctly adding filepaths to the entries array, but HandleFile is never being called on these entries.

## B) Brainstorm a few possible causes of the bug

- I am incorrectly looping through the entries, such that HandleFile is never being called on the filepaths in the entries list.
- I am incorrectly breaking out of the loop that recursively searches for files in a directory, such that the entries list is lost.

## C) How you fixed the bug and why the fix was necessary

- I changed the exit condition of the while loop on line 140-141 to call break instead of return. Returning once the loop had processed every file/directory in the current directory meant that the entries list was lost. Breaking out the loop correctly ended the loop while allowing the rest of the file processing code to run.

# Bug 4

## A) How is your program acting differently than you expect it to?

- After running searchshell the ranks of the files don't match the transcript

## B) Brainstorm a few possible causes of the bug

- There is something wrong with eitehr FileParser.c or MemIndex.c when parsing or incrementing rank. Another reason could be my token variable in searchshell.c is not splitting the input correctly. 

## C) How you fixed the bug and why the fix was necessary

- I changed the loop condition in step 6 of MemIndex.c from LLIterator_isValid to a for loop where it is incrementing the ranks. I set a boundary using number of elements in the Linked List Iterator that we stored previously. The fix was necessary because some of the bigger files were still inceremnting the rank as long as the iterator seemed valid and probably was also individually running some words by itself. But there was no boundary like once the number of elements passed is done parsing, you stop incrementing rank. Hence, it was giving us higher rank number than usual.