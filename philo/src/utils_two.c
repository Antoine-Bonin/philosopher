/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:20:00 by antbonin          #+#    #+#             */
/*   Updated: 2025/08/09 18:48:10 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_status	get_status(t_data *data)
{
	t_status	status_value;

	pthread_mutex_lock(&data->death_lock);
	status_value = data->status;
	pthread_mutex_unlock(&data->death_lock);
	return (status_value);
}

int	should_stop(t_data *data)
{
	return (get_status(data) != ALIVE);
}
