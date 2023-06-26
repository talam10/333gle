# 333gle

This is a multithreaded web server that processes files and queries and is built up on four different homeworks each dependednt prospectively on the previously numbered one.
Beloe is mentioned each homework specs to get a better idea on what is the whole project about and how it is built.

HW1:

Goals
For Homework 1, you will complete our implementation of two C data structures: a doubly-linked list (Part A) and a chained hash table (Part B).
Part A: Doubly-Linked List
If you've programmed in Java, you're used to having a fairly rich library of elemental data structures upon which you can build, such as vectors and hash tables. In C, you don't have that luxury: the C standard library provides you with very little. In this assignment, you will add missing pieces of code in our implementation of a generic doubly-linked list.

At a high-level, a doubly-linked list looks like this:

Each node in a doubly-linked list has three fields: a payload, a pointer to the previous element in the list (or NULL if there is no previous element), and a pointer to the next element in the list. If the list is empty, there are no nodes. If the list has a single element, both of its next and previous pointers are NULL.

So, what makes implementing this in C tricky? Quite a few things:

We want to make the list useful for storing arbitrary kinds of payloads. In practice, this means the payload element in a list node needs to be a pointer supplied by the customer of the list implementation. Given that the pointer might point to something malloc'ed by the customer, this means we might need to help the customer free the payload when the list is destroyed.
We want to hide details about the implementation of the list by exposing a high-level, nicely abstracted API. In particular, we don't want our customers to fiddle with next and previous pointers in order to navigate through the list, and we don't want our customers to have to stitch up pointers in order to add or remove elements from the list. Instead, we'll offer our customers nice functions for adding and removing elements and a Java-like iterator abstraction for navigating through the list.
C is not a garbage-collected language: you're responsible for managing memory allocation and deallocation yourself. This means we need to be malloc'ing structures when we add nodes to a list, and we need to be free'ing structures when we remove nodes from a list. We also might need to malloc and free structures that represent the overall list itself.
Given all of these complications, our actual linked list data structure ends up looking like this:



Specifically, we define the following types and structures:

LinkedList: The structure containing our linked list's metadata, such as head and tail pointers. When our customer asks us to allocate a new, empty linked list, we malloc and initialize an instance of this structure then return a pointer to that malloc'ed structure to the customer.
LinkedListNode: this structure represents a node in a doubly-linked list. It contains a field for stashing away (a pointer to) the customer-supplied payload and fields pointing to the previous and next LinkedListNode in the list. When a customer requests that we add an element to the linked list, we malloc a new LinkedListNode to store the pointer to that element, do surgery to splice the LinkedListNode into the data structure, and update our LinkedList's metadata.
LLIterator: sometimes customers want to navigate through a linked list; to help them do that, we provide them with an iterator. LLIterator contains bookkeeping associated with an iterator. In particular, it tracks the list that the iterator is associated with and the node in the list that the iterator currently points to. Note that there is a consistency problem here: if a customer updates a linked list by removing a node, it's possible that some existing iterator becomes inconsistent because it referenced the deleted node. So, we make our customers promise that they will free any live iterators before mutating the linked list. (Since we are generous, we do allow a customer to keep an iterator if the mutation was done using that iterator.) When a customer asks for a new iterator, we malloc an instance and return a pointer to it to the customer.
Part A Instructions
You should follow these steps to do this part of the assignment:

