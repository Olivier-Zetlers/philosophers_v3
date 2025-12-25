/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 12:17:56 by weiyang           #+#    #+#             */
/*   Updated: 2025/12/25 00:00:00 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status(t_philo_status status, t_philo *philo)
{
	long	elapsed;

	if (philo->full)
		return ;
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	elapsed = gettime(MILLISECOND) - philo->table->start_simulation;
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
		&& !simulation_finished(philo->table))
		printf("%ld %d has taken a fork\n", elapsed, philo->id + 1);
	if ((status == EATING) && !simulation_finished(philo->table))
		printf("%ld %d is eating\n", elapsed, philo->id + 1);
	if ((status == SLEEPING) && !simulation_finished(philo->table))
		printf("%ld %d is sleeping\n", elapsed, philo->id + 1);
	if ((status == THINKING) && !simulation_finished(philo->table))
		printf("%ld %d is thinking\n", elapsed, philo->id + 1);
	if ((status == DIED) && !simulation_finished(philo->table))
		printf("%ld %d died\n", elapsed, philo->id + 1);
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}
