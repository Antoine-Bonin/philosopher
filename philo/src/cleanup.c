/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:52:32 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/08 15:22:32 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
		pthread_mutex_destroy(&data->death_lock);
	if (data->mutex_init >= 3)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->philos)
		free(data->philos);
	if (data->forks)
		free(data->forks);
	if (data->forks_state)
		free(data->forks_state);
	free(data);
}

void	garbage_data(t_data *data)
{
	if (data->philos)
		free(data->philos);
	if (data->forks)
		free(data->forks);
	free(data);
}

int	garbage_mutex(t_data *data)
{
	int	i;

	if (data->mutex_init >= 3)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->mutex_init >= 2)
		pthread_mutex_destroy(&data->death_lock);
	if (data->mutex_init >= 1)
		pthread_mutex_destroy(&data->update);
	i = 0;
	while (i < data->nb_philo && data->mutex_init > 0)
		pthread_mutex_destroy(&data->forks[i++]);
	return (1);
}
