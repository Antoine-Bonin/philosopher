/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:29:22 by antbonin          #+#    #+#             */
/*   Updated: 2025/07/04 18:02:32 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	print_message(t_data *data, int id, char *msg)
{
	pthread_mutex_lock(&data->print_mutex);
	if (should_stop(data))
	{
		pthread_mutex_unlock(&data->print_mutex);
		return ;
	}
	printf("%ld %d %s\n", get_current_time() - data->start_time, id + 1, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	if (data->mutex_init >= 1)
		pthread_mutex_destroy(&data->update);
	if (data->mutex_init >= 2)
		pthread_mutex_destroy(&data->is_dead);
	if (data->mutex_init >= 3)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->mutex_init >= 4)
		pthread_mutex_destroy(&data->forks_mutex);
	free(data->philos);
	free(data->forks);
	free(data->forks_state);
	free(data);
}
