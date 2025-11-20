/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:06 by aabelkis          #+#    #+#             */
/*   Updated: 2025/10/24 11:29:23 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	int			i;
	int			num_of_phil;
	t_philo		*philo_st;
	t_monitor	monitor;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s incorrect number of arguments\n", argv[0]);
		return (1);
	}
	num_of_phil = ft_error(argc, argv, 1);
	if (num_of_phil <= 0)
		return (1);
	philo_st = malloc(sizeof(t_philo) * num_of_phil);
	if (!philo_st)
		return (1);
	memset(philo_st, 0, sizeof(t_philo) * num_of_phil);
	monitor.philos = philo_st;
	monitor.num_of_phil = num_of_phil;
	monitor.someone_died = 0;
	monitor.start_time_ms = get_current_time_ms();
	pthread_mutex_init(&monitor.print_mutex, NULL);
	i = 0;
	while (i < num_of_phil)
	{
		struct_init(ft_error(argc, argv, 2), i, 2, &philo_st[i]);
		struct_init(ft_error(argc, argv, 3), i, 3, &philo_st[i]);
		struct_init(ft_error(argc, argv, 4), i, 4, &philo_st[i]);
		if (argc == 6)
			struct_init(ft_error(argc, argv, 5), i, 5, &philo_st[i]);

		philo_st[i].id = i + 1;
		philo_st[i].monitor = &monitor;
		philo_st[i].last_meal_time_ms = monitor.start_time_ms;
		i++;
	}
	// Initialize right fork mutexes
	i = 0;
	while (i < num_of_phil)
	{
		pthread_mutex_init(&philo_st[i].right_fork_mutex, NULL);
		i++;
	}
	// Assign left fork pointers
	i = 0;
	while (i < num_of_phil)
	{
		philo_st[i].left_fork_mutex = &philo_st[(i + num_of_phil - 1) % num_of_phil].right_fork_mutex;
		i++;
	}
	// Create philosopher threads
	i = 0;
	while (i < num_of_phil)
	{
		if (pthread_create(&philo_st[i].thread, NULL, &routine, &philo_st[i]) != 0)
		{
			printf("Error: failed to create philosopher thread %d\n", i + 1);
			monitor.someone_died = 1;
		}
		i++;
	}
	// Start monitor thread
	pthread_create(&monitor.thread, NULL, &monitor_routine, &monitor);
	// Join philosopher threads
	i = 0;
	while (i < num_of_phil)
	{
		pthread_join(philo_st[i].thread, NULL);
		i++;
	}
	// Join monitor thread
	pthread_join(monitor.thread, NULL);
	// Destroy mutexes
	i = 0;
	while (i < num_of_phil)
	{
		pthread_mutex_destroy(&philo_st[i].right_fork_mutex);
		i++;
	}
	pthread_mutex_destroy(&monitor.print_mutex);
	free(philo_st);
	return (0);
}


		//takes following arguments
	/*Your program(s) must take the following arguments:
[1]number_of_philosophers [2]time_to_die [3]time_to_eat [4]time_to_sleep
[5][number_of_times_each_philosopher_must_eat]
//for 3 - 1, 2, 3 repeat
//for 5: 1&3, 2&4, 5&3, 1&4, 2&5 repeat

◦ number_of_philosophers: The number of philosophers and also the number
of forks.
◦ time_to_die (in milliseconds): If a philosopher has not started eating within
time_to_die milliseconds since the start of their last meal or the start of the
simulation, they die.
◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.
• Each philosopher has a number ranging from 1 to number_of_philosophers.
• Philosopher number 1 sits next to philosopher number number_of_philosophers.
Any other philosopher, numbered N, sits between philosopher N - 1 and philosopher
N + 1.
	*/
	//could use pthread-detach and run at intervals for monitor
	//pass pointer to struct
	///if tte > ttd, tte = ttd - i think like they will die while eating
