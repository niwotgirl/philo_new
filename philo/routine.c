/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:02:14 by aabelkis          #+#    #+#             */
/*   Updated: 2025/10/20 21:39:31 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


/*void *routine(philo_st)
{
	static int ate;
	ate = 0;
    if (philo_st->id % 2)
	{
        pthread_mutex_lock(&philo_st.left_fork_mutex);
        pthread_mutex_lock(&philo_st.right_fork_mutex);
	}
    else
	{
		pthread_mutex_lock(&philo_st.right_fork_mutex);
        pthread_mutex_lock(&philo_st.left_fork_mutex);		
	}
	philo_st.last_meal_time_ms = get_current_time_ms; //this is what I will base my death determination off of (once it's set)
	set booleen to yes // may or may not need - this is bassically the "fork"
	ate = 1; // set back to 0??
	print_routine(ft_itoa(philo_st.last_meal_time_ms) philo_st.id is eating);
	usleep(philo_st.time_to_eat);
    if (philo_st.id % 2)
	{
		pthread_mutex_unlock(philo_st.right_fork_mutex);
        pthread_mutex_unlock(philo_st.left_fork_mutex);
	}
	else 
	{
        pthread_mutex_unlock(philo_st.left_fork_mutex);
		pthread_mutex_unlock(philo_st.right_fork_mutex);
	}
	philo_st.meals_eaten += 1;
	print_routine(ft_itoa(get_current_time_ms) philosopher philo_st.id is sleeping); // double check this wording - do i nee "" etc
	usleep(philo_st.time_to_sleep)
	if (ate = 1 && (philo_st.time_to_eat > philo_st.time_to_sleep))//means they have eaten and eating takes more time than sleeping
	{
		print_routine(ft_itoa(get_current_time_ms) philo_st.id is sleeping) //also check
		usleep(philo_st.time_to_eat - philo_st.time_to_sleep);
	}
	else 
		try to eat again; //need to look into how to do this
}
*/
/*void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    // small stagger to avoid immediate deadlock for 2 philosophers
    if (philo->id % 2 == 0)
        usleep(100);

    while (1)
    {
        // Update last meal time BEFORE picking up forks
        philo->last_meal_time_ms = get_current_time_ms();

        // Pick up forks (left is pointer, right is value)
        if (philo->id % 2)
        {
            pthread_mutex_lock(philo->left_fork_mutex);
            pthread_mutex_lock(&philo->right_fork_mutex);
        }
        else
        {
            pthread_mutex_lock(&philo->right_fork_mutex);
            pthread_mutex_lock(philo->left_fork_mutex);
        }

        // Eating
        ft_strcpy(philo->status, "is eating");
        philo->has_new_status = 1;
        philo->meals_eaten++;
        custom_sleep(philo->time_to_eat);

        // Put down forks
        pthread_mutex_unlock(philo->left_fork_mutex);
        pthread_mutex_unlock(&philo->right_fork_mutex);

        // Check if finished minimum meals
        if (philo->minimum_meals > 0 && philo->meals_eaten >= philo->minimum_meals)
            break;

        // Sleeping
        ft_strcpy(philo->status, "is sleeping");
        philo->has_new_status = 1;
        custom_sleep(philo->time_to_sleep);

        // Thinking
        ft_strcpy(philo->status, "is thinking");
        philo->has_new_status = 1;
    }

    return NULL;
}*/
/*void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (!philo->monitor->someone_died)
    {
        // Pick up forks (odd/even ordering to avoid deadlock)
        if (philo->id % 2)
        {
            pthread_mutex_lock(philo->left_fork_mutex);
            pthread_mutex_lock(&philo->right_fork_mutex);
        }
        else
        {
            pthread_mutex_lock(&philo->right_fork_mutex);
            pthread_mutex_lock(philo->left_fork_mutex);
        }

        // Eat
        philo->last_meal_time_ms = get_current_time_ms();
        philo->status = 1; // eating
        philo->has_new_status = 1;
        philo->meals_eaten++;

        custom_sleep(philo->time_to_eat); // break eat time into small naps

        // Put down forks
        pthread_mutex_unlock(philo->left_fork_mutex);
        pthread_mutex_unlock(&philo->right_fork_mutex);

        // Check if finished minimum meals
        if (philo->minimum_meals > 0 && philo->meals_eaten >= philo->minimum_meals)
            break;

        // Sleep
        philo->status = 2; // sleeping
        philo->has_new_status = 1;
        custom_sleep(philo->time_to_sleep);

        // Think
        philo->status = 3; // thinking
        philo->has_new_status = 1;

        // small pause to let monitor print before looping again
        usleep(500);
    }

    return NULL;
}*/

