#include "philo.h"

void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	mutex_op(mutex, LOCK);
	*dest = value;
	mutex_op(mutex, UNLOCK);
}

bool	get_bool(t_mutex *mutex, bool *value)
{
	bool	ret;

	mutex_op(mutex, LOCK);
	ret = *value;
	mutex_op(mutex, UNLOCK);
	return (ret);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	mutex_op(mutex, LOCK);
	*dest = value;
	mutex_op(mutex, UNLOCK);
}

long	get_long(t_mutex *mutex, long *value)
{
	long	ret;

	mutex_op(mutex, LOCK);
	ret = *value;
	mutex_op(mutex, UNLOCK);
	return (ret);
}
