#include <signal.h>
#include <unistd.h>

int	ft_atoi(char *str)
{
	int	index;

	index = 0;
	while (*str)
	{
		index = index * 10 + (*str - '0');
		str++;
	}
	return (index);
}

void    send_client_pid(int client_pid, int server_pid)
{
    int pid_byte;

    pid_byte = 32;
    while (pid_byte)
    {
        if (client_pid & (1 << pid_byte))
        {
            if (kill(server_pid, SIGUSR1))
            {
                write(2, "hata", 5);   
                return;
            }
        }
        else if (kill(server_pid, SIGUSR2))
        {
            write(2, "hata", 5);   
            return;
        }
        usleep(10000);
        pid_byte--;
    }
    
}


void	print_pid(int pid)
{
	char	buffer[20];
	int		len;
	int		temp;

	write(1, "PID: ", 5);
	len = 0;
	temp = pid;
	while (temp)
	{
		buffer[len++] = (temp % 10) + '0';
		temp /= 10;
	}
	while (len)
		write(1, &buffer[--len], 1);
	write(1, "\n", 1);
}

int	main(int ac, char **av)
{
	int server_pid;
	
    server_pid = ft_atoi(av[1]);

    print_pid(getpid());
    if (ac != 3)
	{
		write(2, "Argümanları", 15);
        return (1);
    }
    send_client_pid(getpid(), server_pid);
	return (0);
}