/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:06:39 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/20 11:12:26 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static const char	*validate_input(const char *str)
{
	const char	*start;
	size_t		len;

	len = 0;
	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		error_exit("Give me only positive values");
	if (!is_digit(*str))
		error_exit("Not valid input: expected digit");
	start = str;
	while (is_digit(*str))
	{
		len++;
		str++;
	}
	if (len > 10)
		error_exit("The value is too big. INT_MAX is the limit");
	while (is_space(*str))
		str++;
	if (*str != '\0')
		error_exit("Invalid characters after number");
	return (start);
}

static long	ft_atoi(const char *str)
{
	long	num;
	int		digit;

	num = 0;
	str = validate_input(str);
	while (is_digit(*str))
	{
		digit = *str - '0';
		if (num > (INT_MAX - digit) / 10)
			error_exit("The value is too big, the limit is INT_MAX");
		num = num * 10 + digit;
		str++;
	}
	return (num);
}

void	parse_input(t_table *table, char **av)
{
	long	min_time_us;

	min_time_us = 60000;
	table->philosopher_count = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]) * 1000;
	table->time_to_eat = ft_atoi(av[3]) * 1000;
	table->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (table->time_to_die < min_time_us || table->time_to_eat < min_time_us
		|| table->time_to_sleep < min_time_us)
		error_exit("Use timestamps greater than 60ms");
	if (av[5])
		table->meal_limit = ft_atoi(av[5]);
	else
		table->meal_limit = -1;
}
