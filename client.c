/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:34:34 by syanak            #+#    #+#             */
/*   Updated: 2026/02/02 13:05:43 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

int		g_flag = 0;

int	ft_atoi(char *str)
{
	int	result;

	result = 0;
	if (*str == '-')
		return (0);
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

int	send_client_pid(int client_pid, int server_pid)
{
	int	bit;

	bit = 32;
	while (bit)
	{
		bit--;
		if (client_pid & (1 << bit))
		{
			if (kill(server_pid, SIGUSR1) == -1)
				return (1);
		}
		else
		{
			if (kill(server_pid, SIGUSR2) == -1)
				return (1);
		}
		usleep(20);
	}
	return (0);
}

int	send_byte(char byte, int server_pid)
{
	int	i;

	i = 8;
	while (i)
	{
		i--;
		g_flag = 0;
		if (byte & (1 << i))
		{
			if (kill(server_pid, SIGUSR1) == -1)
				return (1);
		}
		else
		{
			if (kill(server_pid, SIGUSR2) == -1)
				return (1);
		}
		while (!g_flag)
			;
	}
	return (0);
}

int	main(int ac, char **av)
{
	int	server_pid;
	int	i;

	i = 0;
	if (ac != 3)
	{
		write(2, "Usage: ./client <pid> <message>\n", 31);
		return (1);
	}
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	server_pid = ft_atoi(av[1]);
	if (server_pid == 0)
		return (1);
	g_flag = 0;
	if (send_client_pid(getpid(), server_pid))
		return (1);
	while (av[2][i])
	{
		if (send_byte(av[2][i], server_pid))
			return (1);
		i++;
	}
	send_byte(0, server_pid);
	return (0);
}
