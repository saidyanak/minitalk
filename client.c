#include <signal.h>
#include <unistd.h>

volatile int	g_flag = 0;

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
		usleep(200);
	}
}

void	send_char(char c, int server_pid)
{
	int	i;

	i = 8;
	while (i)
	{
		i--;
		g_flag = 0;
		if (c & (1 << i))
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		while (!g_flag)
			;
	}
}

void	send_message(char *message, int server_pid)
{
	while (*message)
	{
		send_char(*message, server_pid);
		message++;
	}
	send_char('\0', server_pid);
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
	print_pid(getpid());
	server_pid = ft_atoi(av[1]);
	g_flag = 0;
	send_client_pid(getpid(), server_pid);
	while (!g_flag)
		usleep(50);
	send_message(av[2], server_pid);
	return (0);
}
