/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:52:33 by antbonin          #+#    #+#             */
/*   Updated: 2025/06/06 17:45:42 by antbonin         ###   ########.fr       */
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
	int				*forks_state;
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	update;
	pthread_mutex_t	is_dead;
	pthread_mutex_t	forks_mutex;
	t_philo			*philos;
	pthread_mutex_t	*forks;
}					t_data;

/**********************utils**************************/

int					is_digit(char **av);
int					ft_atoi(char *str);
int					get_current_time(void);
void				cleanup(t_data *data);
int					should_stop(t_data *data);
void				print_message(t_data *data, int id, char *msg);
int					init_data_malloc(t_data *data);

/*********************thread**************************/

void				*philo_routine(void *arg);
void				thinking(t_philo *philo);
void				*monitor_routine(void *arg);

/*********************forks**************************/

int					forks_lock_two(t_philo *philo);

#endif