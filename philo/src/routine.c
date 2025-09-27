/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:18:27 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/27 15:08:06 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	lock_fork_order(t_philo *philo, int first, int second)
{
	int	first_fork;
	int	second_fork;

	first_fork = philo->forks[first];
	second_fork = philo->forks[second];
	if (!lock_first_fork(philo, first_fork))
		return (0);
	if (!lock_second_fork(philo, second_fork))
	{
		pthread_mutex_lock(&philo->table->forks[first_fork]);
		philo->table->forks_state[first_fork] = 0;
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		return (0);
	}
	return (1);
}

static int	take_forks_ordered(t_philo *philo)
{
	int	first;
	int	second;
	int	first_fork;
	int	second_fork;

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
	first_fork = philo->forks[first];
	second_fork = philo->forks[second];
	if (!check_fork_availability(philo, first_fork)
		|| !check_fork_availability(philo, second_fork))
		return (0);
	return (lock_fork_order(philo, first, second));
}

static void	sleep_then_think(t_data_table *table, t_philo *philo)
{
	if (get_status(table) != ALIVE)
		return ;
	print_message(table, philo->id, SLEEP);
	smart_usleep(table, table->time_to_sleep);
	print_message(table, philo->id, THINK);
	if (table->nb_philo % 2 != 0)
		smart_usleep(table, table->time_to_eat);
}

static void	handle_routine(t_data_table *table, t_philo *philo)
{
	while (get_status(table) == ALIVE)
	{
		if (table->nb_philo == 1)
		{
			print_message(table, philo->id, FORK);
			smart_usleep(table, table->time_to_die);
			return ;
		}
		if (!take_forks_ordered(philo))
		{
			usleep(300);
			continue ;
		}
		philo->is_eating = 1;
		pthread_mutex_lock(&table->update);
		philo->meal_eat++;
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(&table->update);
		print_message(table, philo->id, EAT);
		smart_usleep(table, table->time_to_eat);
		release_forks_ordered(philo);
		philo->is_eating = 0;
		sleep_then_think(table, philo);
	}
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	t_data_table	*table;

	philo = (t_philo *)arg;
	table = philo->table;
	pthread_mutex_lock(&table->wait_thread_create);
	pthread_mutex_unlock(&table->wait_thread_create);
	pthread_mutex_lock(&table->update);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&table->update);
	if (philo->id % 2 != 0)
	{
		print_message(table, philo->id, THINK);
		smart_usleep(table, table->time_to_eat / 2);
	}
	else
		print_message(table, philo->id, THINK);
	handle_routine(table, philo);
	return (NULL);
}