/*void *routine(void *arg)
{
	//make sure update eating at right time
    t_philo *philo = (t_philo *)arg;

    while (!philo->monitor->someone_died)
    {
        // 🥄 Pick up forks (avoid deadlock with odd/even ID)
        if (philo->id % 2)
        {
            pthread_mutex_lock(philo->left_fork_mutex);
            pthread_mutex_lock(&philo->right_fork_mutex);
        }
        else
        {
            pthread_mutex_lock(&philo->right_fork_mutex);
            pthread_mutex_lock(philo->left_fork_mutex);
        }

        // 🍽 Start eating
        philo->last_meal_time_ms = get_current_time_ms();
        philo->status = 1; // eating
        philo->has_new_status = 1;
        philo->meals_eaten++;

        custom_sleep(philo->time_to_eat);

        // 🥄 Release forks
        pthread_mutex_unlock(philo->left_fork_mutex);
        pthread_mutex_unlock(&philo->right_fork_mutex);

        // Check if finished minimum meals
        if (philo->minimum_meals > 0 && philo->meals_eaten >= philo->minimum_meals)
            break;

        // 💤 Sleep
        philo->status = 2; // sleeping
        philo->has_new_status = 1;
        custom_sleep(philo->time_to_sleep);

        // 💭 Think
        philo->status = 3; // thinking
        philo->has_new_status = 1;
    }

    return NULL;
}*/

/*void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (!philo->monitor->someone_died)
    {
        if (philo->id % 2)
        {
            pthread_mutex_lock(philo->left_fork_mutex);
            pthread_mutex_lock(&philo->right_fork_mutex);
        }
        else
        {
            pthread_mutex_lock(&philo->right_fork_mutex);
            pthread_mutex_lock(philo->left_fork_mutex);
        }

        // Start eating
        philo->last_meal_time_ms = get_current_time_ms();
        philo->status = 1;
        philo->has_new_status = 1;
        philo->meals_eaten++;

        custom_sleep(philo->time_to_eat);

        pthread_mutex_unlock(philo->left_fork_mutex);
        pthread_mutex_unlock(&philo->right_fork_mutex);

        if (philo->minimum_meals > 0 && philo->meals_eaten >= philo->minimum_meals)
            break;

        philo->status = 2;
        philo->has_new_status = 1;
        custom_sleep(philo->time_to_sleep);

        philo->status = 3;
        philo->has_new_status = 1;
    }

    return NULL;
}*/ //working well

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (!philo->monitor->someone_died)
    {
        // 🥄 Pick up forks (avoid deadlock with odd/even ID)
        if (philo->id % 2)
        {
            pthread_mutex_lock(philo->left_fork_mutex);
            pthread_mutex_lock(&philo->right_fork_mutex);
        }
        else
        {
            pthread_mutex_lock(&philo->right_fork_mutex);
            pthread_mutex_lock(philo->left_fork_mutex);
        }

        // 🍽 Eat
        philo->last_meal_time_ms = get_current_time_ms();
        philo->status = 1; // eating
        philo->has_new_status = 1;
        philo->meals_eaten++;
        custom_sleep(philo->time_to_eat);

        // 🥄 Release forks
        pthread_mutex_unlock(philo->left_fork_mutex);
        pthread_mutex_unlock(&philo->right_fork_mutex);

        // Check if finished minimum meals
        if (philo->minimum_meals > 0 && philo->meals_eaten >= philo->minimum_meals)
            break;

        // 💤 Sleep
        philo->status = 2; // sleeping
        philo->has_new_status = 1;
        custom_sleep(philo->time_to_sleep);

        // 💭 Think only if they can survive until next meal
        long now = get_current_time_ms();
        if (now + 1 < philo->last_meal_time_ms + philo->time_to_die)
        {
            philo->status = 3; // thinking
            philo->has_new_status = 1;
            // no sleep, just log status
        }
    }

    return NULL;
}
