/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:12:02 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/09 18:20:55 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void monitor(t_data *data)
{
    int i;
    int j;
    int time;
    t_philo *philo;

    philo = data->philos;
    while (1)
    {
        i = 0;
        while (i < data->nb_philo)
        {
            time = get_current_time();
            if (time - philo[i].last_meal > data->time_to_eat)
            {
                pthread_mutex_lock(&data->is_dead);
                printf("%d %d died\n", time, philo[i].id);
                pthread_mutex_unlock(&data->is_dead);
                return ;
            }
            i++;
        }
    }
}
