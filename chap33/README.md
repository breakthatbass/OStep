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

2. Now, add the `select()` interface. Build a main program that can accept multiple connections, and an event loop that checks which file descriptors have data on them, and then read ond process those requests. Make sure to carefully test that you are using `select()` correctly.

    [select.c](https://github.com/breakthatbass/OStep/blob/main/chap33/select.c)


3. Next, let's make the requests a little more interesting, to mimic a simple web or file server. Each request should be to read teh contents of a file (named in the request), and the server should respond by reading the file into a buffer, the returning the contents to the client. Use the standard `open()`, `read()`, `close()` system calls to implement this feature. Be a little careful here: If you leave this running for a long time, someone may figure out how to use it to read all the files on your computer!

    [`file-server.c`](https://github.com/breakthatbass/OStep/blob/main/chap33/file-server.c)  
    [`file-client.c`](https://github.com/breakthatbass/OStep/blob/main/chap33/file-client.c)  

    - compile:  
        ```
        gcc -o file-server file-server.c
        gcc -o file-client file-client.c
        ```
    - usage:
        ```
        ./file-server   // for now, the server port is 3490
        ./file-client <hostname> <port>
        ```
    - Run the server first then run the client. It will alert when they successfully connect. 
    
    - Type a file path into the client and hit enter. If the server can successfully `open` and `read` from the file, the client will be able to display the contents of the file.

    - BUGS: the client is only able to send and recieve one file at a time. You must exit the client and start it up again to read from more files.

4. Now, instead of using standard I/O system calls, use the asynchronous I/O interfaces as described in the chapter. How hard was it to incorporate asynchronus interfaces into your program?  

    - Pretty simple. Basically you just put your file info into the `aiocb` struct and `aio_read()` takes that struct as an argument. There is a little more setup work compared to the normal `read()` though.  Other than the function below, I just replaced `read` with `aio_read`.

    ```c
    // function to fill the struct
    struct aiocb *aio_init(int fd, int bufsize, char *buf)
    {
        struct aiocb *n = malloc(sizeof(struct aiocb));
        if (n == NULL) {
            perror("aio_int: malloc");
            exit(EXIT_FAILURE);
        }

        n->aio_fildes = fd;
        n->aio_buf = buf;
        n->aio_nbytes = bufsize;

        return n;
    }
    ```

5. For fun, add some signal handling to your code. One common use of signals is to poke a server to reload some kind of configuration file, or take some other kind of administrative action. Perhaps one natural way is to play around with this is to add a user-level file cache to your server, which stores recently accessed files. Implement a signal handler that clears the cache when the signal is sent to the server process.

    - [cache-server.c](https://github.com/breakthatbass/OStep/blob/main/chap33/cache-server.c)
    - signals can be sent to a program with the `signal()` function. According to Rago & Stevens in *Advanced Programming in the UNIX Environment*, `select()` is "almost useless for UNIX systems" and suggests using `sigaction()`. I use `select()` because it's super simple and achieves the goal of the question.
    - Interface for `select()`:  
    ```c
    void (*signal(int signo, void (*func)(int)))(int);
    ```
    This nonsense just takes a SIGNAL as the first param and a function for the second. The function is just a function that you want to trigger when the signal is sent.

    ```c
    // In the main loop
    if (signal(SIGINT, clear_cache) == SIG_ERR) {
			perror("signal");
			exit(EXIT_FAILURE);
		}
    ```
    - to send the interrupt signal, just hit `cntrl-c` in the server's terminal.

