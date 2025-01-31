/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 19:07:58 by syanak            #+#    #+#             */
/*   Updated: 2025/01/31 19:09:41 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

void	ft_putnbr(int n)
{
	if (n > 9)
		ft_putnbr(n / 10);
	write(1, &"0123456789"[n % 10], 1);
}

int	ft_write_message(int client_signal)
{
	static int	bit_message = 1;
	static int	message = 0;

	if (client_signal == SIGUSR1)
		message += bit_message;
	bit_message *= 2;
	if (bit_message == 256)
	{
		write(1, &message, 1);
		bit_message = 1;
		if (message == 0)
			return (write(1, "\n", 1), 1);
		message = 0;
	}
	return (0);
}

void	ft_handler(int client_signal)
{
	static int	bit_pid = 0;
	static int	client_pid = 0;
	int			control;

	control = 0;
	if (bit_pid < 32)
	{
		if (client_signal == SIGUSR1)
			client_pid += 1 << bit_pid;
		bit_pid++;
	}
	else
	{
		control = ft_write_message(client_signal);
		if (control)
		{
			kill(client_pid, SIGUSR1);
			bit_pid = 0;
			client_pid = 0;
		}
	}
}

int	main(void)
{
	ft_putnbr(getpid());
	write(1, "\n", 1);
	signal(SIGUSR1, ft_handler);
	signal(SIGUSR2, ft_handler);
	while (1)
	{
		usleep(10);
		pause();
	}
	return (0);
}