Make sure you are comfortable with C pointers, structures, malloc, and free. We will cover them in detail in lecture, but you might need to brush up and practice a bit on your own; you should have no problem Googling for practice programming exercises on the Web for each of these topics.
Get the source files for hw1: navigate to a directory that contains a checked-out copy of your cse333 git repository and run the command git pull. (See GitHub Docs if the pull command fails because you have unstaged changes or other problems.) After the pull command finishes you should see at least the following directories and files in your repository:
$ ls
cpplint.py  exercises gtest  hw0  hw1
Look inside the hw1 directory. You'll see a number of files and subdirectories, including these that are relevant to Part A:
Makefile: a Makefile you can use to compile the assignment using the Linux command make on the CSE Linux machines.
LinkedList.h: a header file that defines and documents the API of the linked list. A customer of the linked list includes this header file and uses the functions defined within in. Read through this header file very carefully to understand how the linked list is expected to behave.
LinkedList_priv.h: a private header file included by LinkedList.c; it defines the structures we diagrammed above. These implementation details would typically be withheld from the client by placing the contents of this header directly in LinkedList.c; however, we have opted to place them in a "private .h" instead so that our unit test code can verify the correctness of the linked list's internals.
LinkedList.c: Contains the partially-completed implementation of our doubly-linked list. Your task will be to finish the implementation; see the labels that say "STEP X:". Take a minute and read through both LinkedList_priv.h and LinkedList.c.
example_program_ll.c: this is a simple example of how a customer might use the linked list; in it, you can see how a customer can allocate a linked list, add elements to it, create an iterator, use the iterator to navigate a bit, and then clean up.
test_linkedlist.cc: this file contains unit tests that we wrote to verify that the linked list implementation works correctly. The unit tests are written to use the Google Test unittesting framework, which has similarities to Java's JUnit testing framework. As well, this test driver will assist the course staff in grading your assignment. As you add more pieces to the implementation, the test driver will make it further through the unit tests, and it will print out a cumulative score along the way. You don't need to understand what's in the test driver for this assignment, though if you peek inside it, you might get hints for what kinds of things you should be doing in your implementation!
solution_binaries: in this directory, you'll find some Linux executables, including example_program_ll and test_suite. These binaries were compiled with a complete, working version of LinkedList.c; you can run them to explore what should be displayed when your assignment is working!
Run make on a CSE Linux machine to verify that you can build your own versions of example_program_ll and test_suite. make should print out a few things, and you should end up with new binaries inside the hw1 directory.
Since you haven't yet finished the implementation of LinkedList.c, the binaries you just compiled won't work correctly yet. Try running them, and note that example_program_ll halts with an assertion error or a segfault and test_suite prints out some information indicating failed tests, and may crash before terminating.
This is the hard step: finish the implementation of LinkedList.c. Go through LinkedList.c, find each comment that says "STEP X", and place working code there (please keep the "STEP X" comment for your graders' sanity so they can locate your code!). The initial steps are meant to be relatively straightforward, and some of the later steps are trickier. You will probably find it helpful to read through the code from top to bottom to figure out what's going on. You will also probably find it helpful to recompile frequently to see what compilation errors you've introduced and need to fix. When compilation works again, try running the test driver to see if you're closer to being finished.
Note: You may not modify any header files or interfaces in this or later project assignments. We may test your code by extracting your implementations and compiling them with the original header files or in some other test harness where they are expected to behave as specified. You are certainly free, of course, to add additional private (e.g., static – more info here) helper functions in your implementation, and you should do that when it improves modularity.
Debugging hint: Verify333 is used in many places in the code to check for errors and terminate execution if something is wrong. You might find it helpful to discover the function that is called when this happens so you can place a debugger breakpoint there.
We'll also be testing whether your program has any memory issues using Valgrind. To try out Valgrind on the solution binaries:
From the hw1 directory run the following command:
$ valgrind --leak-check=full ./solution_binaries/example_program_ll
Note that we are runnning this on the solution binaries, so Valgrind prints out that no memory issues were found. Similarly, try running the solution test driver under Valgrind:
$ valgrind --leak-check=full ./solution_binaries/test_suite
and note that Valgrind again indicates that no memory issues were found.
To test your implementation, compile your versions of the example_program_ll and test_suite binaries while in the hw1 directory and run them under Valgrind. For example:
$ valgrind --leak-check=full ./test_suite
If you have no memory issues and the test_suite runs the linked list tests to completion, you're done with Part A!
Part B: Chained Hash Table
A chained hash table is a data structure that consists of an array of buckets, with each bucket containing a linked list of elements. When a user inserts a key/value pair into the hash table, the hash table uses a hash function to map the key into one of the buckets, and then adds the key/value pair onto the linked list. There is an important corner case: if the key of the inserted key/value pair already exists in the hash table; our implementation of a hash table replaces the existing key/value pair with the new one and returns the old key/value pair to the customer.

Over time, as more and more elements are added to the hash table, the linked lists hanging off of each bucket will start to grow. As long as the number of elements in the hash table is a small multiple of the number of buckets, lookup time is fast: you hash the key to find the bucket, then iterate through the (short) chain (linked list) hanging off the bucket until you find the key. As the number of elements gets larger, lookups become less efficient, so our hash table includes logic to resize itself by increasing the number of buckets to maintain short chains.

As with the linked list in Part A, we've given you a partial implementation of a hash table. Our hash table implementation looks approximately like this:



Specifically, we defined the following types and structures:

HashTable: The structure containing our hash table's metadata, such as the number of elements and the bucket array. When our customer asks us to allocate a new, empty hash table, we malloc and initialize an instance of this (including malloc'ing space for the bucket array that it uses and allocating LinkedLists for each bucket), and return a pointer to that malloc'ed structure to the customer.
HTIterator (not shown in the diagram): sometimes customers want to iterate through all elements in a hash table; to help them do that, we provide them with an iterator. HTIterator points to a structure that contains bookkeeping associated with an iterator. Similar to a linked list iterator, the hash table iterator keeps track of the hash table the iterator is associated with and in addition has a linked list iterator for iterating through the bucket linked lists. When a customer asks for a new iterator we malloc an HTIterator and return a pointer to it.
Part B Instructions
You should follow these steps to do this part of the assignment:

The code you fetched in Part A also contains the files you'll need to complete your hash table implementation and test it. Similar to the linked list, the hash table implementation is split across a few files: HashTable.c contains the implementation you need to finish, HashTable.h contains the public interface to the hash table and documents all of the functions & structures that customers see, and HashTable_priv.h contains some private, internal structures that HashTable.c uses.
Read through HashTable.h first to get a sense of what the hash table interface semantics are. Then, take a look at example_program_ht.c; this is a program that uses the hash table interface to insert/lookup/remove elements from a hash table, and uses the iterator interface to iterate through the elements of the hash table.
As before, test_hashtable.cc contains our Google Test unit tests for the hash table. Run this — on its own, and using valgrind — to seer how close you are to finishing your hash table implementation.
Look through HashTable.c, find all of the missing pieces (identified by "STEP X" comments, as before), and implement them.
As before, in solution_binaries, we've provided linux executables (i.e., example_program_ht and the same test_suite) that were compiled with our complete, working version of HashTable.c. You can run them to explore what should be displayed when your Part B implementation is working and look at the source code for examples of how to use the data structures.


HW2:

Goals
In this assignment, you will use the LinkedList and HashTable modules that you built in Homework 1 in order to finish our implementation of a file system crawler, indexer, and search engine. Be sure to follow good C practices and maintain good code quality as you complete the assignment.

In Part A, you will build a module that reads the content of a file into memory, parses it into a series of words, and builds a linked list of (word, position) information.
In Part B, you will build modules that convert a series of these linked lists into an in-memory, .
In Part C, you will use this in-memory, inverted index to build a query processor that has a console-based interface.
While you do these parts, you are expected to fill in a bug journal which can be found in bug_journal.md.
As before, please read through this entire document before beginning the assignment, and please start early! There is a fair amount of coding you need to do to complete this assignment, and it will definitely expose any conceptual weaknesses you have with the prior material on C, pointers, malloc/free, and the semantics of the LinkedList and HashTable implementations.

