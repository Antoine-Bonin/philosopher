/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:36:35 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/27 14:34:31 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	death_occured(t_data_table *table, int i)
{
	pthread_mutex_lock(&table->death_lock);
	table->status = DEAD;
	pthread_mutex_unlock(&table->death_lock);
	pthread_mutex_lock(&table->print_mutex);
	printf("%ld %d died\n", get_current_time() - table->start_time,
		table->philos[i].id + 1);
	pthread_mutex_unlock(&table->print_mutex);
}

static void	philo_ate(t_data_table *table)
{
	pthread_mutex_lock(&table->death_lock);
	table->status = FULL;
	pthread_mutex_unlock(&table->death_lock);
}

static int	check_death(t_data_table *table, int *i, int *ate)
{
	long	current_time;
	long	last_meal;
	int		meal_count;

	pthread_mutex_lock(&table->update);
	current_time = get_current_time();
	last_meal = table->philos[*i].last_meal;
	meal_count = table->philos[*i].meal_eat;
	pthread_mutex_unlock(&table->update);
	if (current_time - last_meal > table->time_to_die)
	{
		death_occured(table, *i);
		return (1);
	}
	if (table->nb_eat != -1 && meal_count >= table->nb_eat)
		(*ate)++;
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data_table	*table;
	int				i;
	int				ate;

	table = (t_data_table *)arg;
	while (!should_stop(table))
	{
		i = 0;
		ate = 0;
		while (i < table->nb_philo && !should_stop(table))
		{
			if (check_death(table, &i, &ate))
				return (NULL);
			i++;
			if (table->nb_eat != -1 && ate == table->nb_philo)
				break ;
		}
		if (table->nb_eat != -1 && ate == table->nb_philo)
		{
			philo_ate(table);
			return (NULL);
		}
		usleep(300);
	}
	return (NULL);
}
