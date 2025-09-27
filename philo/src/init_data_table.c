/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_table.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:17:02 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/27 15:13:10 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	init_data_table_malloc(t_data_table *table)
{
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->nb_philo);
	if (!table->philos)
		return (1);
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* table->nb_philo);
	if (!table->forks)
	{
		free(table->philos);
		table->philos = NULL;
		return (1);
	}
	table->forks_state = (int *)malloc(sizeof(int) * table->nb_philo);
	if (!table->forks_state)
	{
		free(table->philos);
		free(table->forks);
		table->philos = NULL;
		table->forks = NULL;
		return (1);
	}
	return (0);
}

int	init_data_table_arg(t_data_table *table, int ac, char **av)
{
	if (is_digit(av) == 1)
		return (1);
	table->nb_philo = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (table->nb_philo < 1 || table->time_to_eat < 1
		|| table->time_to_sleep < 1 || table->time_to_die < 1)
		return (1);
	if (ac == 6)
	{
		table->nb_eat = ft_atoi(av[5]);
		if (table->nb_eat < 1)
			return (1);
	}
	else
		table->nb_eat = -1;
	if (init_data_table_malloc(table))
		return (1);
	return (0);
}

static int	init_mutex(t_data_table *table)
{
	int	i;

	i = 0;
	table->mutex_init = 0;
	while (i < table->nb_philo)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (garbage_mutex(table));
		i++;
	}
	if (pthread_mutex_init(&table->update, NULL) != 0)
		return (garbage_mutex(table));
	table->mutex_init = 1;
	if (pthread_mutex_init(&table->death_lock, NULL) != 0)
		return (garbage_mutex(table));
	table->mutex_init = 2;
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (garbage_mutex(table));
	table->mutex_init = 3;
	if (pthread_mutex_init(&table->wait_thread_create, NULL) != 0)
		return (garbage_mutex(table));
	table->mutex_init = 4;
	return (0);
}

static void	init_philos(t_data_table *table)
{
	int	i;

	table->start_time = get_current_time();
	table->status = ALIVE;
	i = 0;
	while (i < table->nb_philo)
	{
		table->philos[i].id = i;
		table->philos[i].forks[0] = i;
		table->philos[i].forks[1] = (i + 1) % table->nb_philo;
		table->philos[i].table = table;
		table->philos[i].last_meal = table->start_time;
		table->philos[i].meal_eat = 0;
		table->philos[i].is_eating = 0;
		table->philos[i].wait = 0;
		table->forks_state[i] = 0;
		i++;
	}
}

int	init_table(t_data_table *table)
{
	int	i;

	if (init_mutex(table))
		return (1);
	init_philos(table);
	pthread_mutex_lock(&table->wait_thread_create);
	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine,
				(void *)&table->philos[i]) != 0)
			return (clean_thread(table, i));
		i++;
	}
	if (pthread_create(&table->monitor_thread, NULL, monitor_routine,
			(void *)table) != 0)
		return (clean_thread(table, i));
	pthread_mutex_unlock(&table->wait_thread_create);
	pthread_join(table->monitor_thread, NULL);
	i = -1;
	while (++i < table->nb_philo)
		pthread_join(table->philos[i].thread, NULL);
	return (0);
}
