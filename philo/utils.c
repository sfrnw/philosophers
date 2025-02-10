/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:45:20 by asafrono          #+#    #+#             */
/*   Updated: 2025/02/10 16:21:58 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Validates if a string represents a valid number:
// Skips whitespace characters at the start
// Rejects strings with '+' or '-' signs
// Checks if string contains only digits
// Ensures number doesn't exceed INT_MAX
// Returns 1 if valid, 0 if invalid
int	is_valid_num(char *str)
{
	long long	num;

	num = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '+' || *str == '-')
		return (0);
	if (!*str)
		return (0);
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		num = num * 10 + (*str - '0');
		if (num > INT_MAX)
			return (0);
		str++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

// Gets current time in milliseconds:
// Uses gettimeofday to get current time
// Converts seconds to milliseconds
// Adds microseconds converted to milliseconds
// Returns total milliseconds since epoch
long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Prints philosopher status in a thread-safe manner:
// Locks write mutex to prevent message mixing
// Checks if simulation is still running
// Prints timestamp, philosopher ID, and status
// Unlocks mutex after printing
void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->simulation_mutex);
	if (!philo->data->simulation_stop)
		printf("%lld %d %s\n", get_current_time()
			- philo->data->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->data->simulation_mutex);
}

// Custom sleep function for precise timing:
// Records start time
// Sleeps in small intervals (20μs)
// Continues until desired milliseconds have passed
// More precise than standard usleep for longer durations
void	ft_usleep(int ms)
{
	long long	start;

	start = get_current_time();
	while (get_current_time() - start < ms)
		usleep(20);
}
