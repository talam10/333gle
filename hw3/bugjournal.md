# Bug 1

## A) How is your program acting differently than you expect it to?
- When I was running valgrind on WriteIndex file, it was giving me a lot of errors. It shouldn't have given me any error.

## B) Brainstorm a few possible causes of the bug
- Maybe I forgot to check any edge case
- Maybe I forgot to free an iterator
- Maybe I forgot to initialize something

## C) How you fixed the bug and why the fix was necessary
- With the help of the TA, I figured out I was trying to pass in an empty bucket record position to WriteHTBucketRecord which was giving me the memory leak issue. I changed it to initialized positions and it fixed the memory leak issue.


# Bug 2

## A) How is your program acting differently than you expect it to?
- Once I fixed the memory leak issue, it started giving me DocIDReaderCheck issue, where it was predicting that the position is not correct. It should have passed all the tests when running test_suite.

## B) Brainstorm a few possible causes of the bug
- Maybe I assigned the position incorrectly
- Maybe I implemented WriteDocidToDocnameFn incorrectly 
- Maybe I was implementing WriteHashTable wrong 

## C) How you fixed the bug and why the fix was necessary
- With the help of the TA, I realized I forgot a small check in WriteHashTable where i wasn't doing a check whether the bucket record bytes were the size of BucketRecord struct. 


# Bug 3

## A) How is your program acting differently than you expect it to?
- Once I fixed my previous error, I encountered a new one where it was failing one of the test_suite tests called WriteIndexBasic. It was returning a -1 instead of the desired result (res). It should have returned the correct result intead of a kFailedWrite value "-1".

## B) Brainstorm a few possible causes of the bug
- maybe I am implementing WriteHashTable wrong
- maybe I am implementing WriteHTBucketRecord wrong
- maybe I am implementing WriteHTBucket wrong

## C) How you fixed the bug and why the fix was necessary
- Again with the help of the TA, we did some print statements in between of the WriteHashTable, WriteHTBucketRecord, and WriteHTBucket. And after running test_suite we saw that I forgot to do a check fwrite(...) !=1 which was creating all the issues. Once I fixed that check, it worked perfectly fine. YAY! :)
