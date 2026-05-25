/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:14 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 00:00:00 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_done(const t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.nb_coders)
	{
		if (sim->coders[i].compile_count < sim->args.nb_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static void	broadcast_all(t_sim *sim)
{
	int	j;

	j = 0;
	while (j < sim->args.nb_coders)
	{
		pthread_cond_broadcast(&sim->dongles[j].cond);
		j++;
	}
}

static int	check_burnout(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.nb_coders)
	{
		if (sim->coders[i].state != COMPILING
			&& elapsed_ms(sim->coders[i].last_compile_start_ms)
			> sim->args.time_to_burnout)
		{
			sim->coders[i].state = BURNED_OUT;
			sim->stop_flag = 1;
			log_event(sim, sim->coders[i].id, "burned out");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_all_done(t_sim *sim)
{
	return (sim->args.nb_compiles_required > 0 && all_done(sim));
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (1)
	{
		sleep_ms(1);
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->stop_flag || check_burnout(sim) || check_all_done(sim))
		{
			sim->stop_flag = 1;
			pthread_mutex_unlock(&sim->state_mutex);
			broadcast_all(sim);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->state_mutex);
	}
	return (NULL);
}
