/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:01:13 by asafrono          #+#    #+#             */
/*   Updated: 2024/12/26 15:03:23 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// This function checks if the simulation should stop. It:
// Uses mutex to safely access the shared simulation_stop flag
// Returns the current state of simulation_stop
// Prevents race conditions when checking simulation status
static int	check_stop(t_philo *philo)
{
	int	should_stop;

	pthread_mutex_lock(&philo->data->stop_mutex);
	should_stop = philo->data->simulation_stop;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	return (should_stop);
}

// Manages philosopher's thinking phase:
// Calculates safe thinking time based on:
// 	Time since last meal
// 	Time to die
// 	Time needed to eat
// Limits thinking time to time_to_die/nop to prevent starvation
// Ensures minimum thinking time of 1ms
// Prints thinking status and sleeps for calculated duration
static void	philo_think(t_philo *philo)
{
	long long	safe_think_time;
	long long	time_since_last_meal;

	time_since_last_meal = get_current_time() - philo->lmt;
	safe_think_time = philo->ttd - time_since_last_meal - philo->tte;
	if (safe_think_time > philo->ttd / philo->nop)
		safe_think_time = philo->ttd / philo->nop;
	if (safe_think_time < 1)
		safe_think_time = 1;
	print_status(philo, "is thinking");
	ft_usleep(safe_think_time);
}

// Handles philosopher's eating process:
// Implements fork pickup based on philosopher ID to prevent deadlocks:
// 	Even-numbered philosophers take right fork first
// 	Odd-numbered philosophers take left fork first
// Updates last meal time
// Increments meals eaten counter
// Explicitly puts forks back after eating
// Prints all state changes (taking forks, eating, putting forks back)
static void	philo_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->rf->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->lf->mutex);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->lf->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->rf->mutex);
		print_status(philo, "has taken a fork");
	}
	print_status(philo, "is eating");
	philo->lmt = get_current_time();
	ft_usleep(philo->tte);
	philo->me++;
	pthread_mutex_unlock(&philo->lf->mutex);
	pthread_mutex_unlock(&philo->rf->mutex);
	print_status(philo, "has put the forks back");
}

// Manages philosopher's sleeping phase:
// Prints sleeping status
// Makes philosopher sleep for specified duration (time_to_sleep)
static void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->tts);
}

// Main routine for each philosopher thread:
// Runs continuously until simulation stops
// Implements the philosopher's lifecycle:
// 	Think
// 	Eat
// 	Sleep
// Checks for simulation stop after each action
// Returns NULL when simulation ends
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!check_stop(philo))
	{
		philo_think(philo);
		if (check_stop(philo))
			break ;
		philo_eat(philo);
		if (check_stop(philo))
			break ;
		philo_sleep(philo);
		if (check_stop(philo))
			break ;
	}
	return (NULL);
}
