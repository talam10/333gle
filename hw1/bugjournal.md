# Bug 1

## A) How is your program acting differently than you expect it to?
- test (does it work)?
My output from LLIteratorNext() doesn't match the solution binaries
## B) Brainstorm a few possible causes of the bug
- Maybe my iterator that is already at the last node is not getting moved pass the list
- Maybe my iterator pointer is stuck at some node and not updating to the next one
- Maybe I'm returning a false value when it should return a true value

## C) How you fixed the bug and why the fix was necessary
- I fixed the bug with the help of my TA. I had to update my current iterator first with the next iterator. And then check if my current iterator was NULL or not.

- The fix was necessary because I was checking if my current node is null before I was updating my current node to the next one. This way, when the iterator pointer got to the very end, it was returning a null value but the iterator was not getting moved pass the list.


# Bug 2

## A) How is your program acting differently than you expect it to?
- My output from LLIteratorRemove() doesn't match the solution binaries

## B) Brainstorm a few possible causes of the bug
- I think I'm removing the wrong node
- I am probably freeing my node wrongly
- I am probably not handling a degenerative case properly

## C) How you fixed the bug and why the fix was necessary
- I made a separate copy of the iterator node I wanted to free
- Then run all the operations for the degenerative cases and after that I freed the iterator I stored earlier
- The fix was necessary because I was freeing the node only after I had updated my iterator according to all the degenerative cases. Hence, it was not working for the middle iterator slicing since that was the last denerative case. Once I made a sepaarte copy of the node I wanted to free and then updated the pointers and freed the previously stored pointer, it didn't mess with the iterators in the middle. 

# Bug 3

## A) How is your program acting differently than you expect it to?
- My HashTable is producing a segmentation fault upon testing.

## B) Brainstorm a few possible causes of the bug
- I'm not correctly passing pointers between functions, leading me to dereference deallocated stack addresses
- I'm not correctly storing and accessing the LLPayload_t of the HTKeyValue in findAndRemove()
- I'm not dynamically allocating the HTKeyValue payload to the heap, such that it remains a local variable and is overwritten upon stack frame deallocation

## C) How you fixed the bug and why the fix was necessary
- I dynamically allocated the HTKeyValue payload to the heap. This solved the segmentation faults because I am now dereferencing persistent addresses in the heap to set and access the HT payload, rather than local stack frame addresses.
