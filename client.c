#include <signal.h>
#include <unistd.h>

int		g_flag = 0;

int	ft_atoi(char *str)
{
	int	result;

	result = 0;
	while (*str)
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result);
}

void	handle_signal(int sig)
{
	(void)sig;
	g_flag = 1;
}

void	send_client_pid(int client_pid, int server_pid)
{
	int	bit;

	bit = 32;
	while (bit)
	{
		bit--;
		if (client_pid & (1 << bit))
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		usleep(20);
	}
}

void	send_message(char *message, int server_pid)
{
	int	i;

	while (*message)
	{
		i = 8;
		while (i)
		{
			i--;
			g_flag = 0;
			if (*message & (1 << i))
				kill(server_pid, SIGUSR1);
			else
				kill(server_pid, SIGUSR2);
			while (!g_flag)
				pause();
		}
		message++;
	}
	i = 8;
	while (--i > 0)
	{
		kill(server_pid, SIGUSR2);
		g_flag = 0;
		while (!g_flag)
				pause();
	}
}

int	main(int ac, char **av)
{
	int	server_pid;

	if (ac != 3)
	{
		write(2, "Usage: ./client <pid> <message>\n", 31);
		return (1);
	}
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	server_pid = ft_atoi(av[1]);
	g_flag = 0;
	send_client_pid(getpid(), server_pid);
	send_message(av[2], server_pid);
	return (0);
}
