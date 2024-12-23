/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asafrono <asafrono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:26:21 by asafrono          #+#    #+#             */
/*   Updated: 2024/12/23 16:35:35 by asafrono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	validate_arguments(int argc, char **argv)
{
	if (argc == 6 && is_valid_num(argv[1]) && is_valid_num(argv[2]) && is_valid_num(argv[3]) && is_valid_num(argv[4]) && is_valid_num(argv[5]))
		return (1);
	if (argc == 5 && is_valid_num(argv[1]) && is_valid_num(argv[2]) && is_valid_num(argv[3]) && is_valid_num(argv[4]))
		return (1);
	return (0);
}

void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int should_stop = 0;

    while (!should_stop)
    {
        // Check simulation_stop flag with a mutex lock
        pthread_mutex_lock(&philo->data->stop_mutex);
        should_stop = philo->data->simulation_stop;
        pthread_mutex_unlock(&philo->data->stop_mutex);

        if (should_stop)
            break;

        // Thinking
        print_status(philo, "is thinking");

        // Check again after thinking
        pthread_mutex_lock(&philo->data->stop_mutex);
        should_stop = philo->data->simulation_stop;
        pthread_mutex_unlock(&philo->data->stop_mutex);
        if (should_stop)
            break;

        // Eating
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

        // Check again after eating
        pthread_mutex_lock(&philo->data->stop_mutex);
        should_stop = philo->data->simulation_stop;
        pthread_mutex_unlock(&philo->data->stop_mutex);
        if (should_stop)
            break;

        // Sleeping
        print_status(philo, "is sleeping");
        ft_usleep(philo->tts);
    }
    return NULL;
}

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg;
    int i, all_ate;

    while (1)
    {
        i = -1;
        all_ate = 1;
        while (++i < data->philos[0].nop)
        {
            pthread_mutex_lock(&data->write_mutex);
            if (get_current_time() - data->philos[i].lmt > data->philos[i].ttd)
            {
                print_status(&data->philos[i], "died");
                pthread_mutex_lock(&data->stop_mutex);
                data->simulation_stop = 1;
                pthread_mutex_unlock(&data->stop_mutex);
                pthread_mutex_unlock(&data->write_mutex);
                return NULL;
            }
            if (data->philos[i].notepme != -1 && data->philos[i].me < data->philos[i].notepme)
                all_ate = 0;
            pthread_mutex_unlock(&data->write_mutex);
        }
        if (data->philos[0].notepme != -1 && all_ate)
        {
            pthread_mutex_lock(&data->stop_mutex);
            data->simulation_stop = 1;
            pthread_mutex_unlock(&data->stop_mutex);
            return NULL;
        }
        usleep(1000);
    }
}


int main(int argc, char **argv)
{
	t_data data;
	pthread_t monitor_thread = 0;
	int monitor_created = 0;

	if (validate_arguments(argc, argv))
	{
		init_data(&data, argc, argv);
		init_forks(&data);
		if (init_philosophers(&data) != 0)
			return (cleanup(&data),1);
		if (pthread_create(&monitor_thread, NULL, monitor, &data) == 0)
			monitor_created = 1;
		else
		{
			write(2, "Failed to create monitor thread\n", 32);
			cleanup(&data);
			return (1);
		}
		if (monitor_created)
			pthread_join(monitor_thread, NULL);
		cleanup(&data);
		write(1, "Simulation completed\n", 21);
	}
	else
		return (write(2, "Invalid arguments\n", 18), 1);
	return (0);
}

