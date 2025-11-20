/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:44 by aabelkis          #+#    #+#             */
/*   Updated: 2025/11/20 18:14:28 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitor_routine(void *arg)
{
	t_monitor	*monitor;
	int			someone_died;
	int			all_done;
	long		now;
	int			i;
	t_philo		*p;

	monitor = (t_monitor *)arg;
	while (1)
	{
		pthread_mutex_lock(&monitor->print_mutex);
		someone_died = monitor->someone_died;
		pthread_mutex_unlock(&monitor->print_mutex);
		if (someone_died)
			break ;
		all_done = 1;
		now = get_current_time_ms();
		i = 0;
		while (i < monitor->num_of_phil)
		{
			p = &monitor->philos[i];
			pthread_mutex_lock(&monitor->print_mutex);
			long last_meal = p->last_meal_time_ms;
			int status = p->status;
			int new_status = p->has_new_status;
			int meals = p->meals_eaten;
			int min_meals = p->minimum_meals;
			pthread_mutex_unlock(&monitor->print_mutex);
			// Death check
			if (now - last_meal >= p->time_to_die)
			{
				pthread_mutex_lock(&monitor->print_mutex);
				printf("%ld %d died\n", now - monitor->start_time_ms, p->id);
				monitor->someone_died = 1;
				pthread_mutex_unlock(&monitor->print_mutex);
				return (NULL);
			}
			// Status printing
			if (new_status)
			{
				pthread_mutex_lock(&monitor->print_mutex);
				if (status == 1)
					printf("%ld %d is eating\n", now - monitor->start_time_ms, p->id);
				else if (status == 2)
					printf("%ld %d is sleeping\n", now - monitor->start_time_ms, p->id);
				else if (status == 3)
					printf("%ld %d is thinking\n", now - monitor->start_time_ms, p->id);
				p->has_new_status = 0;
				pthread_mutex_unlock(&monitor->print_mutex);
			}
			// Check if minimum meals requirement is met
			if (min_meals == 0 || meals < min_meals)
				all_done = 0;
			i++;
		}
		if (all_done)
		{
			pthread_mutex_lock(&monitor->print_mutex);
			monitor->someone_died = 1;
			pthread_mutex_unlock(&monitor->print_mutex);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
