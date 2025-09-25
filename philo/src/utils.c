/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:29:22 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/25 16:11:39 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	is_digit(char **av)
{
	int	x;
	int	y;

	x = 1;
	while (av[x])
	{
		y = 0;
		while (av[x][y])
		{
			if (av[x][y] < '0' || av[x][y] > '9')
				return (1);
			y++;
		}
		x++;
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

int	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_message(t_data_table *table, int id, char *msg)
{
	pthread_mutex_lock(&table->print_mutex);
	if (get_status(table) == ALIVE)
		printf("%ld %d %s\n", get_current_time() - table->start_time, id + 1,
			msg);
	pthread_mutex_unlock(&table->print_mutex);
}

void	smart_usleep(t_data_table *table, int duration)
{
	long	start_time;
	long	current_time;
	long	elapsed;

	start_time = get_current_time();
	while (1)
	{
		if (should_stop(table))
			break ;
		current_time = get_current_time();
		elapsed = current_time - start_time;
		if (elapsed >= (long)duration)
			break ;
		if (duration - elapsed > 10)
			usleep(1000);
		else
			usleep(100);
	}
}
