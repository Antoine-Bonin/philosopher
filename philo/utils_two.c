/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:41:50 by antbonin          #+#    #+#             */
/*   Updated: 2025/07/04 18:01:29 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_stop(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->is_dead);
	stop = data->stop;
	pthread_mutex_unlock(&data->is_dead);
	return (stop);
}

int	init_data_malloc(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	data->forks_state = malloc(sizeof(int) * data->nb_philo);
	if (!data->forks_state)
		return (1);
	return (0);
}

void	ft_usleep(t_data *data, long time_in_ms)
{
	long	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
	{
		if (should_stop(data))
			break ;
		usleep(100);
	}
}
