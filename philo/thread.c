/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:18:27 by antbonin          #+#    #+#             */
/*   Updated: 2025/06/06 17:41:42 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlock_forks(t_data *data, t_philo *philo)
{
	int	r_fork_idx;
	int	l_fork_idx;

	r_fork_idx = philo->r_fork - data->forks;
	l_fork_idx = philo->l_fork - data->forks;
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_lock(&data->forks_mutex);
	data->forks_state[r_fork_idx] = 0;
	data->forks_state[l_fork_idx] = 0;
	pthread_mutex_unlock(&data->forks_mutex);
}

int	handle_single_philo(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	print_message(data, philo->id, FORK);
	usleep(data->time_to_die * 1001);
	pthread_mutex_unlock(philo->r_fork);
	return (0);
}

int	fork_lock(t_data *data, t_philo *philo, int unlock)
{
	int	ret;

	ret = 0;
	if (unlock == 1)
	{
		unlock_forks(data, philo);
		return (0);
	}
	if (should_stop(philo->data))
		return (0);
	if (data->nb_philo == 1)
		ret = handle_single_philo(data, philo);
	else
		ret = forks_lock_two(philo);
	return (ret);
}

void	handle_routine(t_data *data, t_philo *philo)
{
	while (!should_stop(data))
	{
		if (!fork_lock(data, philo, 0))
			continue ;
		philo->is_eating = 1;
		pthread_mutex_lock(&data->update);
		philo->meal_eat++;
		philo->last_meal = get_current_time();
		print_message(data, philo->id, EAT);
		pthread_mutex_unlock(&data->update);
		usleep(data->time_to_eat * 1000);
		fork_lock(data, philo, 1);
		philo->is_eating = 0;
		print_message(data, philo->id, SLEEP);
		usleep(data->time_to_sleep * 1000);
		print_message(data, philo->id, THINK);
		thinking(philo);
	}
	return ;
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (data->nb_philo > 1 && philo->id % 2 == 1)
		usleep(data->time_to_eat * 500);
	else if (data->nb_philo % 2 == 1 && philo->id == data->nb_philo - 1)
		usleep(data->time_to_eat * 250);
	handle_routine(data, philo);
	return (NULL);
}
