/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:23:25 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/25 14:21:20 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	print_and_free(t_data_table *data)
{
	printf("Error: init data\n");
	garbage_data(data);
}

static int	init_data_table_malloc(t_data_table **data)
{
	*data = (t_data_table *)malloc(sizeof(t_data_table));
	if (!*data)
		return (1);
	(*data)->philos = NULL;
	(*data)->forks = NULL;
	(*data)->forks_state = NULL;
	(*data)->mutex_init = 0;
	return (0);
}

int	main(int ac, char **av)
{
	t_data_table	*data;

	data = NULL;
	if (init_data_table_malloc(&data))
		return (1);
	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong number of arguments\n");
		free(data);
		return (1);
	}
	if (init_data_table_arg(data, ac, av))
	{
		if (data->nb_philo != 1)
			printf("Error: wrong arguments\n");
		garbage_data(data);
		return (1);
	}
	if (init_table(data))
	{
		print_and_free(data);
		return (1);
	}
	clean_exit(data);
	return (0);
}
