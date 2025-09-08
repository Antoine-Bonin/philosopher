/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:20:33 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/08 15:50:28 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	lock_first_fork(t_philo *philo, int first_fork)
{
	pthread_mutex_lock(&philo->data->forks[first_fork]);
	if (get_status(philo->data) != ALIVE)
	{
		pthread_mutex_unlock(&philo->data->forks[first_fork]);
		return (0);
	}
	philo->data->forks_state[first_fork] = 1;
	print_message(philo->data, philo->id, FORK);
	return (1);
}

int	lock_second_fork(t_philo *philo, int second_fork)
{
	pthread_mutex_lock(&philo->data->forks[second_fork]);
	if (get_status(philo->data) != ALIVE)
	{
		pthread_mutex_unlock(&philo->data->forks[second_fork]);
		return (0);
	}
	philo->data->forks_state[second_fork] = 1;
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
		philo->data->forks_state[fork2] = 0;
		pthread_mutex_unlock(&philo->data->forks[fork2]);
		philo->data->forks_state[fork1] = 0;
		pthread_mutex_unlock(&philo->data->forks[fork1]);
	}
	else
	{
		philo->data->forks_state[fork1] = 0;
		pthread_mutex_unlock(&philo->data->forks[fork1]);
		philo->data->forks_state[fork2] = 0;
		pthread_mutex_unlock(&philo->data->forks[fork2]);
	}
}
