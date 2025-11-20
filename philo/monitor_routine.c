/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:44 by aabelkis          #+#    #+#             */
/*   Updated: 2025/11/20 16:20:56 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *monitor_routine(void *arg)
{
    t_monitor *monitor = (t_monitor *)arg;

    while (1)
    {
        pthread_mutex_lock(&monitor->print_mutex);
        int someone_died = monitor->someone_died;
        pthread_mutex_unlock(&monitor->print_mutex);
        if (someone_died)
            break;

        int all_done = 1;
        long now = get_current_time_ms();

        for (int i = 0; i < monitor->num_of_phil; i++)
        {
            t_philo *p = &monitor->philos[i];

            pthread_mutex_lock(&monitor->print_mutex);
            long last_meal = p->last_meal_time_ms;
            int status = p->status;
            int new_status = p->has_new_status;
            int meals = p->meals_eaten;
            int min_meals = p->minimum_meals;
            pthread_mutex_unlock(&monitor->print_mutex);

            // Check death (die at >= time_to_die)
            if (now - last_meal >= p->time_to_die)
            {
                pthread_mutex_lock(&monitor->print_mutex);
                printf("%ld %d died\n", now - monitor->start_time_ms, p->id);
                monitor->someone_died = 1;
                pthread_mutex_unlock(&monitor->print_mutex);
                return NULL;
            }

            // Print status if updated
            if (new_status)
            {
                pthread_mutex_lock(&monitor->print_mutex);
                switch (status)
                {
                    case 1: printf("%ld %d is eating\n", now - monitor->start_time_ms, p->id); break;
                    case 2: printf("%ld %d is sleeping\n", now - monitor->start_time_ms, p->id); break;
                    case 3: printf("%ld %d is thinking\n", now - monitor->start_time_ms, p->id); break;
                    default: break;
                }
                p->has_new_status = 0; // mark as printed
                pthread_mutex_unlock(&monitor->print_mutex);
            }

            // Check if all philosophers finished minimum meals
            if (min_meals == 0 || meals < min_meals)
                all_done = 0;
        }

        if (all_done)
        {
            pthread_mutex_lock(&monitor->print_mutex);
            monitor->someone_died = 1;
            pthread_mutex_unlock(&monitor->print_mutex);
            return NULL;
        }

        usleep(500); // small sleep to avoid busy waiting and be more responsive
    }

    return NULL;
}