/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleeping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:22 by aabelkis          #+#    #+#             */
/*   Updated: 2025/11/20 16:01:07 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void custom_sleep(int duration_ms)
{
	long start;
	
	start = get_current_time_ms();
	while (get_current_time_ms() - start < duration_ms)
		usleep(100); // sleep 100 µs steps
}

/*void custom_sleep(t_philo *philo, long duration_ms)
{
    long start = get_current_time_ms();
    while (!philo->monitor->someone_died)
    {
        long now = get_current_time_ms();
        if (now - start >= duration_ms)
            break; // finished sleeping

        // small sleep step to avoid busy waiting
        usleep(500); // 0.5 ms

        // optional: early exit if sleeping too long would kill philosopher
        if (now - philo->last_meal_time_ms >= philo->time_to_die)
            break;
    }
}*/

