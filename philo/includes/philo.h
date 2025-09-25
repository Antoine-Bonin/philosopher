/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:52:33 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/25 14:21:20 by antbonin         ###   ########.fr       */
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
# define MDEAD "died"

typedef enum e_status
{
	ALIVE,
	DEAD,
	FULL,
}					t_status;

typedef struct s_philo
{
	int				id;
	long			last_meal;
	int				is_eating;
	int				wait;
	int				meal_eat;
	pthread_t		thread;
	int				forks[2];
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
	t_status		status;
	int				mutex_init;
	int				*forks_state;
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	update;
	pthread_mutex_t	death_lock;
	t_philo			*philos;
	pthread_mutex_t	*forks;
}					t_data_table;

/**********************utils**************************/

int					is_digit(char **av);
int					ft_atoi(char *str);
int					get_current_time(void);
int					should_stop(t_data_table *data);
void				print_message(t_data_table *data, int id, char *msg);
t_status			get_status(t_data_table *data);
void				smart_usleep(t_data_table *data, int duration);

/**********************clean_exit****************************/

void				garbage_data(t_data_table *data);
void				clean_exit(t_data_table *data);
int					garbage_mutex(t_data_table *data);
int					clean_thread(t_data_table *data, int i);

/*********************init data***********************/

int					init_table(t_data_table *data);
int					init_data_table_arg(t_data_table *data, int ac, char **av);

/*********************thread**************************/

void				*philo_routine(void *arg);
void				*monitor_routine(void *arg);

/**********************forks**************************/

void				release_forks_ordered(t_philo *philo);
int					lock_second_fork(t_philo *philo, int second_fork);
int					lock_first_fork(t_philo *philo, int first_fork);
#endif