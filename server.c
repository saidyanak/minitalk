/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 19:07:37 by syanak            #+#    #+#             */
/*   Updated: 2025/01/31 19:07:39 by syanak           ###   ########.fr       */
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

void	ft_write_message(int client_signal)
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
			write(1, "\n", 1);
		message = 0;
	}
}

int	main(void)
{
	ft_putnbr(getpid());
	write(1, "\n", 1);
	signal(SIGUSR1, ft_write_message);
	signal(SIGUSR2, ft_write_message);
	while (1)
	{
		usleep(10);
		pause();
	}
	return (0);
}
