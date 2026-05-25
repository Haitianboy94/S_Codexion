/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruthler <ruthler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:23:33 by ruthler           #+#    #+#             */
/*   Updated: 2026/05/25 14:26:08 by ruthler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	launch_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread,
				NULL, coder_routine, &sim->coders[i]) != 0)
			return (sim->stop_flag = 1, -1);
		i++;
	}
	return (0);
}

static void	join_all(t_sim *sim)
{
	int	i;

	i = sim->args.nb_coders - 1;
	while (i >= 0)
		pthread_join(sim->coders[i--].thread, NULL);
	pthread_join(sim->monitor, NULL);
}

static int	any_burned_out(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.nb_coders)
		if (sim->coders[i++].state == BURNED_OUT)
			return (1);
	return (0);
}

int	sim_run(t_sim *sim)
{
	if (pthread_create(&sim->monitor,
        NULL, monitor_routine, sim) != 0)
		return (-1);
	launch_coders(sim);
	join_all(sim);
	return (any_burned_out(sim) ? -1 : 0);
}

void    sim_destroy(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim -> args.nb_coders)
    {
        dongle_destroy(&sim -> dongles[i]);
        i++;
    }
    free(sim -> dongles);
    free(sim -> coders);
    pthread_mutex_destroy(&sim->log_mutex);
    pthread_mutex_destroy(&sim->state_mutex);
}