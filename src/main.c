/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:26:21 by asafrono          #+#    #+#             */
/*   Updated: 2024/12/24 13:49:22 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	validate_arguments(int argc, char **argv)
{
	if (argc == 6 && is_valid_num(argv[1]) && is_valid_num(argv[2])
		&& is_valid_num(argv[3]) && is_valid_num(argv[4])
		&& is_valid_num(argv[5]))
		return (1);
	if (argc == 5 && is_valid_num(argv[1]) && is_valid_num(argv[2])
		&& is_valid_num(argv[3]) && is_valid_num(argv[4]))
		return (1);
	return (0);
}

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
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->philos);
	free(data->forks);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;

	monitor_thread = 0;
	if (validate_arguments(argc, argv))
	{
		init_data(&data, argc, argv);
		init_forks(&data);
		if (init_philosophers(&data) != 0)
			return (cleanup(&data), 1);
		if (pthread_create(&monitor_thread, NULL, monitor, &data) != 0)
		{
			write(2, "Failed to create monitor thread\n", 32);
			cleanup(&data);
			return (1);
		}
		pthread_join(monitor_thread, NULL);
		cleanup(&data);
		write(1, "Simulation completed\n", 21);
	}
	else
		return (write(2, "Invalid arguments\n", 18), 1);
	return (0);
}
