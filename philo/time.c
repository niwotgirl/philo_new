/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:21:46 by aabelkis          #+#    #+#             */
/*   Updated: 2025/10/20 21:18:48 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
/*long get_current_time_ms(void)
{
    struct timeval tval;
    gettimeofday(&tval, NULL);
    return (tval.tv_sec * 1000 + tval.tv_usec / 1000);
}*/
long get_current_time_ms(void)
{
    struct timeval tval;
    gettimeofday(&tval, NULL);
    return (tval.tv_sec * 1000L + tval.tv_usec / 1000L);
}

long timestamp_ms(long start_time_ms)
{
    long now = get_current_time_ms();
    long delta = now - start_time_ms;
    return (delta < 0) ? 0 : delta; // prevent negative numbers
}

//returns an int 
///philo_st->last_meal_time_ms = get_current_time_ms
//later 
//long current time = get_current_time_ms();
//if (current time - philo_st->last_meal_time_ms > philo_st->time_to_die) - meaning: time elapsed > than death timer
//  { philo dies to xyz }
