/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruthler <ruthler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:21 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/25 14:29:33 by ruthler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_dongles(t_dongle *dongles, int count)
{
	while (count-- > 0)
		dongle_destroy(&dongles[count]);
	free(dongles);
}

static int	init_dongles(t_sim *sim, const t_args *args)
{
	int	i;

	i = 0;
	while (i < args->nb_coders)
	{
		if (dongle_init(&sim->dongles[i], i) != 0)
		{
			cleanup_dongles(sim->dongles, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	init_coders(t_sim *sim, const t_args *args)
{
	int	j;

	j = 0;
	while (j < args->nb_coders)
	{
		sim->coders[j].id = j + 1;
		sim->coders[j].state = THINKING;
		sim->coders[j].compile_count = 0;
		sim->coders[j].last_compile_start_ms = now_ms();
		sim->coders[j].sim = sim;
		sim->coders[j].thread = 0;
		j++;
	}
}

static int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		return (-1);
	}
	return (0);
}

int	sim_init(t_sim *sim, const t_args *args)
{
	sim->args = *args;
	sim->coders = malloc(sizeof(t_coder) * args->nb_coders);
	sim->dongles = malloc(sizeof(t_dongle) * args->nb_coders);
	if (!sim->coders || !sim->dongles)
		return (free(sim->coders), free(sim->dongles), -1);
	if (init_dongles(sim, args) != 0)
		return (free(sim->coders), free(sim->dongles), -1);
	init_coders(sim, args);
	sim->stop_flag = 0;
	sim->start_ms = now_ms();
	if (init_mutexes(sim) != 0)
		return (cleanup_dongles(sim->dongles, args->nb_coders),
			free(sim->coders), -1);
	return (0);
}
