/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:29:07 by asafrono          #+#    #+#             */
/*   Updated: 2025/02/10 16:41:12 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// It allocates memory for the philosophers array using malloc.
// If allocation fails, it prints an error message and returns 0.
// It then allocates memory for the forks array.
// If fork allocation fails, it frees the previously 
// allocated philosopher memory, prints an error, and returns 0.
// If both allocations succeed, it returns 1.
int	allocate_memory(t_data *data, int num_philos)
{
	data->philos = malloc(sizeof(t_philo) * num_philos);
	if (!data->philos)
		return (write(2, "MA failed for philosophers\n", 27), 0);
	memset(data->philos, 0, sizeof(t_philo) * num_philos);
	data->forks = malloc(sizeof(t_fork) * num_philos);
	if (!data->forks)
		return (free(data->philos), write(2, "MA failed for forks\n", 20), 0);
	memset(data->forks, 0, sizeof(t_fork) * num_philos);
	return (1);
}

// This function handles the cleanup process after the simulation ends:
// It takes a pointer to the t_data structure as an argument.
// It iterates through all philosophers:
// 	Joins any active philosopher threads (pthread_join).
// 	Destroys the mutex for each fork.
// It destroys the write mutex and stop mutex.
// Finally, it frees the allocated memory for philosophers and forks.
// This function ensures proper resource deallocation, preventing memory 
// leaks and cleaning up all created threads and mutexes.
void	cleanup(t_data *data)
{
	int	i;
	int	num_philos;

	i = -1;
	num_philos = data->philos[0].nop;
	while (++i < num_philos)
	{
		if (data->philos[i].thread != 0)
			pthread_join(data->philos[i].thread, NULL);
		pthread_mutex_destroy(&data->forks[i].mutex);
	}
	pthread_mutex_destroy(&data->simulation_mutex);
	free(data->philos);
	free(data->forks);
}
