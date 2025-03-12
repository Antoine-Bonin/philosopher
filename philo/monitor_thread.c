/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:36:35 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/11 17:56:32 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	current_time;

	data = (t_data *)arg;
	while (!should_stop(data))
	{
		i = 0;
		while (i < data->nb_philo && !data->stop)
		{
			current_time = get_current_time();
			pthread_mutex_lock(&data->update);
			if ((current_time - data->philos[i].last_meal > data->time_to_die)
				|| (data->nb_meal >= data->nb_eat && data->nb_eat != 0))
			{
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
			pthread_mutex_unlock(&data->update);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
