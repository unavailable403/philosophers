/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ergrigor < ergrigor@student.42yerevan.am > +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 18:58:58 by smikayel          #+#    #+#             */
/*   Updated: 2022/09/22 22:42:08 by ergrigor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_if_all_eat(t_philo	*philosophers)
{
	int	i;
	int	count_philo;

	i = 0;
	count_philo = philosophers[0].rules->philo;
	if (philosophers[0].rules->eat_max_count == -1)
		return (-1);
	while (i < count_philo)
	{
		if (philosophers[i].eat_count < philosophers[i].rules->eat_max_count)
			return (-1);
		else
			i++;
	}
	return (1);
}

int	check_last_eat_time(t_philo	*philosophers)
{
	int			i;
	long long	time;

	i = 0;
	while (i < philosophers[0].rules->philo)
	{
		time = current_timestamp();
		if ((time - philosophers[i].last_eat_time)
			>= philosophers[0].rules->death_time)
			return (i);
		i++;
	}
	return (-1);
}
