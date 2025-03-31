/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:52:33 by antbonin          #+#    #+#             */
/*   Updated: 2025/03/31 15:53:11 by antbonin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "pthread.h"
# include "stdio.h"
# include "stdlib.h"
# include "sys/time.h"
# include <unistd.h>

# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DEAD "died"

typedef struct s_philo
{
	int				id;
	long			last_meal;
	int				is_eating;
	int				wait;
	int				meal_eat;
	pthread_t		thread;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	struct s_data	*data;

}					t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				nb_eat;
	long			start_time;
	int				stop;
	int				waiting_last;
	int				mutex_init;
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	update;
	pthread_mutex_t	is_dead;
	t_philo			*philos;
	pthread_mutex_t	*forks;
}					t_data;

/**********************utils**************************/

int					is_digit(char **av);
int					ft_atoi(char *str);
void				*philo_routine(void *arg);
int					get_current_time(void);
void				*monitor_routine(void *arg);
void				cleanup(t_data *data);
int					should_stop(t_data *data);
void				print_message(t_data *data, int id, char *msg);

/*****************************************************/

#endif