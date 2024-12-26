/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 13:30:00 by asafrono          #+#    #+#             */
/*   Updated: 2024/12/26 14:35:20 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// This function checks if a philosopher has died:
// Uses mutex to safely access shared data
// Calculates time since philosopher's last meal
// If time since last meal exceeds time_to_die:
// 	Prints death message
// 	Sets simulation_stop flag
// 	Returns 1 to indicate death
// Returns 0 if philosopher is alive
static int	check_death(t_data *data, int i)
{
	pthread_mutex_lock(&data->stop_mutex);
	if (get_current_time() - data->philos[i].lmt > data->philos[i].ttd)
	{
		print_status(&data->philos[i], "died");
		data->simulation_stop = 1;
		pthread_mutex_unlock(&data->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->stop_mutex);
	return (0);
}

// Verifies if a philosopher has eaten enough times:
// Uses mutex to protect meal count access
// If number_of_times_each_philosopher_must_eat is set (-1 means unlimited):
// 	Returns 0 if philosopher hasn't eaten enough
// 	Returns 1 if philosopher has reached required meals
// Ensures thread-safe access to meal counters
static int	check_meals(t_data *data, int i)
{
	pthread_mutex_lock(&data->write_mutex);
	if (data->philos[i].notepme != -1
		&& data->philos[i].me < data->philos[i].notepme)
	{
		pthread_mutex_unlock(&data->write_mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->write_mutex);
	return (1);
}

// Determines if simulation should end:
// Checks two conditions under mutex protection:
// 	If simulation_stop flag is set (death occurred)
// 	If all philosophers have eaten required meals
// Sets simulation_stop flag if conditions are met
// Returns 1 to end simulation, 0 to continue
static int	check_simulation_end(t_data *data, int all_ate)
{
	pthread_mutex_lock(&data->stop_mutex);
	if (data->simulation_stop || (data->philos[0].notepme != -1 && all_ate))
	{
		data->simulation_stop = 1;
		pthread_mutex_unlock(&data->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->stop_mutex);
	return (0);
}

// Main monitoring thread that:
// Continuously checks all philosophers
// For each philosopher:
// 	Checks if they died
// 	Tracks if they've eaten enough
// Ends simulation if:
// 	Any philosopher dies
// 	All philosophers have eaten enough
void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	int		all_ate;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		all_ate = 1;
		while (++i < data->philos[0].nop)
		{
			if (check_death(data, i))
				return (NULL);
			if (!check_meals(data, i))
				all_ate = 0;
		}
		if (check_simulation_end(data, all_ate))
			return (NULL);
		usleep(20);
	}
}
