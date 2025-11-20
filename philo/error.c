/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:59:40 by aabelkis          #+#    #+#             */
/*   Updated: 2025/10/20 11:46:33 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

    /*Your program(s) must take the following arguments:
[1]number_of_philosophers [2]time_to_die [3]time_to_eat [4]time_to_sleep
[5][number_of_times_each_philosopher_must_eat]*/
//called from main - checks that we have workable data - if not prints something and returns 1
int ft_error(int argc, char **argv, int i)
{
	int val;
	val = 0;
	if (argc < 5 || argc > 6)
	{
		printf("input needed: number_of_philosophers, time_to_die, time_to_eat, time_to_sleep, and optionally: number_of_times_each_philosopher_must_eat\n");
		return(-1);
	}
	val = ft_error_atoi(argv[i]);
	if (val < 0) 
	{
		printf("error: only (positive) numbers taken as params\n");
		return(-1);
	}
	if (i == 1 && val == 0)
	{
		printf("error: minimum 1 philo needed and other params must be positive\n");
		return(-1);
	}
	if (i == 1 && val == 1) // this is the one I might need to delete
	{
		printf("philo stuck holding 1 fork\n");
		return(-1);
	}
	if (i == 2 && val == 0)
	{
		printf("philo died immediatly\n"); // could just say get_time_of_day() philo _ died
		return(-1);
	}	
	if (i == 3 && val == 0)
	{
		printf("philos ate fast\n"); // this one still works actually - they just sleep until have had enough meals - can still send through
		return(-1);
	}
	//with sleep time == 0 -> they just never sleep
	if (i == 5 && val == 0)
	{
		printf("all philos met minimum meal\n"); //or just say sim ended
		return(-1);
	}
	return(val);
}