In-Memory File System Search Engine
Part A: File Parser
You're going to write a module that reads the contents of a text file into memory and then parses the text file to look for words within it. As it finds words, it will build up a HashTable that contains one record for each word. Each record will contain a lowercase copy of the word, and also a sorted linked list. Each record of the linked list contains an offset within the file that the word appeared in (the first character in the file has offset zero). However, our word parser won't be very smart. It will treat as a word any non-zero sequence of alphabetic characters separated by non-alphabetic characters.

So, graphically, what your module will do is take a text file that contains something like this, where '\n' represents the newline control character that appears at the end of each line in the input file:

My goodness!  I love the course CSE333.\n
I'll recommend this course to my friends.\n
and produces a data structure that looks like this:



Specifically, note a few things:

Each key in the hash table is the result of calling the HashTable module's FNVHash64() function, passing the string as the first argument, and the strlen(string) as the second argument.
Each element in the hash table is a pointer to a heap-allocated structure that contains two fields; a string and a linked list. Note the string is lower-cased, and that our parser is not very smart: because it treats any sequence of alphabetic characters surrounded by non-alphabetic characters as words, the word I'll will be misparsed as the two words i and ll.
Each element in the linked list is an integer representing the position in the text file at which the word starts; this is both its byte offset and, since we are only handling ASCII files, the number of characters from the start of the file (each ASCII character is exactly 1 byte). So, the word "my" starts at offset 0 in the text file, the word "i" appears twice, once at offset 14 and once at offset 40, and the word "course" appears twice, once at offset 25 and once at offset 62.
Each list is sorted in ascending order.
Part A Instructions
You should follow these steps:

