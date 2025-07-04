/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_lock_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:38:41 by antbonin          #+#    #+#             */
/*   Updated: 2025/07/04 18:09:21 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	get_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->r_fork;
		*second = philo->l_fork;
	}
	else
	{
		*first = philo->l_fork;
		*second = philo->r_fork;
	}
}

int	forks_lock_two(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->data->nb_philo == 1)
	{
		print_message(philo->data, philo->id, FORK);
		ft_usleep(philo->data, philo->data->time_to_die * 1000);
		return (0);
	}
	get_fork_order(philo, &first_fork, &second_fork);
	pthread_mutex_lock(first_fork);
	print_message(philo->data, philo->id, FORK);
	if (should_stop(philo->data))
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	pthread_mutex_lock(second_fork);
	print_message(philo->data, philo->id, FORK);
	return (1);
}
