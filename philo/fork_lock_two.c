/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_lock_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:38:41 by antbonin          #+#    #+#             */
/*   Updated: 2025/06/06 17:46:22 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	get_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second, int *indices)
{
	if (philo->r_fork < philo->l_fork)
	{
		*first = philo->r_fork;
		*second = philo->l_fork;
	}
	else
	{
		*first = philo->l_fork;
		*second = philo->r_fork;
	}
	indices[0] = *first - philo->data->forks;
	indices[1] = *second - philo->data->forks;
}

int	check_forks_availability(t_philo *philo, int first_idx, int second_idx)
{
	pthread_mutex_lock(&philo->data->forks_mutex);
	if (philo->data->forks_state[first_idx] == 1
		|| philo->data->forks_state[second_idx] == 1)
	{
		pthread_mutex_unlock(&philo->data->forks_mutex);
		return (0);
	}
	philo->data->forks_state[first_idx] = 1;
	philo->data->forks_state[second_idx] = 1;
	pthread_mutex_unlock(&philo->data->forks_mutex);
	return (1);
}

void	release_fork_states(t_philo *philo, int first_idx, int second_idx)
{
	pthread_mutex_lock(&philo->data->forks_mutex);
	philo->data->forks_state[first_idx] = 0;
	philo->data->forks_state[second_idx] = 0;
	pthread_mutex_unlock(&philo->data->forks_mutex);
}

int	lock_first_fork(t_philo *philo, pthread_mutex_t *first_fork, int first_idx,
		int second_idx)
{
	pthread_mutex_lock(first_fork);
	print_message(philo->data, philo->id, FORK);
	if (should_stop(philo->data))
	{
		pthread_mutex_unlock(first_fork);
		release_fork_states(philo, first_idx, second_idx);
		return (0);
	}
	return (1);
}

int	forks_lock_two(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	int				indices[2];

	if (should_stop(philo->data))
		return (0);
	get_fork_order(philo, &first_fork, &second_fork, indices);
	if (!check_forks_availability(philo, indices[0], indices[1]))
		return (0);
	if (!lock_first_fork(philo, first_fork, indices[0], indices[1]))
		return (0);
	pthread_mutex_lock(second_fork);
	print_message(philo->data, philo->id, FORK);
	return (1);
}
