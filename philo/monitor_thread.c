/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:36:35 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/12 11:53:06 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	current_time;
	int		death_occurred;

	data = (t_data *)arg;
	while (!should_stop(data))
	{
		i = 0;
		death_occurred = 0;
		while (i < data->nb_philo && !data->stop)
		{
			current_time = get_current_time();
			pthread_mutex_lock(&data->update);
			if (current_time - data->philos[i].last_meal > data->time_to_die)
			{
				death_occurred = 1;
				pthread_mutex_unlock(&data->update);
				pthread_mutex_lock(&data->is_dead);
				data->stop = 1;
				pthread_mutex_unlock(&data->is_dead);
				pthread_mutex_lock(&data->print_mutex);
				printf("%ld %d died\n", current_time - data->start_time,
					data->philos[i].id + 1);
				pthread_mutex_unlock(&data->print_mutex);
				return (NULL);
			}
			else if (data->nb_eat != 0 && data->nb_meal >= data->nb_eat)
			{
				pthread_mutex_unlock(&data->update);
				usleep(10);
				pthread_mutex_lock(&data->is_dead);
				data->stop = 1;
				pthread_mutex_unlock(&data->is_dead);
				pthread_mutex_lock(&data->print_mutex);
				printf("%ld all philos ate\n", current_time - data->start_time);
				pthread_mutex_unlock(&data->print_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->update);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
