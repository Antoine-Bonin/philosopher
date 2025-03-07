/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:23:25 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/07 18:29:03 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_data

int main(int ac, char **av)
{
    t_data *data;
    
    data = malloc(sizeof(t_data));
    if (!data)
        return (1);
    if (ac != 5 && ac != 6)
    {
        printf("Error: wrong number of arguments\n");
        return (1);
    }
    if (init_data(data, ac, av))
    {
        printf("Error: wrong arguments\n");
        return (1);
    }
}