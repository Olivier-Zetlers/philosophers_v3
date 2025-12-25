/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 11:16:44 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/22 11:17:03 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	think(t_philo *philo, bool pre_simulation)
{
	long	time_to_eat;
	long	time_to_sleep;
	long	think_duration;

	if (!pre_simulation)
		write_status(THINKING, philo);
	if (philo->table->philosopher_count % 2 == 0)
		return ;
	time_to_eat = philo->table->time_to_eat;
	time_to_sleep = philo->table->time_to_sleep;
	think_duration = time_to_eat * 2 - time_to_sleep;
	if (think_duration < 0)
		think_duration = 0;
	precise_usleep(think_duration * 0.42, philo->table);
}

void	eat(t_philo *philo)
{
	safe_mutex_handle(&philo->first_fork->mutex, LOCK);
	write_status(TAKE_FIRST_FORK, philo);
	safe_mutex_handle(&philo->second_fork->mutex, LOCK);
	write_status(TAKE_SECOND_FORK, philo);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	safe_mutex_handle(&philo->philo_mutex, LOCK);
	philo->meal_count++;
	safe_mutex_handle(&philo->philo_mutex, UNLOCK);
	write_status(EATING, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meal_limit > 0
		&& philo->meal_count == philo->table->meal_limit)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_mutex_handle(&philo->first_fork->mutex, UNLOCK);
	safe_mutex_handle(&philo->second_fork->mutex, UNLOCK);
}
