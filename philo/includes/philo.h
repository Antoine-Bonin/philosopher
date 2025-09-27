/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antbonin <antbonin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:52:33 by antbonin          #+#    #+#             */
/*   Updated: 2025/09/27 15:01:05 by antbonin         ###   ########.fr       */
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
}						t_status;

typedef struct s_philo
{
	int					id;
	long				last_meal;
	int					is_eating;
	int					wait;
	int					meal_eat;
	pthread_t			thread;
	int					forks[2];
	struct s_data_table	*table;
}						t_philo;

typedef struct s_data_table
{
	int					nb_philo;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_die;
	int					nb_eat;
	long				start_time;
	t_status			status;
	int					mutex_init;
	int					*forks_state;
	pthread_t			monitor_thread;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		update;
	pthread_mutex_t		death_lock;
	pthread_mutex_t		*forks;
	pthread_mutex_t		wait_thread_create;
	t_philo				*philos;
}						t_data_table;

/**********************utils**************************/

int						is_digit(char **av);
int						ft_atoi(char *str);
int						get_current_time(void);
int						should_stop(t_data_table *table);
void					print_message(t_data_table *table, int id, char *msg);
t_status				get_status(t_data_table *table);
void					smart_usleep(t_data_table *table, int duration);

/**********************clean_exit****************************/

void					garbage_data(t_data_table *table);
void					clean_exit(t_data_table *table);
int						garbage_mutex(t_data_table *table);
int						clean_thread(t_data_table *table, int i);

/*********************init data***********************/

int						init_table(t_data_table *table);
int						init_data_table_arg(t_data_table *table, int ac,
							char **av);

/*********************thread**************************/

void					*philo_routine(void *arg);
void					*monitor_routine(void *arg);

/**********************forks**************************/

int						check_fork_availability(t_philo *philo, int fork_id);
void					release_forks_ordered(t_philo *philo);
int						lock_second_fork(t_philo *philo, int second_fork);
int						lock_first_fork(t_philo *philo, int first_fork);
#endif