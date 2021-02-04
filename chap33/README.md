## Chapter 33
# Event-Based Concurrency (Advanced)

The homework for this chapter mainly revolves around network programming which the book doesn't go into as it doesn't exactly fit in an OS class. It suggests doing your own research. Network programming is typically an entire class in itself in a CS undergrad. It's a large topic and requires more than a quick StackOverflow search. Reading through [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf) covers more than you will need for the homework here.  

#

1. First, write a simple server that can accept and serve TCP connections. You'll have to poke around the internet a bit if you don't already know how to do this. Build this to serve exactly one request at a time; have each request be very simple, e.g., to get the current time of day.

    [server.c](https://github.com/breakthatbass/OStep/blob/main/chap33/server.c)
    This server just attempts to connect to a client and if it does, it'll send the current time of day and the date. 

    This is just a server, so you'll need a client of of some sort to connect to it so it can send the data through. `telnet` is simple enough. If you don't have it, `brew install telnet`. First run the server, the run `telnet` with:
    ```
    telnet hostname port
    ```
    To get your hostname, on MacOS and Linux, it can be retrieved with the `hostname` command. For port the server is set to port 3490. 

