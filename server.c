#include <signal.h>
#include <unistd.h>

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

int	handle_client_signal(int signal, int *client_pid)
{
	static int	bit = 32;

	if (bit == 0)
		return (1);
	if (signal == SIGUSR1)
		*client_pid =  *client_pid | (1 << bit);
	bit--;
	if (bit == 0)
	{
		bit = 32;
		return (1);
	}
	return (0);
}

void	handle_signal(int signal)
{
	static int	client_pid = 0;

	if (handle_client_signal(signal, &client_pid))
	{
		// client e selam yollamalıyım testingen doğru - li kısımlarıa bakıcam
		print_pid(client_pid);
	}
}

int	main(void)
{
	int pid;

	pid = getpid();
	print_pid(pid);
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	while (1)
		pause();
}