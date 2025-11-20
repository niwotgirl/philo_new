/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_timing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:33:26 by aabelkis          #+#    #+#             */
/*   Updated: 2025/10/20 10:41:06 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


///idk why i wanted this
/*
              ┌──────────────────────────┐
              │         main()           │
              └────────────┬─────────────┘
                           │
        ┌──────────────────┼────────────────────┐
        │                  │                    │
        ▼                  ▼                    ▼
  [Parse Input]       [Init Structs]       [Create Threads]
(ft_error, atoi)   (malloc t_philo, forks)   (pthread_create)
        │                  │                    │
        ▼                  ▼                    ▼
     Validate        Build philosopher        Each thread runs
  command-line args   structs, link forks        routine()
        │                  │                    │
        └──────────────┬───┴────────────────────┘
                       ▼
              [Monitor Thread or Loop]
                 (checks if any die)
                       │
                       ▼
                 [Clean Exit / Free]
          (destroy mutexes, free memory)
*/