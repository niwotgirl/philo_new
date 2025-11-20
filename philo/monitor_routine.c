/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:44 by aabelkis          #+#    #+#             */
/*   Updated: 2025/11/20 16:06:58 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
/*void *monitor_routine()
{
here I am maining jump starting sim and checking if dead*/
    /*
    gettimeofday() - returns 0 on success and -1 on failure and erno variable is set
    key-> philos can write to something ex. monitor But cant read from a shared thing***
    10 ms is max delay allowed 
        > need time to die >= time to sleep + eat
    good test: 800, 10000, 1, 0
    look up: bankers algorithm and coffmans conditions
    since cant use malloc (but i think I mean exit): have functions return all the way back to main(return ;)
    useful to assign id of philo to fork when they hold it or -1 when it is on table -> communicating??
    check if dead
    if dead 
        > can he lock all forks?
        > tell all to exit 
        > need 1 ptr in each struct that = dead philo
            > check if dead before each action - but ensure philo's arent talking to each other
            > if dead all break out and set ptr to NULL
        > free all and set ptr to Null
        > let every thread finish by themselves 
        > you can make sure they end by joining them in main thread
        > insure no thread left running in background and can safely destroy all mutexes
        > when philo dies -> stop program w/in a few ms
            > dont wait till time to eat/sleep runs out


    > maybe have 1 ptr/mutex that allows them to print - or something in monitor (mutex?) lets them
        > each have own "write mutex" and if none of others are locked can write - but then this is them communicating potentially
        > doesn't involve talking - tell monitor "I want to print" and then monitor decides - has a mutex - if locked lets him pass - tells thread it printed - can it do this?
            > if want to print sets var to 1 and monitor sets it back... idk 
    > make sure to lock writing(printing) 
        > also lock printf -> to ensure not printing in incorrect order
    > figure out how to write the death message when philo is busy eating (usleep) and holding forks
    > could use pthread_detach -> run @ intervals 
        > every philosopher should be its own thread but you can have extra threads 
        > it's just forbidden for philosophers to share threads - wait... then does each philo needs it own minitor...
    > lock common write(sd) mutex
        > if philo died should hold the write mutex and dont unlock it
            > prevents other philo from writing 
            > starve others until death
                > philos aren't communicating
    > calculate arbitrary start times based on num of threads and all threads wait before they start to execute (not perfect but works)
    > "Best to init all and then run sim -> could set a var that monitor checks to see if init done"
    > don't let them start routine until your've initialized everything successfully
    > time to die claculatd from start of eating 
        > last meal (time) updated and then usleep of meal**
            > time to eat 150 and sleep 100 will die...?
    with odd num:
        for 3: 1,2,3(repeat)
        for 5: 1&3, 2&4, 5&3, 1&4, 2&5 (repeat)
    each philo has individual logic to:
        > know if must star eating immediatly or wait
        > After eating know if must wait "normal" or "double"
    Can't eat and die at same time
        > solution: time_of_last_meal mutex for each philo
            > only accessed by philo itself (updates on it own and by death monitor - when check whether or not to kill philo)
            > OR check if philo is dead before eating
    > if you dn't check whether they are alive before every action can print something after someone died -> solving data race

    if philo can write but NOT read then> monitor bust be the one reading and acting accordingly
        > ex if a philo's death time is over don't let anyone write, free all and shut down
        > if all init - start sim
            > 1 "start" time
            > check against ths at first for death then if someone in mean (they are eating/ate) compare with that
    if write mutex locked - let them print 
        > idkkk
        > this is where the "if time to tprint have time to thing comes in"
    return everything to main - shut down from there?
    someone said "lock printf"
        > only monitor can because threads can't talk though...
        > hold the write (stdout)
            > if someone dies keep holding so nobody else can
        > check if alive before every action
*/
/*void *monitor_routine(void *arg)
{
    t_monitor *monitor = (t_monitor *)arg;
    int all_done;

    while (1)
    {
        all_done = 1;
        long now = get_current_time_ms();

        for (int i = 0; i < monitor->num_of_phil; i++)
        {
            t_philo *p = &monitor->philos[i];

            // Check death safely
            if (now - p->last_meal_time_ms > p->time_to_die)
            {
                pthread_mutex_lock(&monitor->print_mutex);
                printf("%ld %d died\n", now - monitor->start_time_ms, p->id);
                pthread_mutex_unlock(&monitor->print_mutex);

                monitor->someone_died = 1;
                return NULL;
            }

            // Print status if ready
            if (p->has_new_status && !monitor->someone_died)
            {
                pthread_mutex_lock(&monitor->print_mutex);
                printf("%ld %d %s\n", now - monitor->start_time_ms, p->id, p->status);
                pthread_mutex_unlock(&monitor->print_mutex);
                p->has_new_status = 0;
            }

            // Check if philosopher finished minimum meals
            if (p->minimum_meals == 0 || p->meals_eaten < p->minimum_meals)
                all_done = 0;
        }

        if (all_done)
        {
            monitor->someone_died = 1;
            return NULL;
        }

        usleep(1000); // sleep 1 ms to reduce CPU usage
    }
}*/

