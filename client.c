#include <unistd.h>
#include <signal.h>

void    handle_server_pid(int signum, int *server_pid)
{
    int i = 0;
    static int bit = 0;

    if (signum == SIGUSR1)
        i = i | (1 << bit);
    bit++;
    if (bit == 32)
    {
        server_pid = i;
        bit = 0;
        i = 0;
        char msg[] = "Server PID received\n";
        write(1, msg, sizeof(msg) - 1);
    }
}

 void sig_handler(int signum)
{
    
    static int bit = 0;
    static char c = 0;
    static int server_pid = 0;
    

    handle_server_pid(signum, &server_pid);
    if (server_pid)
    {
        /* code */
    }
    
    if (signum == SIGUSR1)
        c = c | (1 << bit);
    bit++;
    if (bit == 7)
    {
        write(1, &c, 1);
        bit = 0;
        c = 0;
    }
}

void    print_pid()
{
    int pid;

    pid = getpid();
    write(1, "PID: ", 5);
    char buffer[20];
    int len = 0;
    int temp = pid;
    while (temp)
    {
        buffer[len++] = (temp % 10) + '0';
        temp /= 10;
    }
    while (len)
        write(1, &buffer[--len], 1);
    write(1, "\n", 1);  
}

int main(int ac, char **av)
{
    if (ac != 1)
        return (0);
    print_pid();
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    while (1)
        ;
    return (0);
}