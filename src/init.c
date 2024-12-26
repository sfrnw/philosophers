/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:12:43 by asafrono          #+#    #+#             */
/*   Updated: 2024/12/26 14:37:09 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Initializes a single philosopher's attributes:
// Sets parameters from command line arguments
// Sets last meal time (lmt) to simulation start time
// Initializes number of times each philosopher must eat (notepme)
// Sets meals eaten counter (me) to 0
// Links philosopher to main data structure
void	init_philo(t_philo *philo, t_data *data, int argc, char **argv)
{
	philo->nop = ft_atoi(argv[1]);
	philo->ttd = ft_atoi(argv[2]);
	philo->tte = ft_atoi(argv[3]);
	philo->tts = ft_atoi(argv[4]);
	philo->lmt = data->start_time;
	if (argc == 6)
		philo->notepme = ft_atoi(argv[5]);
	else
		philo->notepme = -1;
	philo->me = 0;
	philo->data = data;
}

// Initializes the main data structure:
// Initializes pointers to NULL and simulation_stop flag to 0
// Sets simulation start time
// Initializes mutexes for writing and stopping simulation
// Allocates memory for philosophers and forks
// Creates and initializes all philosopher structures
void	init_data(t_data *data, int argc, char **argv)
{
	int	i;
	int	num_philos;

	data->philos = NULL;
	data->forks = NULL;
	data->simulation_stop = 0;
	data->start_time = get_current_time();
	pthread_mutex_init(&data->write_mutex, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
	num_philos = ft_atoi(argv[1]);
	if (!allocate_memory(data, num_philos))
		exit(1);
	i = -1;
	while (++i < num_philos)
	{
		data->philos[i].id = i + 1;
		init_philo(&data->philos[i], data, argc, argv);
	}
}

// Sets up philosopher threads and fork assignments:
// Assigns left and right forks to each philosopher
// Creates a thread for each philosopher using philo_routine
// Handles thread creation failures with proper cleanup
// Returns 0 on success, 1 on failure
int	init_philosophers(t_data *data)
{
	int	i;
	int	num_philos;

	num_philos = data->philos[0].nop;
	i = -1;
	while (++i < num_philos)
	{
		data->philos[i].lf = &data->forks[i];
		data->philos[i].rf = &data->forks[(i + 1) % num_philos];
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]) != 0)
		{
			write(2, "Thread creation failed\n", 23);
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i].mutex);
			free(data->philos);
			free(data->forks);
			pthread_mutex_destroy(&data->write_mutex);
			return (1);
		}
	}
	return (0);
}

// Initializes the forks:
// Creates mutex for each fork
// Assigns unique ID to each fork
// Handles mutex initialization failures with proper cleanup
// Ensures proper synchronization between philosophers
void	init_forks(t_data *data)
{
	int	i;
	int	num_philos;

	num_philos = data->philos[0].nop;
	i = -1;
	while (++i < num_philos)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
		{
			write(2, "Thread creation failed\n", 23);
			while (--i >= 0)
				pthread_join(data->philos[i].thread, NULL);
			cleanup(data);
			exit(1);
		}
		data->forks[i].id = i + 1;
	}
}
