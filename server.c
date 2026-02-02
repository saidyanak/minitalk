/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:34:29 by syanak            #+#    #+#             */
/*   Updated: 2026/02/02 11:09:52 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

int		g_client_pid = 0;

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

void	receive_pid(int sig)
{
	static int	bit = 32;
	static int	pid = 0;

	bit--;
	if (sig == SIGUSR1)
		pid = pid | (1 << bit);
	if (bit == 0)
	{
		g_client_pid = pid;
		pid = 0;
		bit = 32;
		kill(g_client_pid, SIGUSR1);
	}
}

void	receive_msg(int sig)
{
	static int				msg_bit = 8;
	static unsigned char	byte = 0;

	msg_bit--;
	if (sig == SIGUSR1)
		byte = byte | (1 << msg_bit);
	if (msg_bit == 0)
	{
		if (byte == '\0')
		{
			write(1, "\n", 1);
			kill(g_client_pid, SIGUSR1);
			g_client_pid = 0;
			byte = 0;
			msg_bit = 8;
			return ;
		}
		write(1, &byte, 1);
		byte = 0;
		msg_bit = 8;
	}
	kill(g_client_pid, SIGUSR1);
}

void	handle_signal(int sig)
{
	if (g_client_pid == 0)
		receive_pid(sig);
	else
		receive_msg(sig);
}

int	main(void)
{
	print_pid(getpid());
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	while (1)
		pause();
}
