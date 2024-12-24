/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:01:13 by asafrono          #+#    #+#             */
/*   Updated: 2024/12/24 13:49:31 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	check_stop(t_philo *philo)
{
	int	should_stop;

	pthread_mutex_lock(&philo->data->stop_mutex);
	should_stop = philo->data->simulation_stop;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	return (should_stop);
}

static void	philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

static void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->lf->mutex);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->rf->mutex);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	philo->lmt = get_current_time();
	ft_usleep(philo->tte);
	philo->me++;
	pthread_mutex_unlock(&philo->lf->mutex);
	pthread_mutex_unlock(&philo->rf->mutex);
}

static void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->tts);
}

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
	}
	return (NULL);
}
