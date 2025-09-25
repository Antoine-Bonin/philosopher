/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:23:25 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/25 16:11:53 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	print_and_free(t_data_table *table)
{
	printf("Error: init data\n");
	garbage_data(table);
}

static int	init_data_table_malloc(t_data_table **table)
{
	*table = (t_data_table *)malloc(sizeof(t_data_table));
	if (!*table)
		return (1);
	(*table)->philos = NULL;
	(*table)->forks = NULL;
	(*table)->forks_state = NULL;
	(*table)->mutex_init = 0;
	return (0);
}

int	main(int ac, char **av)
{
	t_data_table	*table;

	table = NULL;
	if (init_data_table_malloc(&table))
		return (1);
	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong number of arguments\n");
		free(table);
		return (1);
	}
	if (init_data_table_arg(table, ac, av))
	{
		if (table->nb_philo != 1)
			printf("Error: wrong arguments\n");
		garbage_data(table);
		return (1);
	}
	if (init_table(table))
	{
		print_and_free(table);
		return (1);
	}
	clean_exit(table);
	return (0);
}