/*void *monitor_routine(void *arg)
{
    t_monitor *monitor = (t_monitor *)arg;

    while (!monitor->someone_died)
    {
        int all_done = 1;
        long now = get_current_time_ms();

        for (int i = 0; i < monitor->num_of_phil; i++)
        {
            t_philo *p = &monitor->philos[i];

            // Check death
            if (now - p->last_meal_time_ms > p->time_to_die)
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
                switch (p->status)
                {
                    case 1: printf("%ld %d is eating\n", now - monitor->start_time_ms, p->id); break;
                    case 2: printf("%ld %d is sleeping\n", now - monitor->start_time_ms, p->id); break;
                    case 3: printf("%ld %d is thinking\n", now - monitor->start_time_ms, p->id); break;
                    default: break;
                }
                pthread_mutex_unlock(&monitor->print_mutex);
                p->has_new_status = 0;
            }

            // Check if all philosophers finished
            if (p->minimum_meals == 0 || p->meals_eaten < p->minimum_meals)
                all_done = 0;
        }

        if (all_done)
        {
            monitor->someone_died = 1;
            return NULL;
        }

        usleep(1000); // 1 ms polling
    }

    return NULL;
}*/

/* working well void *monitor_routine(void *arg)
{
    t_monitor *monitor = (t_monitor *)arg;

    while (!monitor->someone_died)
    {
        int all_done = 1;
        long now = get_current_time_ms();

        for (int i = 0; i < monitor->num_of_phil; i++)
        {
            t_philo *p = &monitor->philos[i];

            // Check death
            if (now - p->last_meal_time_ms > p->time_to_die)
            {
                pthread_mutex_lock(&monitor->print_mutex);
                printf("%ld %d died\n", timestamp_ms(monitor->start_time_ms), p->id);
                pthread_mutex_unlock(&monitor->print_mutex);
                monitor->someone_died = 1;
                return NULL;
            }

            // Print status if updated
            if (p->has_new_status)
            {
                pthread_mutex_lock(&monitor->print_mutex);
                switch (p->status)
                {
                    case 1: printf("%ld %d is eating\n", timestamp_ms(monitor->start_time_ms), p->id); break;
                    case 2: printf("%ld %d is sleeping\n", timestamp_ms(monitor->start_time_ms), p->id); break;
                    case 3: printf("%ld %d is thinking\n", timestamp_ms(monitor->start_time_ms), p->id); break;
                    default: break;
                }
                pthread_mutex_unlock(&monitor->print_mutex);
                p->has_new_status = 0;
            }

            if (p->minimum_meals == 0 || p->meals_eaten < p->minimum_meals)
                all_done = 0;
        }

        if (all_done)
        {
            monitor->someone_died = 1;
            return NULL;
        }

        usleep(1000);
    }

    return NULL;
}*/

//newest
/*void *monitor_routine(void *arg)
{
    t_monitor *monitor = (t_monitor *)arg;

    while (!monitor->someone_died)
    {
        int all_done = 1;
        long now = get_current_time_ms();

        for (int i = 0; i < monitor->num_of_phil; i++)
        {
            t_philo *p = &monitor->philos[i];

            // Check death
            if (now - p->last_meal_time_ms > p->time_to_die)
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

                switch (p->status)
                {
                    case 1: printf("%ld %d is eating\n", now - monitor->start_time_ms, p->id); break;
                    case 2: printf("%ld %d is sleeping\n", now - monitor->start_time_ms, p->id); break;
                    case 3: printf("%ld %d is thinking\n", now - monitor->start_time_ms, p->id); break;
                    default: break;
                }

                pthread_mutex_unlock(&monitor->print_mutex);
                p->has_new_status = 0; // mark as printed
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

        usleep(1000); // small sleep to avoid busy waiting
    }

    return NULL;
}*/

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

            // Check death
            if (now - last_meal > p->time_to_die)
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

        usleep(1000); // small sleep to avoid busy waiting
    }

    return NULL;
}