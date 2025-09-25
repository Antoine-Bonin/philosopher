/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:17:14 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/25 16:17:18 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	clean_exit(t_data_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	if (table->mutex_init >= 1)
		pthread_mutex_destroy(&table->update);
	if (table->mutex_init >= 2)
		pthread_mutex_destroy(&table->death_lock);
	if (table->mutex_init >= 3)
		pthread_mutex_destroy(&table->print_mutex);
	garbage_data(table);
}

void	garbage_data(t_data_table *table)
{
	if (table->philos)
		free(table->philos);
	if (table->forks)
		free(table->forks);
	if (table->forks_state)
		free(table->forks_state);
	free(table);
}

int	garbage_mutex(t_data_table *table)
{
	int	i;

	if (table->mutex_init >= 3)
		pthread_mutex_destroy(&table->print_mutex);
	if (table->mutex_init >= 2)
		pthread_mutex_destroy(&table->death_lock);
	if (table->mutex_init >= 1)
		pthread_mutex_destroy(&table->update);
	i = 0;
	while (i < table->nb_philo && table->mutex_init > 0)
		pthread_mutex_destroy(&table->forks[i++]);
	return (1);
}

int	clean_thread(t_data_table *table, int i)
{
	int	j;

	table->status = DEAD;
	j = 0;
	while (j < i)
	{
		pthread_join(table->philos[j].thread, NULL);
		j++;
	}
	if (table->mutex_init >= 3)
		pthread_mutex_destroy(&table->print_mutex);
	if (table->mutex_init >= 2)
		pthread_mutex_destroy(&table->death_lock);
	if (table->mutex_init >= 1)
		pthread_mutex_destroy(&table->update);
	j = 0;
	while (j < table->nb_philo && table->mutex_init > 0)
	{
		pthread_mutex_destroy(&table->forks[j]);
		j++;
	}
	return (1);
}
