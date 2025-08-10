/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:20:33 by antbonin          #+#    #+#             */
/*   Updated: 2025/08/10 16:48:28 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_forks_availability(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->data->forks_mutex);
	if (philo->data->forks_state[first_fork] == 1
		|| philo->data->forks_state[second_fork] == 1)
	{
		pthread_mutex_unlock(&philo->data->forks_mutex);
		return (0);
	}
	philo->data->forks_state[first_fork] = 1;
	philo->data->forks_state[second_fork] = 1;
	pthread_mutex_unlock(&philo->data->forks_mutex);
	return (1);
}

void	release_forks_state(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->data->forks_mutex);
	philo->data->forks_state[first_fork] = 0;
	philo->data->forks_state[second_fork] = 0;
	pthread_mutex_unlock(&philo->data->forks_mutex);
}

int	lock_first_fork(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->data->forks[first_fork]);
	if (get_status(philo->data) != ALIVE)
	{
		pthread_mutex_unlock(&philo->data->forks[first_fork]);
		release_forks_state(philo, first_fork, second_fork);
		return (0);
	}
	print_message(philo->data, philo->id, FORK);
	return (1);
}

int	lock_second_fork(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->data->forks[second_fork]);
	if (get_status(philo->data) != ALIVE)
	{
		pthread_mutex_unlock(&philo->data->forks[first_fork]);
		pthread_mutex_unlock(&philo->data->forks[second_fork]);
		release_forks_state(philo, first_fork, second_fork);
		return (0);
	}
	print_message(philo->data, philo->id, FORK);
	return (1);
}

void	release_forks_ordered(t_philo *philo)
{
	int	fork1;
	int	fork2;

	fork1 = philo->forks[0];
	fork2 = philo->forks[1];
	if (philo->forks[0] < philo->forks[1])
	{
		pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]);
		pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
		pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]);
	}
	pthread_mutex_lock(&philo->data->forks_mutex);
	philo->data->forks_state[fork1] = 0;
	philo->data->forks_state[fork2] = 0;
	pthread_mutex_unlock(&philo->data->forks_mutex);
}
