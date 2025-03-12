/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:18:27 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/12 18:45:58 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_data *data, int id, char *msg);

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	if (data->mutex_init == 1)
		pthread_mutex_destroy(&data->update);
	if (data->mutex_init == 2)
		pthread_mutex_destroy(&data->is_dead);
	if (data->mutex_init == 3)
		pthread_mutex_destroy(&data->print_mutex);
	free(data->philos);
	free(data->forks);
	free(data);
}


void	forks_lock(t_philo *philo)
{
	if(should_stop(philo->data))
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		print_message(philo->data, philo->id, FORK);
		pthread_mutex_lock(philo->r_fork);
		print_message(philo->data, philo->id, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		print_message(philo->data, philo->id, FORK);
		pthread_mutex_lock(philo->l_fork);
		print_message(philo->data, philo->id, FORK);
	}
}
void	forks_unlock(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}
int	should_stop(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->is_dead);
	stop = data->stop;
	pthread_mutex_unlock(&data->is_dead);
	return (stop);
}

void	print_message(t_data *data, int id, char *msg)
{
	if (should_stop(data))
		return ;
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d %s\n", get_current_time() - data->start_time, id + 1, msg);
	pthread_mutex_unlock(&data->print_mutex);
}
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (!should_stop(data))
	{
		forks_lock(philo);
		philo->is_eating = 1;
		pthread_mutex_lock(&data->update);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(&data->update);
		print_message(data, philo->id, EAT);
		usleep(data->time_to_eat * 1000);
		forks_unlock(philo);
		philo->is_eating = 0;
		philo->meal_eat++;
		if (should_stop(data))
			break ;
		print_message(data, philo->id, SLEEP);
		usleep(data->time_to_sleep * 1000);
		print_message(data, philo->id, THINK);
		pthread_mutex_lock(&data->update);
		data->nb_meal++;
		pthread_mutex_unlock(&data->update);
	}
	return (NULL);
}
