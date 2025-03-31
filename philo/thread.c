/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:18:27 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/31 15:38:58 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	forks_lock(t_philo *philo)
{
	if (should_stop(philo->data))
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

void	handle_routine(t_data *data, t_philo *philo)
{
	while (!should_stop(data))
	{
		forks_lock(philo);
		philo->is_eating = 1;
		pthread_mutex_lock(&data->update);
		philo->meal_eat++;
		pthread_mutex_unlock(&data->update);
		pthread_mutex_lock(&data->update);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(&data->update);
		print_message(data, philo->id, EAT);
		usleep(data->time_to_eat * 1000);
		forks_unlock(philo);
		philo->is_eating = 0;
		print_message(data, philo->id, SLEEP);
		usleep(data->time_to_sleep * 1000);
		print_message(data, philo->id, THINK);
	}
	return ;
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	handle_routine(data, philo);
	return (NULL);
}
