/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smikayel <smikayel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 21:04:08 by ergrigor          #+#    #+#             */
/*   Updated: 2022/09/17 16:58:59 by smikayel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	rfork(t_settings *rules, int index)
{
	if (index + 1 >= rules->philo)
		return (0);
	return (index + 1);
}

void	mutex_maker(t_settings *rules, pthread_mutex_t **mutexes)
{
	int				i;

	i = 0;
	while (i < rules->philo)
	{
		mutexes[i] = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(mutexes[i], NULL) < 0)
			put_msg("Mutex Error", 2, rules);
		i++;
	}
}

long long int    current_timestamp(void)
{
    struct timeval    te;

    gettimeofday(&te, 0);
    return (te.tv_sec * 1000LL + te.tv_usec / 1000);
}


void	print_action(char *str, t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->print);
	printf("%lld %d %s\n",current_timestamp(), philo->index + 1, str);
	pthread_mutex_unlock(&philo->rules->print);
}

void	get_philo_forks(t_philo	*philo)
{
	if (philo->index + 1 == philo->rules->philo)
	{
		pthread_mutex_lock(philo->lfork);
		print_action("has taken a fork\n", philo);
		pthread_mutex_lock(philo->rfork);
		print_action("has taken a fork\n", philo);
	}
	else
	{
		pthread_mutex_lock(philo->rfork);
		print_action("has taken a fork\n", philo);
		pthread_mutex_lock(philo->lfork);
		print_action("has taken a fork\n", philo);
	}
}

void eat(t_philo	*philo)
{
	print_action(" is eating\n", philo);
	philo->eat_count += 1;
	usleep(philo->rules->eat_time);
}

void	put_philo_forks(t_philo	*philo)
{
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
}

void	sleeping(t_philo	*philo)
{
	print_action(" is sleeping\n", philo);
	philo->mode = 3;
	usleep(philo->rules->sleep_time);
}

void	thinking(t_philo	*philo)
{
	print_action(" is thinking\n", philo);
	philo->mode = 1;
}

void	*life(void *gago)
{
	t_philo	*philo;

	philo = (t_philo *)gago;
	if (philo->index % 2 == 0)
		usleep(10);
	while (1)
	{
		if (philo->mode == 1)
		{
			get_philo_forks(philo);
			eat(philo);
			put_philo_forks(philo);
		}
		if (philo->mode == 2)
			sleeping(philo);
		if (philo->mode == 3)
			thinking(philo);
	}
	return 0;
}

t_philo	*create_philo(t_settings *rules, pthread_mutex_t **mutexes, int i, pthread_t *thread)
{
	t_philo	*philo;
	pthread_mutex_t *mutex;

	mutex = *mutexes;
	philo = malloc(sizeof(t_philo));
	if (!philo)
		put_msg("Philo create error", 2, rules);
	philo->index = i;
	philo->mode = 1;
	philo->lfork = &mutex[i];
	philo->rfork = &mutex[rfork(rules, i)];
	philo->rules = rules;
	i = 0;
	if (pthread_create(&thread[i], NULL, life, (void *)philo) < 0)
		put_msg("Philo create error", 2, rules);
	return (philo);
}

void	destroy_mutexs(pthread_mutex_t **mutex, int i)
{

	int l = 0;
	pthread_mutex_t	*curr;

	curr = *mutex;
	while (i >= l)
	{
		pthread_mutex_destroy(&curr[l]);
		l++;
	}
}

int	check_if_all_eat(t_philo	**philosophers)
{
	int i;
	int count_philo;

	i = 0;
	count_philo = philosophers[0]->rules->philo;
	while (i < count_philo)
	{
		if (philosophers[0]->eat_count <  philosophers[0]->rules->eat_max_count)
			return (-1);
	}
	return (1);
}

void detach_threads(pthread_t	*thread, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		pthread_detach(thread[i]);
		i++;
	}
}

void	start(t_settings	*rules)
{
	t_philo			**philosophers;
	pthread_mutex_t	**mutexes;
	pthread_t		*thread;
	int				i;

	i = 0;
	philosophers = malloc(sizeof(t_philo *) * rules->philo);
	thread = malloc(rules->philo * sizeof(pthread_t));
	mutexes = malloc(rules->philo * sizeof(pthread_mutex_t *));
	if (!mutexes)
		put_msg("Malloc Error", 2, rules);
	mutex_maker(rules, mutexes);
	if (!mutexes)
		return ;
	while (i < rules->philo)
	{
		philosophers[i] = create_philo(rules, mutexes, i, thread);
		i++;
	}
	while (1)
	{ 
		usleep(5);
		if (check_if_all_eat(philosophers) == 1)
		{
			detach_threads(thread, rules->philo);
			exit(0);
		}
	}
	destroy_mutexs(mutexes, i - 1);
}
