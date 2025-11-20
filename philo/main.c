/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:06 by aabelkis          #+#    #+#             */
/*   Updated: 2025/10/20 20:53:46 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//this is the working one
#include "philosophers.h"
/*int main(int argc, char** argv)
{
	int i;
	int val;
    int j;
	int num_of_phil;
	t_philo *philo_st; //philo struct - which holds thread - maybe this needs to be a round linked list
	val = 0;
	i = 1; // helps itearte through argv
	j = 0; //helsp iterate through array of philos
	//here I find out num of philosophers and initiate their structs - do i need an array of philos or a linked list or something...

	//need to store each val in each struct
	//need to retrieve vals and then itterate through the philo structs adding them
	//once all created set start time for all- can I use a pointer to update all at once?
	//i - 2+
	while (argc > i)
	{
		val = ft_error(argc, argv, i);
		if (val == -1)
			return(1);
		else // cycle through each philo and give them the val - if num of phils - set array
		{
			if (i == 1)
			{
				num_of_phil = val;//might be unnecsary 
				philo_st = malloc(sizeof(t_philo) * val + 1);
				if (!philo_st)
					return(1);
				memset(philo_st, 0, sizeof(t_philo) * val); // create and set to 0;
				philo_st[val + 1] = '\0'; //unnecissary?
			}
			else
			{
                j = 0;
				while (j < num_of_phil) //for each philo
				{
					struct_init(val, j, i, &philo_st[j]);
					j++;
				}
			}
		}
		i++;
	}
	j = 0;
	while(j < num_of_phil)
	{
		pthread_mutex_init(philo_st[j].right_fork_mutex, NULL);
		j++;
	}
	j = 0;
	while(j < num_of_phil)
	{
        if (philo_st[j].id == 1)
            philo_st[j].left_fork_mutex = philo_st[num_of_phil].right_fork_mutex;
        else
            philo_st[j].left_fork_mutex = philo_st[j-1].right_fork_mutex;
		j++;
	}
	j = 0;
	while(j < num_of_phil)
	{
		if (pthread_create(philo_st[j].thread, NULL, &philo_actions, NULL) != 0)
			return(1);
		j++;
	}
	j = 0;
	while (j < num_of_phil)
	{
		if (pthread_join(philo_st[j]->thread, NULL) != 0)
			//deal with error
		j++;
	}
	return(0);
}*/

int main(int argc, char **argv)
{
    int i;
    int num_of_phil;
    t_philo *philo_st;
    t_monitor monitor;

    if (argc < 5 || argc > 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }

    num_of_phil = ft_error(argc, argv, 1);
    if (num_of_phil <= 0)
        return 1;

    philo_st = malloc(sizeof(t_philo) * num_of_phil);
    if (!philo_st)
        return 1;
    memset(philo_st, 0, sizeof(t_philo) * num_of_phil);

    monitor.philos = philo_st;
    monitor.num_of_phil = num_of_phil;
    monitor.someone_died = 0;
    monitor.start_time_ms = get_current_time_ms();
    pthread_mutex_init(&monitor.print_mutex, NULL);

    // Initialize philosophers
    for (i = 0; i < num_of_phil; i++)
    {
        struct_init(ft_error(argc, argv, 2), i, 2, &philo_st[i]);
        struct_init(ft_error(argc, argv, 3), i, 3, &philo_st[i]);
        struct_init(ft_error(argc, argv, 4), i, 4, &philo_st[i]);
        if (argc == 6)
            struct_init(ft_error(argc, argv, 5), i, 5, &philo_st[i]);
        philo_st[i].id = i + 1;
        philo_st[i].monitor = &monitor;

        // ✅ Minimal fix: set last meal time to start time
        philo_st[i].last_meal_time_ms = monitor.start_time_ms;
    }

    // Initialize right fork mutexes
    for (i = 0; i < num_of_phil; i++)
        pthread_mutex_init(&philo_st[i].right_fork_mutex, NULL);

    // Assign left fork pointers
    for (i = 0; i < num_of_phil; i++)
        philo_st[i].left_fork_mutex = &philo_st[(i + num_of_phil - 1) % num_of_phil].right_fork_mutex;

    // Create philosopher threads
    for (i = 0; i < num_of_phil; i++)
    {
        if (pthread_create(&philo_st[i].thread, NULL, &routine, &philo_st[i]) != 0)
        {
            fprintf(stderr, "Error: failed to create philosopher thread %d\n", i + 1);
            monitor.someone_died = 1;
        }
    }

    // Start monitor thread
    pthread_create(&monitor.thread, NULL, &monitor_routine, &monitor);

    // Join philosopher threads
    for (i = 0; i < num_of_phil; i++)
        pthread_join(philo_st[i].thread, NULL);

    // Join monitor thread
    pthread_join(monitor.thread, NULL);

    // Destroy mutexes
    for (i = 0; i < num_of_phil; i++)
        pthread_mutex_destroy(&philo_st[i].right_fork_mutex);
    pthread_mutex_destroy(&monitor.print_mutex);

    free(philo_st);

    return 0;
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
