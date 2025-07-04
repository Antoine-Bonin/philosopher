/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:18:27 by antbonin          #+#    #+#             */
/*   Updated: 2025/07/04 18:09:16 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	unlock_forks_in_order(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	get_fork_order(philo, &first, &second);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

void	handle_routine(t_data *data, t_philo *philo)
{
	while (!should_stop(data))
	{
		if (!forks_lock_two(philo))
			break ;
		philo->is_eating = 1;
		pthread_mutex_lock(&data->update);
		philo->meal_eat++;
		philo->last_meal = get_current_time();
		print_message(data, philo->id, EAT);
		pthread_mutex_unlock(&data->update);
		ft_usleep(data, data->time_to_eat);
		unlock_forks_in_order(philo);
		philo->is_eating = 0;
		print_message(data, philo->id, SLEEP);
		ft_usleep(data, data->time_to_sleep);
		print_message(data, philo->id, THINK);
		thinking(philo);
	}
}

void	thinking(t_philo *philo)
{
	long	time_since_last_meal;
	long	thinking_time;

	pthread_mutex_lock(&philo->data->update);
	time_since_last_meal = get_current_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->data->update);
	thinking_time = (philo->data->time_to_die - philo->data->time_to_eat
			- time_since_last_meal) / 2;
	if (thinking_time > 0)
		ft_usleep(philo->data, thinking_time);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(data, data->time_to_eat / 2);
	while (1)
	{
		if (should_stop(data))
			break ;
		handle_routine(data, philo);
		usleep(100);
	}
	return (NULL);
}
