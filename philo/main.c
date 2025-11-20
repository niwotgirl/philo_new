/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:06 by aabelkis          #+#    #+#             */
/*   Updated: 2025/11/20 18:07:55 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	int			i;
	int			num_of_phil;
	t_philo		*philo_st;
	t_monitor	monitor;
	long		start;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
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
	// Initialize philosophers
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
	// Special-case: single philosopher
	if (num_of_phil == 1)
	{
		pthread_mutex_lock(&monitor.print_mutex);
		printf("0 1 has taken a fork\n");
		pthread_mutex_unlock(&monitor.print_mutex);
		start = get_current_time_ms();
		while (get_current_time_ms() - start < philo_st[0].time_to_die)
			usleep(1000);
		pthread_mutex_lock(&monitor.print_mutex);
		printf("%ld 1 died\n", get_current_time_ms() - monitor.start_time_ms);
		pthread_mutex_unlock(&monitor.print_mutex);
		pthread_mutex_destroy(&monitor.print_mutex);
		free(philo_st);
		return (0);
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
