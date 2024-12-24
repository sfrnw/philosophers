/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 13:30:00 by asafrono          #+#    #+#             */
/*   Updated: 2024/12/24 14:17:25 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
			all_ate &= check_meals(data, i);
		}
		if (check_simulation_end(data, all_ate))
			return (NULL);
		usleep(5);
	}
}
