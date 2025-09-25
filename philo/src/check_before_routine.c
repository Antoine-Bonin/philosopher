/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_before_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:13:21 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/25 16:10:13 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// void	*philo_routine(void *arg)
// {
// 	t_philo			*philo;
// 	t_data_table	*table;

// 	philo = (t_philo *)arg;
// 	table = philo->table;
// 	philo->last_meal = get_current_time();
// 	if (philo->id % 2 != 0)
// 	{
// 		print_message(table, philo->id, THINK);
// 		smart_usleep(table, table->time_to_eat / 2);
// 	}
// 	else
// 		print_message(table, philo->id, THINK);
// 	handle_routine(table, philo);
// 	return (NULL);
// }
