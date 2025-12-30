#include <unistd.h>
#include <signal.h>

static int get_client_pid(char *str)
{
	int pid = 0;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			write(1, "Invalid PID\n", 12);
			return (0);
		}
		pid = pid * 10 + (*str - '0');
		str++;
	}
	return (pid);
}

static void send_message(int pid, char *str)
{
	int i;
	char c;
	while (*str)
	{
		c = *str;
		i = 0;
		while (i < 7)
		{
			if (c & (1 << i))
			{
				if (kill(pid, SIGUSR1) == -1)
				{
					write(1, "Failed to send signal\n", 22);
					return ;
				}
			}
			else
			{
				if (kill(pid, SIGUSR2) == -1)
				{
					write(1, "Failed to send signal\n", 22);
					return ;
				}
			}
			usleep(100);
			i++;
		}
		str++;
	}
}

int main(int ac, char **av)
{
	if (ac < 2)
		return (0);
	int pid;

	pid = get_client_pid(av[1]);
	if (pid)
		send_message(pid, av[2]);
	return (0);
}