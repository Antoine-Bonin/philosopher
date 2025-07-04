/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:36:35 by antbonin          #+#    #+#             */
/*   Updated: 2025/07/04 18:09:31 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	death_occured(t_data *data, int i)
{
	pthread_mutex_lock(&data->is_dead);
	data->stop = 1;
	pthread_mutex_unlock(&data->is_dead);
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d died\n", get_current_time() - data->start_time,
		data->philos[i].id + 1);
	pthread_mutex_unlock(&data->print_mutex);
}

void	philo_ate(t_data *data)
{
	usleep(data->time_to_eat * 1000);
	pthread_mutex_lock(&data->is_dead);
	data->stop = 1;
	pthread_mutex_unlock(&data->is_dead);
}

int	check_death(t_data *data, int *i, int *ate)
{
	long	current_time;
	long	last_meal;
	int		meal_count;

	pthread_mutex_lock(&data->update);
	current_time = get_current_time();
	last_meal = data->philos[*i].last_meal;
	meal_count = data->philos[*i].meal_eat;
	pthread_mutex_unlock(&data->update);
	if (current_time - last_meal > data->time_to_die)
	{
		death_occured(data, *i);
		return (1);
	}
	if (data->nb_eat != -1 && meal_count >= data->nb_eat)
		(*ate)++;
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		ate;

	data = (t_data *)arg;
	usleep(500);
	while (!should_stop(data))
	{
		i = 0;
		ate = 0;
		while (i < data->nb_philo && !should_stop(data))
		{
			if (check_death(data, &i, &ate))
				return (NULL);
			i++;
			if (data->nb_eat != -1 && ate == data->nb_philo)
				break ;
		}
		if (data->nb_eat != -1 && ate == data->nb_philo)
			return (philo_ate(data), NULL);
		usleep(500);
	}
	return (NULL);
}
