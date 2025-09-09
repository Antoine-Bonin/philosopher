/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:46:50 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/09 14:05:19 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	init_data_malloc(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
	{
		free(data->philos);
		data->philos = NULL;
		return (1);
	}
	data->forks_state = malloc(sizeof(int) * data->nb_philo);
	if (!data->forks_state)
	{
		free(data->philos);
		free(data->forks);
		data->philos = NULL;
		data->forks = NULL;
		return (1);
	}
	return (0);
}

int	init_data_arg(t_data *data, int ac, char **av)
{
	if (is_digit(av) == 1)
		return (1);
	data->nb_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (data->nb_philo < 1 || data->time_to_eat < 1 || data->time_to_sleep < 1
		|| data->time_to_die < 1)
		return (1);
	if (ac == 6)
	{
		data->nb_eat = ft_atoi(av[5]);
		if (data->nb_eat < 1)
			return (1);
	}
	else
		data->nb_eat = -1;
	if (init_data_malloc(data))
		return (1);
	return (0);
}

static int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	data->mutex_init = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (garbage_mutex(data));
		i++;
	}
	if (pthread_mutex_init(&data->update, NULL) != 0)
		return (garbage_mutex(data));
	data->mutex_init = 1;
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (garbage_mutex(data));
	data->mutex_init = 2;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (garbage_mutex(data));
	data->mutex_init = 3;
	return (0);
}

static void	init_philos(t_data *data)
{
	int	i;

	data->start_time = get_current_time();
	data->status = ALIVE;
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i;
		data->philos[i].forks[0] = i;
		data->philos[i].forks[1] = (i + 1) % data->nb_philo;
		data->philos[i].data = data;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].meal_eat = 0;
		data->philos[i].is_eating = 0;
		data->philos[i].wait = 0;
		data->forks_state[i] = 0;
		i++;
	}
}

int	init_data(t_data *data)
{
	int	i;

	if (init_mutex(data))
		return (1);
	init_philos(data);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&data->philos[i].thread, NULL, philo_routine,
			(void *)&data->philos[i]);
		i++;
	}
	pthread_create(&data->monitor_thread, NULL, monitor_routine, (void *)data);
	pthread_join(data->monitor_thread, NULL);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	return (0);
}
