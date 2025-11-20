/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelkis <aabelkis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:59:10 by aabelkis          #+#    #+#             */
/*   Updated: 2025/11/20 18:08:45 by aabelkis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_error_atoi(const char *nptr)
{
	int		i;
	long	result;
	int		has_digit;

	i = 0;
	result = 0;
	has_digit = 0;
	// skip leading whitespace
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	// handle optional '+' sign
	if (nptr[i] == '+')
		i++;
	// reject negatives
	if (nptr[i] == '-')
		return (-1);
	// parse digits
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		has_digit = 1;
		result = result * 10 + (nptr[i] - '0');
		if (result > INT_MAX)
			return (-1);
		i++;
	}
	// reject invalid or empty input
	if (!has_digit || nptr[i] != '\0')
		return (-1);
	return ((int)result);
}
