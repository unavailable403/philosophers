/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ergrigor < ergrigor@student.42yerevan.am > +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 18:57:02 by smikayel          #+#    #+#             */
/*   Updated: 2022/09/22 22:16:17 by ergrigor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	rfork(t_settings *rules, int index)
{
	if (index + 1 >= rules->philo)
		return (0);
	return (index + 1);
}

int	mutex_maker(t_settings *rules, pthread_mutex_t **mutexes,
	t_philo **philosophers, pthread_mutex_t **print)
{
	*mutexes = malloc(sizeof(pthread_mutex_t) * rules->philo);
	*print = malloc(sizeof(pthread_mutex_t));
	*philosophers = malloc(sizeof(t_philo) * rules->philo);
	if (!*mutexes || !*philosophers || !*print)
	{
		put_msg("Malloc Error", 2, rules);
		return (-1);
	}
	return (0);
}

long long int	current_timestamp(void)
{
	struct timeval	te;

	gettimeofday (&te, 0);
	return (te.tv_sec * 1000 + te.tv_usec / 1000);
}

void	print_action(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->rules->print);
	printf("%lld %d %s", current_timestamp() - philo->rules->start_time,
		philo->index + 1, str);
	pthread_mutex_unlock(philo->rules->print);
}

void	destroy_mutexs(pthread_mutex_t *mutex, int i)
{
	int	l;

	l = 0;
	while (i >= l)
	{
		pthread_mutex_destroy(&mutex[l]);
		l++;
	}
}
