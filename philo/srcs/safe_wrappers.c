#include "philo.h"

static void	report_mutex_error(int status, t_opcode opcode);

void	mutex_op(t_mutex *mutex, t_opcode opcode)
{
	if (opcode == LOCK)
		report_mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (opcode == UNLOCK)
		report_mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (opcode == INIT)
		report_mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (opcode == DESTROY)
		report_mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
		error_exit("error: invalid mutex opcode");
}

static void	report_mutex_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL && (opcode == LOCK || opcode == UNLOCK))
		error_exit("error: mutex is invalid");
	else if (status == EINVAL && opcode == INIT)
		error_exit("error: mutex attributes invalid");
	else if (status == EDEADLK)
		error_exit("error: mutex deadlock detected");
	else if (status == EPERM)
		error_exit("error: mutex not owned by current thread");
	else if (status == ENOMEM)
		error_exit("error: insufficient memory for mutex");
	else if (status == EBUSY)
		error_exit("error: mutex is already locked");
}

static void	report_thread_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	else if (status == EAGAIN)
		error_exit("error: insufficient resources for thread");
	else if (status == EPERM)
		error_exit("error: thread operation not permitted");
	else if (status == EINVAL && opcode == CREATE)
		error_exit("error: invalid thread attributes");
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		error_exit("error: invalid thread identifier");
	else
		error_exit("error: thread operation failed");
}

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		error_exit("error: malloc failed");
	return (ret);
}

void	thread_op(pthread_t *thread, void *(*start_routine)(void *),
		void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		report_thread_error(pthread_create(thread, NULL,
				start_routine, data), opcode);
	else if (opcode == JOIN)
		report_thread_error(pthread_join(*thread, NULL), opcode);
	else if (opcode == DETACH)
		report_thread_error(pthread_detach(*thread), opcode);
	else
		error_exit("error: invalid thread opcode (use CREATE, JOIN, DETACH)");
}
