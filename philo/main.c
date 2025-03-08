/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:23:25 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/08 18:54:26 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data_arg(t_data *data, int ac, char **av)
{
	if (is_digit(av) == 1)
		return (1);
	data->nb_philo = ft_atoi(av[1]);
	data->time_to_eat = ft_atoi(av[2]);
	data->time_to_sleep = ft_atoi(av[3]);
	data->time_to_think = ft_atoi(av[4]);
	if (data->nb_philo < 1 || data->time_to_eat < 1 || data->time_to_sleep < 1
		|| data->time_to_think < 1)
		return (1);
	if (ac == 6)
	{
		data->nb_eat = ft_atoi(av[5]);
		if (data->nb_eat < 1)
			return (1);
	}
	else
		data->nb_eat = 0;
	return (0);
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->update, NULL) != 0)
		return (1);
	else
		data->mutex_init = 1;
	if (pthread_mutex_init(&data->is_dead, NULL) != 0)
		return (1);
	else
		data->mutex_init = 2;
	return (0);
}

void	init_philos(t_data *data)
{
	int	i;

	data->start_time = get_current_time();
	data->stop = 0;
	data->waiting_last = 0;
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].r_fork = &data->forks[i];
		data->philos[i].l_fork = &data->forks[(i + 1) % data->nb_philo];
		data->philos[i].data = data;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].meal_eat = 0;
		data->philos[i].is_eating = 0;
		data->philos[i].wait = 0;
		i++;
	}
}

int	init_data(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	i = 0;
	if (init_mutex(data))
		return (1);
	init_philos(data);  
	while (i < data->nb_philo)
	{
		pthread_create(&data->philos[i].thread, NULL, philo_routine,
			(void *)&data->philos[i]);
		i++;
	}
	pthread_create(&data->monitor_thread, NULL, monitor, (void *)data);
	i = 0;
	return (0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong number of arguments\n");
		free(data);
		return (1);
	}
	if (init_data_arg(data, ac, av))
	{
		printf("Error: wrong arguments\n");
		free(data);
		return (1);
	}
	if (init_data(data))
	{
		printf("Error: init data\n");
		free(data);
		return (1);
	}
	free(data);
	return (0);
}
