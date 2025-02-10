/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:29:31 by asafrono          #+#    #+#             */
/*   Updated: 2025/02/10 17:10:28 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;
// Each fork is represented by a mutex. 
// This is crucial to the Dining Philosophers problem.
// Philosophers need to acquire two forks (left and right) to eat.
// By representing each fork as a mutex, we ensure that only one 
// philosopher can hold a fork at a time.
// The number of fork mutexes is equal to the number of philosophers.
//
// number_of_philosophers = nop
// time_to_die = ttd
// time_to_eat = tte
// time_to_sleep = tts
// number_of_times_each_philosopher_must_eat = notepme
// last_meal_time = lmt
// meals_eaten = me
// left_fork = lf
// right_fork = rf

typedef struct s_philo	t_philo;
typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				nop;
	int				ttd;
	int				tte;
	int				tts;
	int				notepme;
	long long		lmt;
	int				me;
	t_fork			*lf;
	t_fork			*rf;
	pthread_t		thread;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	simulation_mutex;
	long long		start_time;
	int				simulation_stop;
}	t_data;

//routine
void		*philo_routine(void *arg);
//monitor
void		*monitor(void *arg);
//init
void		init_philo(t_philo *philo, t_data *data, int argc, char **argv);
int			init_data(t_data *data, int argc, char **argv);
int			init_philosophers(t_data *data);
int			init_forks(t_data *data);
//utils
int			is_valid_num(char *str);
int			ft_atoi(const char *str);
long long	get_current_time(void);
void		print_status(t_philo *philo, char *status);
void		ft_usleep(int ms);
void		cleanup(t_data *data);
int			allocate_memory(t_data *data, int num_philos);
int			validate_arguments(int argc, char **argv);

#endif