Navigate to the directory containing your cse333 Gitlab repository. Enter git pull to retrieve new folders containing the starter code for hw2 and a directory containing test data for the remaining parts of the project. Check that you have everything.
$ git pull
...git output...
$ ls
cpplint.py exercises gtest  hw0  hw1  hw2  projdocs
(Note that you still need the hw1 directory; hw2 won't build properly without it. It's ok if you've deleted your hw0 directory.)

Take a look inside hw2, and note a few things:
There is a subdirectory called libhw1/. It has symbolic links to header files and your libhw1.a from ../hw1. Therefore you should make sure you have compiled everything in ../hw1 while working on hw2.
If you don't think your HW1 solution is up to the job, you can use ours instead. Our libhw1.a is in ../hw1/solution_binaries; just copy it over your ../hw1/libhw1.a.
Just like with HW1, there is a Makefile that compiles the project, a series of files (i.e., test_*) that contain our unit tests, and some files (DocTable.c, DocTable.h, CrawlFileTree.c, CrawlFileTree.h, FileParser.c, FileParser.h, MemIndex.c, MemIndex.h, searchshell.c) that contain our partial implementation of the project.
Type "make" to compile the project, and try running the test suite by running ./test_suite. It should fail (and might not even terminate), since most of the implementation is missing!
Also note there is a new subdirectory called test_tree/ that contains a bunch of text files and subdirectories containing more text files. Explore this subdirectory and its contents; start with the README.TXT file.
Your job in Part A is to finish the implementation of FileParser.c. Start by reading through FileParser.h and make sure you understand the semantics of the functions. Also, look at the WordPositions structure defined in fileparser.h and compare it to the figure above. The function ParseIntoWordPositionsTable() builds a HashTable that looks like the figure, and each value in the HashTable contains a heap-allocated WordPositions structure.
Similar to HW1, look through FileParser.c to get a sense of its layout, and look for all occurrences of STEP X (e.g., STEP 1, STEP 2, ...) for where you need to add code. Be sure to read the full file before adding any code, so you can see the full structure of what we want you to do. Once you're finished adding code, run the test suite and you should see some tests start to succeed!
As before, in the subdirectory solution_binaries/ we've provided you with Linux executables (i.e., test_suite and searchshell) that were compiled with our complete, working version of HW2. You can run them to see what should happen when your HW2 is working.
Also, as before, you must implement all components as specified and you may not modify any header files. You are, of course, free to add additional private (static) functions when that makes sense.
Part B: File Crawler and Indexer
At a high-level, a search engine has three major components: a crawler, an indexer, and a query processor. A crawler explores the world, looking for documents to index. The indexer takes a set of documents found by the crawler, and produces something called an inverted index out of them. A query processor asks a user for a query, and processes it using the inverted index to figure out a list of documents that match the query.

File System Crawler
Your file system crawler will be provided with the name of a directory in which it should start crawling. Its job is to look through the directory for documents (text files) and to hand them to the indexer, and to look for subdirectories; it recursively descends into each subdirectory to look for more documents and sub-sub-directories. For each document it encounters, it will assign the document a unique "document ID", or "docID". A docID is just a 64-bit unsigned integer.

Your crawler itself will build two hash tables in memory, adding to them each time it discovers a new text file. The two hash tables map from docID to document filename, and from document filename to docID:



For each document the crawler finds, it will make use of your Part A code to produce a word hashtable using ParseIntoWordPositionsTable().

Indexer
This is the heart of the search engine. The job of the indexer is to take each word hashtable produced by ParseIntoWordPositionsTable(), and fold its contents in to an inverted index. An inverted index is just a hash table that maps from a word to a "posting list," and a posting list is just a list of places that word has been found.

Specifically, the indexer should produce an in-memory hash table that looks roughly like this:



Walking through it, the inverted index is a hash table that maps from a (hash of a) word to a structure. The structure (shown in green) contains the word as a string, and also a HashTable. The HashTable maps from the docID (not the hash of docID) to a LinkedList. The LinkedList contains each position that word appeared in that docID.

So, based on the figure, you can see that the word "course" appeared in a single document with docID 3, at byte offsets 25 and 62 from the start of file. Similarly, the word "love" appears in three documents: docID 1 at positions 7 and 92, docID 3 at position 16, and docID 4 at positions 18, 21, and 55.

Part B Instructions
The bulk of the work in this homework is in this step. We'll tackle it in parts.

Take a look at DocTable.h; this is the public interface to the module that builds up the docID-to-docname HashTable and the docname-to-docID HashTable. Make sure you understand the semantics of everything in that header file; note how we can now implement procedural-style class composition! We create a single DocTable structure contains both of these tables, so when you implement DocTable_Allocate(), you'll end up malloc'ing a structure that contains two HashTables, and you'll allocate each of those HashTables.
Take a look inside DocTable.c; this is our partially completed implementation. Be sure to read the full file. Your job, as always, is to look for the "STEP X" comments and finish our implementation. Once you've finished the implementation, re-compile and re-run the test_suite executable to see if you pass our tests. If not, go back and fix some bugs!
Now, take a look inside CrawlFileTree.h; this is the public interface to our file crawler module. Make sure you understand the semantics of everything in that header file. Next, read through the full CrawlFileTree.c and then complete our implementation. Once you're ready, re-compile and re-run the test_suite executable to see if you pass more tests. If not, go back and fix some bugs!
Finally, take a look inside MemIndex.h. This is the public interface to the module that builds up the in-memory inverted index. Make sure you understand the semantics of everything in that header file and note how we implement procedural-style inheritance! Next, read the full MemIndex.c, and then complete our implementation. (This is the most involved part of the assignment.) Once you're ready, re-compile and re-run the test_suite executable to see if you pass our tests. If not, go back and fix some bugs!
Once you've passed all of the tests, re-run the test suites under valgrind and make sure you don't have any memory leaks or other memory issues.
Part C: Query Processor
Now that you have a working inverted index, you're ready to build your search engine. The job of a search engine is to receive queries from a user, and return a list of matching documents in some rank order.

For us, a query is just a string of words, such as:

course friends my
The goal of our search engine is to find all of the documents that contain all of the words. So, if a document has the words "course" and "my" but not the word "friends," it shouldn't match.

To execute a query, first the query processor must split the query up into a list of words (the strtok_r() function is useful for this). Next, it looks up in the inverted index the list of documents that match the first word. This is our initial matching list.

Next, for each additional word in the query, the query processor uses the inverted index to get access to the HashTable that contains the set of matching documents. For each document in the matching list, the query processor tests to see if that document is also in the HashTable. If so, it keeps the document in the matching list, and if not, it removes the document from the matching list.

Once the processor has processed all of the words, it's done. Now, it just has to rank the results, sort the results by rank, and return the sorted result list to the user.

For us, our ranking function is very simple: given a document that matches against a query, we sum up the number of times each query word appears in the document, and that's our rank. So, if the user provides the query "foo" and that words appears on a document 5 times, the rank for that document given that query is 5. If the user provides a multi-word query, our sum includes the number of times each of the words in the query appears. So, if the user provides the query "foo bar", the word "foo" appears in the document 5 times, and the word "bar" appears 10 times, the rank of the document for that query is 15. The bigger the sum, the higher the rank, and therefore the "better" the search result.

Part C Instructions
We have provided a mostly empty skeleton file searchshell.c. It is up to you to complete it by implementing a program that uses the Linux console to interact with the user. When you run the query processor (called searchshell — you can try a working searchshell in the solution_binaries/ directory), it takes from a command-line argument the name of a directory to crawl. After crawling the directory and building the inverted index, it enters a query processing loop, asking the user to type in a search string and printing the results. If the user signals end-of-file when the program asks for a search string (i.e., control-D from the Linux terminal keyboard), the program should clean up any allocated resources – particularly memory – and shut down.

When you are ready, try running ./searchshell to interact with your program and see if your output matches the . Alternatively, compare your searchshell to the searchshell we provided in the solution_binaries/ directory. Note that our ranking function does not specify an ordering for two documents with the same rank. Documents that have the same rank may be listed in any order, and that might be different from the ordering in the sample transcript or produced by the solution_binaries version of searchshell.


HW3:

Goals
In this assignment, you will build on top of your Homework 2 implementation and move the inverted index to an on-disk index:

In Part A, you will write code that takes an in-memory inverted index produced by HW2 and writes it out to disk in an architecture-neutral format.
In Part B, you will write C++ code that walks through an on-disk index to service a lookup.
In Part C, you will write a query processor that serves queries from multiple on-disk indices.
While you do these parts, you are expected to fill in a bug journal which can be found in bug_journal.md.
As before, please read through this entire document before beginning the assignment, and please start early! As usual, there is a lot of code involved with HW3, and since this is your first serious attempt to use C++ you should expect to encounter a lot of problems along the way. Also, manipulating on-disk data is trickier than in-memory data structures, so plan for some time to get this part right.


Part A: Memory-to-File Index Marshaller
Keeping a search engine index in memory is problematic, since memory is expensive and also volatile. So, in Part A, you're going to write some C++ code that takes advantage of your HW2 implementation to first build an in-memory index of a file subtree, and then it will write that index into an index file in an architecture-neutral format.


What do we mean by architecture-neutral? Every time we need to store an integer in the file's data structure, we will store it in big endian representation. This is the representation that is conventionally used for portability, but the bad news is that this is the opposite representation than most computers you use: x86 computers are little endian. So, you will need to convert integers (whether 16-bit, 32-bit, or 64-bit) into big endian before writing them into the file. We provide you with helper functions to do this.

The good news is that we're going to keep roughly the same data structure inside the file as you built up in memory: we'll have chained hash tables that are arrays of buckets containing linked lists. And, our inverted index will be a hash table containing a bunch of embedded hash tables. But, we need to be very precise about the specific layout of these data structures within the file. So, let's first walk through our specification of an index file's format. We'll do this first at a high level of abstraction, showing the major components within the index file. Then, we'll zoom into these components, showing additional details about each.

At a high-level, the index file looks like the figure on the right. The index file is split into three major pieces: a header, the doctable, and the index. We'll talk about each in turn.

Header
An index file's header contains metadata about the rest of the index file:

The first four bytes of the header are a , or format indicator. Specifically, we use the 32-bit number 0xCAFEF00D. We will always write the magic number out as the last step in preparing an index file. This way, if the program crashes partway through writing one, the magic number will be missing, and it will be easy to tell that the index file is corrupt.
The next four bytes are a  of the doctable and index regions of the file. A checksum is a mathematical signature of a bunch of data, kind of like a hash value. By including a checksum of most of the index file within the header, we can tell if the index file has been corrupted, such as by a disk error. If the checksum stored in the header doesn't match what we recalculate when opening an index file, we know the file is corrupt and we can discard it.
The next four bytes store the size of the doctable region of the file. The size is stored as a 32-bit, signed, big endian integer.
The final four bytes of the header store the size of the index region of the file, in exactly the same way.

Doctable

Let's drill down into the next level of detail by examining the content of the doctable region of the file. The doctable is a hash table that stores a mapping from 64-bit document ID to an ASCII string representing the document's filename. This is the docid_to_docname HashTable that you built up in HW2, but stored in the file rather than in memory.

The doctable consists of three regions; let's walk through them, and then drill down into some details:

num_buckets: This region is the simplest; it is just a 32-bit big endian integer that represents the number of buckets inside the hash table, exactly like you stored in your HashTable.
An array of bucket_rec records: This region contains one record for each bucket in the hash table. A bucket_rec record is 8 bytes long, and it consists of two four-byte fields. The chain len field is a four byte integer that tells you the number of elements in the bucket's chain. (This number might be zero if there are no elements in that chain!) The bucket position field is a four-byte integer that tells you the offset of the bucket data (i.e., the chain of bucket elements) within the index file. The offset is just like a pointer in memory or an index of an array, except that it points within the index file. For example, an offset of 0 would indicate the first byte of the file, an offset of 10 would indicate the 11th byte of the file, and so on.
An array of buckets: This region contains one bucket for each bucket in the hash table. A bucket is slightly more complex; it is a little embedded data structure. Specifically, each bucket contains:
An array of element positions: Since elements are variable-sized, rather than fixed-sized, we need to know where each element of the bucket lives inside the bucket. For each element, we store a four-byte integer containing the position (i.e., offset) of the element within the index file.
An array of elements: At each position specified in the element positions array lives an element. Since this is the docid-to-filename hash table, an element contains a 64-bit document ID and a filename. The document ID is an unsigned, big endian integer. Next, we store a 16-bit (2-byte) signed, big endian integer that contains the number of characters in the file name. Finally, we store the filename characters (each character is a single ASCII byte). Note that we do NOT store a null-terminator at the end of the filename; since we have the filename length in an earlier field, we don't need it!

Index

The index is the most complicated of the three regions within the index file. The great news is that it has pretty much the same structure as the doctable: it is just a hash table, laid out exactly the same way. The only part of the index that differs from the doctable is the structure of each element, so let's focus on that.

An index maps from a word to an embedded docID hash table, or docID table. So, each element of the index contains enough information to store all of that. Specifically, an index table element contains:

A two-byte signed integer that specifies the number of characters in the word.
A four-byte signed integer that specifies the number of bytes in the embedded docID table.
An array of ASCII characters that represents the word; as before, we don't store a NULL terminator at the end.
Some variable number of bytes that represents the docID table.
So, all we need to understand now is the format of the docID table. We're sure its format will come as no surprise at this point...


docIDtable

Like the "doctable" table, each embedded "docIDtable" table within the index is just a hash table! A docIDtable maps from a 64-bit docID to a list of positions with that document that the word can be found in. So, each element of the docID table stores exactly that:

A 64-bit (8-byte) unsigned integer that represents the docID.
A 32-bit (4-byte) signed integer that indicates the number of word positions stored in this element.
An array of 32-bit (4-byte) signed integers, sorted in ascending order, each one containing a position within the docID that the word appears in.

So, putting it all together, the entire index file contains a header, a doctable (a hash table that maps from docID to filename), and an index. The index is a hash table that maps from a word to an embedded docIDtable. The docIDtable is a hash table that maps from a document ID to a list of word positions within that document.

Part A Instructions
The bulk of the work in this homework is in this step. We'll tackle it in parts.

Change to the directory containing your CSE333 GitLab repository. Use git pull to retrieve the new hw3/ folder for this assignment. You still will need the hw1/, hw2/, and projdocs/ directories in the same folder as your new hw3 folder since hw3/ links to files in those previous directories, and the test_tree folder also still needs to be present.
Look around inside of hw3/ to familiarize yourself with the structure. Note that there is a libhw1/ directory that contains a symlink to your libhw1.a, and a libhw2/ directory that contains a symlink to your libhw2.a. You can replace your libraries with ours (from the appropriate solution_binaries/ directories) if you prefer.
Next, run make to compile the three HW3 binaries. One of them is the usual unit test binary. Run it, and you'll see the unit tests fail, crash out, and you won't yet earn the automated grading points tallied by the test suite.
Now, take a look inside Utils.h and LayoutStructs.h. These header files contains some useful utility routines and classes you'll take advantage of in the rest of the assignment. We've provided the full implementation of Utils.cc. Next, look inside WriteIndex.h; this header file declares the WriteIndex() function, which you will be implementing in this part of the assignment. Also, look inside buildfileindex.cc; this file makes use of WriteIndex() and your HW2 CrawlFileTree(), to crawl a file subtree and write the resulting index out into an index file. Try running the solution_binaries/buildfileindex program to build one or two index files for a directory subtree, and then run the solution_binaries/filesearchshell program to try out the generated index file.
Finally, it's time to get to work! Open up WriteIndex.cc and take a look around inside. It looks complex, but all of the helper routines and major functions correspond pretty directly to our walkthrough of the data structures above. Start by reading through WriteIndex(); we've given you part of its implementation. Then, start recursively descending through all the functions it calls, and implement the missing pieces. (Look for STEP: in the text to find what you need to implement.)
Once you think you have the writer working, compile and run the test_suite as a first step. Next, use your buildfileindex binary to produce an index file (we suggest indexing something small, like ./test_tree/tiny as a good test case). After that, use the solution_binaries/filesearchshell program that we provide, passing it the name of the index file that your buildfileindex produces, to see if it's able to successfully parse the file and issue queries against it. If not, you need to fix some bugs before you move on!
Aside: If you write the index files to your personal directories on a CSE lab machine or on attu, you may find that the program runs very slowly. That's because home directories on those machines on a network file server, and buildfileindex does a huge number of small write operations, which can be quite slow over the network. To speed things up dramatically we suggest you write the index files into /tmp, which is a directory on a local disk attached to each machine. Be sure to remove the files when you're done so the disk doesn't fill up.
As an even more rigorous test, try running the hw3fsck program we've provided in solution_binaries against the index that you've produced. hw3fsck scans through the entire index, checking every field inside the file for reasonableness. It tries to print out a helpful message if it spots some kind of problem. Once you pass hw3fsck and once you're able to issue queries against your file indices, then rerun your buildfileindex program under valgrind and make sure that you don't have any memory leaks or other memory errors.
Part B: Index Lookup
Now that you have a working memory-to-file index writer, the next step is to implement code that knows how to read an index file and lookup query words and docids against it. We've given you the scaffolding of the implementation that does this, and you'll be finishing our implementation.

Part B Instructions
Start by looking inside FileIndexReader.h. Notice that we're now in full-blown C++ land; you'll be implementing constructors, manipulating member variables and functions, and so on. Next, open up FileIndexReader.cc. Your job is to finish the implementation of its constructor, which reads the header of the index file and stores various fields as private member variables. As above, look for "STEP:" to figure out what you need to implement. When you're done, recompile and re-run the test suite. You should pass all the tests for test_fileindex_reader.cc once you have implemented FileIndexReader successfully.
Next, move on to HashTableReader.h. Read through it to see what the class does. Don't worry about the copy constructor and assignment operator details (though if you're curious, read through them to see what they're doing and why). This class serves as a base class for other subclasses. The job of a HashTableReader is to provide most of the generic hash-table lookup functionality; it knows how to look through buckets and chains, returning offsets to elements associated with a hash value. Open up HashTableReader.cc. Implement the "STEP:" components in the constructor and the LookupElementPositions function. When you're done, recompile and run the unit tests to see if you pass test_hashtablereader.cc unit test.
Now it's time to move on to DocTableReader.h. Read through it, and note that it is a subclass of HashTableReader. It inherits LookupElementPositions() and other aspects, but provides some new functionality. Next, open up DocTableReader.cc, and implement the "STEP:" functionality. See how well you do on its unit test (and valgrind) when you're done.
Next, lets move on to IndexTableReader.h. Read through it and understand its role. Next, open up IndexTableReader.cc and implement the "STEP:" functionality. Test against the unit tests (and valgrind).
Next, do the same with DocIDTableReader.h and DocIDTableReader.cc.
We're almost there! Open up QueryProcessor.h and read through how it is supposed to work. Check out test_queryprocessor.cc for more information. Now open up QueryProcessor.cc and read through our implementation of the constructor and destructor.
This part of the assignment is the most open-ended. We've given you the function definition for ProcessQuery(), and also a clue about what you should be building up and returning. But, we've given you nothing about its implementation. You get to implement it entirely on your own; you might want to define helper private member functions, you might want to define other structures to help along the way, etc.; it's entirely up to you. But, once you're finished, you'll need to pass our unit test to know you've done it correctly.
Hint #1: You should be able to take inspiration from what you did to implement the query processor in HW2. Here, it's only a little bit more complicated. You want to process the query against each index, and then intersect each index's results together and do a final sort (use the STL's sort). Remember that processing a query against an index means ensuring all query words are present in each matching document, and remember how ranking works. Then, once you have query results from each index, you'll append them all together to form your final query results.
Hint #2: Once you think you have this working, move on to Part C and finish our filesearchshell implementation. You'll be able to test the output of your filesearchshell against ours (in solution_binaries/) as a final sanity check.
Now would be a great time to run valgrind over the unit tests to verify you have no memory leaks or memory errors.
Part C: Search Shell
For Part C, your job is to implement a search shell, just like in HW2, but this time using your HW3 infrastructure you completed Parts A and B.

Open up filesearchshell.cc and read through it. Note that unlike Parts A and B, we have given you almost nothing about the implementation of the filesearchshell besides a really long (and hopefully helpful) comment. Implement filesearchshell.cc.
Try using your filesearchshell binary. You can compare the output of your binary against a . The transcripts should match precisely, except perhaps for the order of equally ranked matches. You can also walk your filesearchshell against a very tiny index (tiny.idx) in the debugger to see if it's reading the correct fields and jumping to the correct offsets.
Also, note that you can hit control-D to exit the filesearchshell. filesearchshell ought to clean up all allocated memory before exiting. So, run your filesearchshell under valgrind to ensure there are no leaks or errors.




HW4:

Goals
In this assignment, you will build on top of your Homework 3 implementation to complete a multithreaded web server front-end to your query processor.

In Part A, you will read through some of our code to learn about the infrastructure we have built for you.
In Part B, you will complete some of our classes and routines to finish the implementation of a simple web server.
In Part C, you will fix some security problems in our web server.
While you do these parts, you are expected to fill in a bug journal which can be found in bug_journal.md.
As before, please read through this entire document before beginning the assignment, and please start early!


Read over the project specifications and understand which code is responsible for what.
Finish ServerSocket.cc. Make sure to cover all functionality, not just what is in the unit tests.
Implement FileReader.cc, which should be very easy, and GetNextRequest() in HttpConnection.cc.
Complete ParseRequest() in HttpConnection.cc. This can be tricky, as it involves both Boost and string parsing.
Finish the code for http333d.cc. Implement HttpServer_ThrFn() in HttpServer.cc.
Complete ProcessFileRequest() and ProcessQueryRequest() in HttpServer.cc. At this point, you should be able to search the "333gle" site and view the webpages available under /static/, e.g., http://localhost:5555/static/bikeapalooza_2011/index.html.
Fix the security issues with the website, if you have any.
Make sure everything works as it is supposed to.
Part A: Read Through Our Code

Our web server is a fairly straightforward multithreaded application. Every time a client connects to the server, the server dispatches a thread to handle all interactions with that client. Threads do not interact with each other at all, which greatly simplifies the design of the server.

The figure to the right shows the high-level architecture of the server. There is a main class called HttpServer that uses a ServerSocket class to create a listening socket, and then sits in a loop waiting to accept new connections from clients. For each new connection that the HttpServer receives, it dispatches a thread from a ThreadPool class to handle the connection. The dispatched thread springs to life in a function called HttpServer_ThrFn() within the HttpServer.cc file.



The HttpServer_ThrFn() function handles reading requests from one client. For each request that the client sends, the HttpServer_ThrFn() invokes GetNextRequest() on the HttpConnection object to read in the next request and parse it.

To read a request, the GetNextRequest() method invokes WrappedRead() some number of times until it spots the end of the request. To parse a request, the method invokes the ParseRequest() method (also within HttpConnection). At this point, the HttpServer_ThrFun() has a fully parsed HttpRequest object (defined in HttpRequest.h).



The next job of HttpServer_ThrFn() is to process the request. To do this, it invokes the ProcessRequest() function, which looks at the request URI to determine if this is a request for a static file, or if it is a request associated with the search functionality. Depending on what it discovers, it either invokes ProcessFileRequest() or ProcessSearchRequest().

Once those functions return an HttpResponse, the HttpServer_ThrFn() invokes the WriteResponse() method on the HttpConnection object to write the response back to the client.


Our web server isn't too complicated, but there is a fair amount of plumbing to get set up. In this part of the assignment, we want you to read through a bunch of lower-level code that we've provided for you. You need to understand how this code works to finish our web server implementation, but we won't have you modify this plumbing.

Part A Instructions
Change to the directory containing your CSE333 GitLab repository. Use git pull to retrieve the new hw4/ folder for this assignment. You will need the hw1/, hw2, hw3/, and projdocs/ directories in the same folder as your new hw4/ folder since hw4 links to files in those previous directories.
Look around inside of hw4/ to familiarize yourself with the structure. Note that there are libhw1/, libhw2/, and libhw3/ directories that contain symlinks to your libhw1.a, libhw2.a, and libhw3.a, respectively. You can replace your libraries with ours (from the appropriate solution_binaries directories) if you prefer.
Next, run make to compile the two HW4 binaries. One of which is the usual unit test binary. Run it, and you'll see the unit tests fail, crash out, and you won't yet earn the automated grading points tallied by the test suite.
The second binary is the web server itself: http333d. Its usage message will reveal its command-line arguments; an example call looks like:
$ ./http333d 5555 ../projdocs unit_test_indices/*
In the meantime, start up a working web server using the provided solution binary:
$ ./solution_binaries/http333d 5555 ../projdocs unit_test_indices/*
You might need to pick a different port than 5555 if someone else is using that port on the same machine as you.
Use a web browser to explore what the server should look like when it's finished:
If you are running the code on a lab computer or the CSE home VM, launch a browser on that machine and open http://localhost:5555/ and http://localhost:5555/static/bikeapalooza_2011/Bikeapalooza.html in different tabs, changing the 5555 to the port you specified when launching http333d.
If you are running the code on attu, note which specific machine you are running the web server on (e.g., attu4) and open http://attu4.cs.washington.edu:5555/ and http://attu4.cs.washington.edu:5555/static/bikeapalooza_2011/Bikeapalooza.html in different tabs, changing the attu number and port number as needed.
Enter a few search queries in the first tab and then click around the Bikeapalooza gallery in the second tab; this is what your finished web server will be capable of!

When you are done with the http333d server, the most graceful way to shut it down is to open another terminal window on the same machine, run the command:
$ ps -u
to find the process id (pid) of the web server, and then run:
$ kill pid
You can also type Control-C in the terminal window where the web server is running, but this isn't quite as graceful and doesn't always work as reliably as a kill command.
Read through ThreadPool.h and ThreadPool.cc. You don't need to implement anything in either, but several pieces of the project rely on this code. The header file is well-documented, so it ought to be clear how it's used. There's also a unit test file that you can peek at.
Read through HttpUtils.h and HttpUtils.cc. This class defines a number of utility functions that the rest of HW4 uses. You will have to implement some of these utilites while completing test_suite. Make sure that you understand what each of the utilities do, and why we may want them.
Finally, read through HttpRequest.h and HttpResponse.h. These files define the HttpRequest and HttpResponse classes, which represent a parsed HTTP request and response, respectively.
Part B: Basic Web Server
You are now going to finish a basic implementation of the http333d web server. You will need to implement some of the event handling routines at different layers of abstraction in the web server, culiminating with generating HTTP and HTML to send to the client.

Part B Instructions
Take a look at ServerSocket.h. This file contains a helpful class for creating a server-side listening socket, and accepting a new connection from a client. We've provided you with the class declaration in ServerSocket.h but no implementation in ServerSocket.cc; your next job is to build it. You'll need to make the code handle both IPv4 and IPv6 clients. Run the test suite to see if you make it past the ServerSocket unittests.
Read through FileReader.h and FileReader.cc. Note that the implementation of FileReader.cc is missing; go ahead and implement it. See if you make it past the FileReader unittests.
Read through HttpConnection.h and HttpConnection.cc. The two major functions in HttpConnection.cc have their implementations missing, but have generous comments for you to follow. Implement the missing functions, and see if you make it past the HttpConnection unittests.
Read through HttpUtils.h and HttpUtils.cc. There are two functions in HttpUtils.cc that have their implementations missing, but have generous comments to help you figure out their implementation. Implement the missing functions, and see if you make it past the HttpUtils unittests.
Now comes the hardest part of the assignment.
Read through HttpServer.cc, HttpServer.h, and http333d.cc. Note that some parts of HttpServer.cc and http333d.cc are missing. Go ahead and implement those missing functions. The only requirement here is that your web server mimics the same behavior (i.e., have a search bar, process files and queries correctly, and show their results similarly) as the solution binaries; although entirely optional, you are free to modify the look of your 333gle site:
If you just want to get the same "look and feel" of our server, you can use the solution binary and then view source to see the HTML to emulate.
In the past, some students implemented 333gle in "dark mode", had a Shrek theme, etc.
If you want to add more features that are more complex than altering apperance, check out the Bonus below.
Once you have the functions implemented, test your http333d binary to see if it works by running the web server and connecting to it from a browser (as described in Part A Step 5 above), exercising both the web search and static file serving functionalities.

At this point, your web server should run correctly, and everything should compile with no warnings. Try running the test_suite under valgrind to make sure there are no memory issues. Finally, launch the web server under valgrind to make sure there are no issues or leaks: after the web server has launched, exercise it by issuing a few queries, then kill the web server.
The supplied code DOES have some memory leaks, but your code should not make things significantly worse.
Part C: Fix Security Vulnerabilities
Now that the basic web server works, you will discover that your web server (probably) has two security vulnerabilities. We are going to point these out to you, and you will repair them. Of course, it IS possible that the way you implemented things above means you have already dealt with these flaws.

You may find that some of the functions defined in HttpUtils.cc will be very helpful in fixing these security flaws in your web server.
Part C Instructions
Fix the following two security flaws, if currently found in your server. As a point of reference, we've provided a version of our web server that has both of these flaws in place (solution_binaries/http333d_withflaws). Feel free to try it out, but DO NOT leave this server running, as it will potentially expose all of your files to anybody that connects to it.

The first is called a . Using Firefox or Safari (Chrome prevents this attack), try typing the following query into both your web server and the solution binary web server and compare the behavior of the two:
hello <script>alert("Boo!");</script>
To fix this flaw, you need "escape" untrusted input from the client before you relay it to output.
Use telnet to connect to your web server and manually send a request for the following URI. (Note: browsers are smart enough to help defend against this attack, so you can't just type it into a browser URL bar, but nothing prevents attackers from directly connecting to your server with a program of their own!)
/static/../hw4/http333d.cc
This is called a . Instead of trusting the file pathname provided by a client, you need to normalize the path and verify that it names a file within your document subdirectory tree (which would be ../projdocs/ if the example command shown in Part A was used to start the server). If the provided path names something outside of that subdirectory, you should return an error message instead of the file contents.




Bug Journal
You are expected maintain a bug journal as you work on each homework this quarter. This is a lightweight way to help you focus on how you approach debugging and should not add much time, assuming that you fill it out as you encounter bugs. In fact, its intent is to save you time overall by improving your debugging skills!

In each homework, you will find a file in the directory called bugjournal.md with a skeleton for the prompts that you will find below for you to fill out. You may treat this file as a standard text file.

Bug Journal Prompts
For at least three bugs that you (and your partner) encounter during this homework, track your progress in solving them by responding to the following prompts:

What is the current (incorrect) behavior and what is the expected behavior?
Examples:
I am getting a segault at line 100, which is supposed to move a pointer to the next node in a linked list.
My output from function_name() doesn't match the solution binaries.
I am getting a memory leak of 128 bytes allocated in function_name(), line 140.
Brainstorm a few possible causes of this bug. We just want ideas; you don't have to investigate these yet. Possible causes often start with phrases like "I think...", "Maybe", or "If X happens, then it would cause Y to fail." Try to be as specific as you can.
