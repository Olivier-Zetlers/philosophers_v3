/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 12:26:33 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/20 12:28:38 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_table *table)
{
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
		;
}

bool	all_threads_running(t_mutex *mutex, long *thread_count,
		long philosopher_count)
{
	bool	ret;

	ret = false;
	safe_mutex_handle(mutex, LOCK);
	if (*thread_count == philosopher_count)
		ret = true;
	safe_mutex_handle(mutex, UNLOCK);
	return (ret);
}

void	increase_long(t_mutex *mutex, long *value)
{
	safe_mutex_handle(mutex, LOCK);
	(*value)++;
	safe_mutex_handle(mutex, UNLOCK);
}
