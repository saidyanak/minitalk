#include <signal.h>
#include <unistd.h>

int		flag = 1;

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

void	send_client_pid(int client_pid, int server_pid)
{
	int	pid_byte;

	pid_byte = 32;
	while (pid_byte)
	{
		if (client_pid & (1 << pid_byte))
		{
			if (kill(server_pid, SIGUSR1))
			{
				write(2, "hata", 5);
				return ;
			}
		}
		else if (kill(server_pid, SIGUSR2))
		{
			write(2, "hata", 5);
			return ;
		}
		usleep(10000);
		pid_byte--;
	}
}

void	hanle_signal(int signal)
{
	if (!flag)
		flag = 1;
    else
        flag = 0
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

void	send_char(char c, int server_pid)
{
	int	i;

	i = 8;
	while (i)
	{
		if (c & (1 << i))
		{
			if (kill(server_pid, SIGUSR1))
			{
				write(2, "hata", 5);
				return ;
			}
		}
		else if (kill(server_pid, SIGUSR2))
		{
			write(2, "hata", 5);
			return ;
		}   
        flag = 1;
        while (flag)
			;
		i--;
	}
}

void	send_message(char *message, int server_pid)
{
	while (*message)
	{
		send_char(*message, server_pid);
		message++;
	}
}

int	main(int ac, char **av)
{
	int server_pid;

	print_pid(getpid());
	if (ac != 3)
	{
		write(2, "Argümanları", 15);
		return (1);
	}
	signal(SIGUSR1, hanle_signal);
	signal(SIGUSR2, hanle_signal);
	server_pid = ft_atoi(av[1]);
	send_client_pid(getpid(), server_pid);
	send_message(av[2], server_pid);
	return (0);
}