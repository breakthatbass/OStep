

#define plock(lock) (if (pthread_mutex_lock(lock) != 0) perror("Pthread_mutex_lock"); exit(EXIT_FAILURE))

#define errmsg(str) perror(str); exit(EXIT_FAILURE)