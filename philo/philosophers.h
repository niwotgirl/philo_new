/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:01:59 by aabelkis          #+#    #+#             */
/*   Updated: 2025/11/20 15:16:20 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <stddef.h>
#include <limits.h>


typedef struct s_philo
{
    pthread_mutex_t *left_fork_mutex;   // set in main
    pthread_mutex_t right_fork_mutex;  // set in main
    pthread_t       thread;             // philosopher thread
    int             id;                 // philosopher ID
    int             time_to_die;        // constants
    int             time_to_eat;
    int             time_to_sleep;
    int             minimum_meals;
    long            last_meal_time_ms;  // updated in routine
    int             meals_eaten;        // updated in routine
    int             status;             // 1 = eating, 2 = sleeping, 3 = thinking
    int             has_new_status;     // 0 = nothing new, 1 = ready to print
    struct s_monitor *monitor;          // pointer to monitor for shared data
} t_philo;


typedef struct s_monitor
{
    t_philo    *philos;          // array of philosophers
    int         num_of_phil;     // total number of philosophers
    int         someone_died;    // shared flag (0 = no, 1 = yes)
    long        start_time_ms;   // simulation start time in ms
    pthread_mutex_t print_mutex; // mutex to protect printing
    pthread_t   thread;          // monitor thread
} t_monitor;

// (status strings are in sources as literals; avoid defining static data in header)

int	ft_error_atoi(const char *nptr);
int ft_error(int argc, char **argv, int i);
char *ft_strcpy(char *dest, const char *src);
char	*ft_strdup(const char *s);
char	*ft_itoa(int n);
void *monitor_routine(void *arg);
void print_routine(t_philo *philo, t_monitor *monitor);
void *routine(void *arg);
void custom_sleep(int val);
void struct_init(int val, int j, int i, t_philo *philo);
long timestamp_ms(long start_time_ms);
long get_current_time_ms(void);

#endif
