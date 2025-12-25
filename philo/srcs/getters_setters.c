#include "philo.h"

bool	get_bool(t_mutex *mutex, bool *value)
{
	bool	ret;

	mutex_op(mutex, LOCK);
	ret = *value;
	mutex_op(mutex, UNLOCK);
	return (ret);
}

long	get_long(t_mutex *mutex, long *value)
{
	long	ret;

	mutex_op(mutex, LOCK);
	ret = *value;
	mutex_op(mutex, UNLOCK);
	return (ret);
}

void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	mutex_op(mutex, LOCK);
	*dest = value;
	mutex_op(mutex, UNLOCK);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	mutex_op(mutex, LOCK);
	*dest = value;
	mutex_op(mutex, UNLOCK);
}
