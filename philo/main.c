/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:26:21 by asafrono          #+#    #+#             */
/*   Updated: 2025/02/10 11:57:16 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Checks if there are either 5 or 6 arguments
// Verifies each argument is a valid number using is_valid_num
// Returns 1 if arguments are valid, 0 otherwise
int	validate_arguments(int argc, char **argv)
{
	return ((argc == 6 && is_valid_num(argv[1]) && is_valid_num(argv[2])
			&& is_valid_num(argv[3]) && is_valid_num(argv[4])
			&& is_valid_num(argv[5])) || (argc == 5 && is_valid_num(argv[1])
			&& is_valid_num(argv[2]) && is_valid_num(argv[3])
			&& is_valid_num(argv[4])));
}

// Initialization:
// 	Creates data structure and monitor thread
// 	Initializes all components
// Simulation Flow:
// 	Creates monitor thread to watch for deaths/completion
// 	Waits for monitor thread to finish (pthread_join)
// 	Handles cleanup when simulation ends
// The main function follows a clear sequence: 
// validate → initialize → simulate → cleanup,
// with proper error handling at each step

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;

	monitor_thread = 0;
	if (!validate_arguments(argc, argv))
		return (write(2, "Invalid arguments\n", 18), 1);
	if (init_data(&data, argc, argv) != 0 || init_forks(&data) != 0
		|| init_philosophers(&data) != 0)
		return (write(2, "Initialization failed\n", 23), cleanup(&data), 1);
	if (pthread_create(&monitor_thread, NULL, monitor, &data) != 0)
		return (write(2, "Failed to create monitor thread\n", 32),
			cleanup(&data), 1);
	pthread_join(monitor_thread, NULL);
	return (cleanup(&data), write(1, "Simulation completed\n", 21), 0);
}
