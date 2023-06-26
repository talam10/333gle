# Bug 1

## A) How is your program acting differently than you expect it to?
- After I finished HttpServer and tried running the 333gle and Bikeapalooza on web, it wasn't running and giving me a connection error

## B) Brainstorm a few possible causes of the bug
- Maybe I am implementing HttpServer wrong
- Maybe I am implementing HttpConnection wrong
- Maybe I am parsing the query or file correctly

## C) How you fixed the bug and why the fix was necessary
- With the help of the TA,I was able to figure out in HttpConnection.cc we weren't clearing/flushing the buffer at the end. Once I used buffer_substr() to flush the buffer, it worked and the fix was necessary because now I could see the query page on the web


# Bug 2

## A) How is your program acting differently than you expect it to?
- Once I was able to fix the connection, I figured my Bikeapalooza file was not loading

## B) Brainstorm a few possible causes of the bug
- Could be sonmething wrong with IsFilePathSafe function
- Could be something wrong with the way we were using IsFilePathSafe function
- I could be implementing HttpServer wrong

## C) How you fixed the bug and why the fix was necessary
- I fixed the bug by running GDB and figuring out we were using IsFilePathSafe function wrong. We were not passing full_file. Rather we were pasisng file_name. So it was giving us the error. Once we fixed the error, we were able to see Bikeapalooza html file. So the fix was necessary.


# Bug 3

## A) How is your program acting differently than you expect it to?
- We were getting a close_nointr(fd) != -EBADF error

## B) Brainstorm a few possible causes of the bug
- We could be missing a possible security safe check in HttpServer.cc
- We could be missing a possible security safe check in SocketServer.cc
- We could be missing a possible security safe check in http333d.cc

## C) How you fixed the bug and why the fix was necessary
- By the help pf the TAs, I was able to fix it by taking out HttpConnection hp out of the while loop. This was needed because we were getting the close_nointr(fd) != -EBADF error after running the http333d projdocs under valgrind. And it was aborting/core dumping. So the fix was needed for bikeapalooza.
