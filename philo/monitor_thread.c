/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:36:35 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/27 14:34:15 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	death_occured(t_data *data, int i)
{
	pthread_mutex_unlock(&data->update);
	pthread_mutex_lock(&data->is_dead);
	data->stop = 1;
	pthread_mutex_unlock(&data->is_dead);
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d died\n", get_current_time() - data->start_time,
		data->philos[i].id + 1);
	pthread_mutex_unlock(&data->print_mutex);
}

void	philo_ate(t_data *data)
{
	pthread_mutex_unlock(&data->update);
	pthread_mutex_lock(&data->is_dead);
	data->stop = 1;
	pthread_mutex_unlock(&data->is_dead);
}

void *monitor_routine(void *arg)
{
    t_data  *data;
    long    current_time;
    int     i;
    int     ate;

    data = (t_data *)arg;
    while (!should_stop(data))
    {
        i = 0;
        ate = 0;
        while (i < data->nb_philo && !data->stop)
        {
            current_time = get_current_time();
            pthread_mutex_lock(&data->update);
            if ((current_time + 1)- data->philos[i].last_meal > data->time_to_die) 
            {
                death_occured(data, i);
                return (NULL);
            }
            if (data->nb_eat != -1 && data->philos[i].meal_eat >= data->nb_eat- 1)
                ate++;    
            pthread_mutex_unlock(&data->update);
            i++;
        }
        if (data->nb_eat != -1 && ate == data->nb_philo)
        {
            pthread_mutex_lock(&data->update);
            philo_ate(data);
            return (NULL);
        }
        
        usleep(1000);
    }
    return (NULL);
}
