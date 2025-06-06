/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:18:27 by antbonin          #+#    #+#             */
/*   Updated: 2025/06/06 17:21:29 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	forks_lock_two(t_philo *philo)
{
    pthread_mutex_t	*first_fork;
    pthread_mutex_t	*second_fork;
    int				first_idx;
    int				second_idx;

    if (should_stop(philo->data))
        return (0);
    if (philo->r_fork < philo->l_fork)
    {
        first_fork = philo->r_fork;
        second_fork = philo->l_fork;
    }
    else
    {
        first_fork = philo->l_fork;
        second_fork = philo->r_fork;
    }
    first_idx = first_fork - philo->data->forks;
    second_idx = second_fork - philo->data->forks;
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
    pthread_mutex_lock(first_fork);
    print_message(philo->data, philo->id, FORK);
    if (should_stop(philo->data))
    {
        pthread_mutex_unlock(first_fork);
        pthread_mutex_lock(&philo->data->forks_mutex);
        philo->data->forks_state[first_idx] = 0;
        philo->data->forks_state[second_idx] = 0;
        pthread_mutex_unlock(&philo->data->forks_mutex);
        return (0);
    }
    pthread_mutex_lock(second_fork);
    print_message(philo->data, philo->id, FORK);
    return (1);
}

int	fork_lock(t_data *data, t_philo *philo, int unlock)
{
    int	ret;
    int	r_fork_idx;
    int	l_fork_idx;

    ret = 0;
    if (unlock == 1)
    {
        r_fork_idx = philo->r_fork - data->forks;
        l_fork_idx = philo->l_fork - data->forks;
        pthread_mutex_unlock(philo->r_fork);
        pthread_mutex_unlock(philo->l_fork);
        pthread_mutex_lock(&data->forks_mutex);
        data->forks_state[r_fork_idx] = 0;
        data->forks_state[l_fork_idx] = 0;
        pthread_mutex_unlock(&data->forks_mutex);
        return (0);
    }
    if (should_stop(philo->data))
        return (0);
    if (data->nb_philo == 1)
    {
        pthread_mutex_lock(philo->r_fork);
        print_message(data, philo->id, FORK);
        usleep(data->time_to_die * 1001);
        pthread_mutex_unlock(philo->r_fork);
        return (0);
    }
    else
        ret = forks_lock_two(philo);
    return (ret);
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
    handle_routine(data, philo);
    return (NULL);
}
