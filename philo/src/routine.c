/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:18:27 by antbonin          #+#    #+#             */
/*   Updated: 2025/08/10 17:41:10 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	lock_fork_order(t_philo *philo, int first, int second)
{
	int	first_fork;
	int	second_fork;

	first_fork = philo->forks[first];
	second_fork = philo->forks[second];
	if (!check_forks_availability(philo, first_fork, second_fork))
		return (0);
	if (!lock_first_fork(philo, first_fork, second_fork))
		return (0);
	if (!lock_second_fork(philo, first_fork, second_fork))
		return (0);
	return (1);
}

static int	take_forks_ordered(t_philo *philo)
{
	int	first;
	int	second;

	if (philo->forks[0] < philo->forks[1])
	{
		first = 0;
		second = 1;
	}
	else
	{
		first = 1;
		second = 0;
	}
	return (lock_fork_order(philo, first, second));
}

static void	sleep_then_think(t_data *data, t_philo *philo)
{
	if (get_status(data) != ALIVE)
		return ;
	print_message(data, philo->id, SLEEP);
	smart_usleep(data, data->time_to_sleep);
	print_message(data, philo->id, THINK);
	smart_usleep(data, 1 / 2);
}

static void	handle_routine(t_data *data, t_philo *philo)
{
	while (get_status(data) == ALIVE)
	{
		if (data->nb_philo == 1)
		{
			print_message(data, philo->id, FORK);
			smart_usleep(data, data->time_to_die);
			return ;
		}
		if (!take_forks_ordered(philo))
		{
			usleep(100);
			continue ;
		}
		philo->is_eating = 1;
		pthread_mutex_lock(&data->update);
		philo->meal_eat++;
		philo->last_meal = get_current_time();
		print_message(data, philo->id, EAT);
		pthread_mutex_unlock(&data->update);
		smart_usleep(data, data->time_to_eat);
		release_forks_ordered(philo);
		philo->is_eating = 0;
		sleep_then_think(data, philo);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2)
		usleep(data->time_to_eat * 100);
	pthread_mutex_lock(&data->update);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&data->update);
	handle_routine(data, philo);
	return (NULL);
}
