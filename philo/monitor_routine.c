/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:44 by aabelkis          #+#    #+#             */
/*   Updated: 2025/10/24 11:20:57 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *monitor_routine(void *arg)
{
	t_monitor *monitor = (t_monitor *)arg;

	while (!monitor->someone_died)
	{
		int all_done = 1;

		for (int i = 0; i < monitor->num_of_phil; i++)
		{
			t_philo *p = &monitor->philos[i];
			long now = get_current_time_ms(); // ✅ move inside loop

			// Check death
			if (now - p->last_meal_time_ms >= p->time_to_die)
			{
				pthread_mutex_lock(&monitor->print_mutex);
				printf("%ld %d died\n", now - monitor->start_time_ms, p->id);
				pthread_mutex_unlock(&monitor->print_mutex);
				monitor->someone_died = 1;
				return NULL;
			}

			// Print status if updated
			if (p->has_new_status)
			{
				pthread_mutex_lock(&monitor->print_mutex);

				if (p->status == 1)
					printf("%ld %d is eating\n", now - monitor->start_time_ms, p->id);
				else if (p->status == 2)
					printf("%ld %d is sleeping\n", now - monitor->start_time_ms, p->id);
				else if (p->status == 3)
					printf("%ld %d is thinking\n", now - monitor->start_time_ms, p->id);

				pthread_mutex_unlock(&monitor->print_mutex);
				p->has_new_status = 0;
			}

			// Check if all philosophers finished minimum meals
			if (p->minimum_meals == 0 || p->meals_eaten < p->minimum_meals)
				all_done = 0;
		}

		if (all_done)
		{
			monitor->someone_died = 1;
			return NULL;
		}

		usleep(200); // small sleep; 100–300 µs is fine
	}

	return NULL;
}

