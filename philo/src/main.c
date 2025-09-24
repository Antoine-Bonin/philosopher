/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:23:25 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/24 14:31:43 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	print_and_free(t_data *data)
{
	printf("Error: init data\n");
	garbage_data(data);
}

static int	init_data_malloc(t_data **data)
{
	*data = (t_data *)malloc(sizeof(t_data));
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
	t_data	*data;

	data = NULL;
	if (init_data_malloc(&data))
		return (1);
	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong number of arguments\n");
		free(data);
		return (1);
	}
	if (init_data_arg(data, ac, av))
	{
		if (data->nb_philo != 1)
			printf("Error: wrong arguments\n");
		garbage_data(data);
		return (1);
	}
	if (init_data(data))
	{
		print_and_free(data);
		return (1);
	}
	cleanup(data);
	return (0);
}
