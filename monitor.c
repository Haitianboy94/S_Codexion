/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:14 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 10:12:43 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// static int  all_done(const t_sim *sim);
// void   *monitor_routine(void *arg)
// {
// 	// Thread entry point (cast arg → t_sim *).
//     // Polls every ~1 ms; for each coder checks:
//     //   now_ms() - last_compile_start_ms > time_to_burnout
//     // If burnout detected: log "X burned out", set stop_flag,
//     //   pthread_cond_broadcast all dongle cvars to unblock waiters.
//     t_sim *new_arg;
//     int i;

//     i = 0;
//     new_arg = (t_sim*)arg;
//     while (new_arg->stop_flag == 0)
//     {
//         sleep_ms(1);
//         pthread_mutex_lock(&new_arg->state_mutex);
//         i = 0;
//         while (i < new_arg->args.nb_coders)
//         {
//             if (elapsed_ms(new_arg->coders[i].last_compile_start_ms) > new_arg->args.time_to_burnout)
//             {
//                 log_event(new_arg, new_arg->coders[i].id, BURNED "burned out 🔥" RESET);
//                 new_arg->coders[i].state = BURNED_OUT;
//                 new_arg->stop_flag = 1;
//             }
//             i++;
//         }
//         if (new_arg->args.nb_compiles_required > 0 && all_done(new_arg))
//         {
//             new_arg->stop_flag = 1;
//             pthread_mutex_unlock(&new_arg->state_mutex);
//             break;
//         }
//         pthread_mutex_unlock(&new_arg->state_mutex);
//         if (new_arg->stop_flag == 1)
//         {
//             i = 0;
//             while (i < new_arg->args.nb_coders)
//             {
//                 pthread_cond_broadcast(&new_arg->dongles[i].cond);
//                 i++;
//             }
//         }
//     }
//     return(NULL);
// }

// static int  all_done(const t_sim *sim)
// {
// 	// Returns 1 if every coder->compile_count >= nb_compiles_required
//     int i;

//     i = 0;
//     while (i < sim->args.nb_coders)
//     {
//         if (sim->coders[i].compile_count < sim->args.nb_compiles_required)
//             return(0);
//         i++;
//     }
//     return(1);
// }
static int	all_done(const t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.nb_coders)
	{
		if (sim->coders[i].compile_count
			< sim->args.nb_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	int		j;

	sim = (t_sim *)arg;
	while (1)
	{
		sleep_ms(1); // fast polling (<10ms guarantee)

		pthread_mutex_lock(&sim->state_mutex);

		if (sim->stop_flag)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			return (NULL);
		}

		i = 0;
		while (i < sim->args.nb_coders)
		{
			if (elapsed_ms(sim->coders[i].last_compile_start_ms)
				> sim->args.time_to_burnout)
			{
				log_event(sim, sim->coders[i].id, "burned out");
				sim->coders[i].state = BURNED_OUT;
				sim->stop_flag = 1;

				pthread_mutex_unlock(&sim->state_mutex);

				j = 0;
				while (j < sim->args.nb_coders)
				{
					pthread_cond_broadcast(
						&sim->dongles[j].cond);
					j++;
				}
				return (NULL);
			}
			i++;
		}

		if (sim->args.nb_compiles_required > 0
			&& all_done(sim))
		{
			sim->stop_flag = 1;
			pthread_mutex_unlock(&sim->state_mutex);

			j = 0;
			while (j < sim->args.nb_coders)
			{
				pthread_cond_broadcast(
					&sim->dongles[j].cond);
				j++;
			}
			return (NULL);
		}

		pthread_mutex_unlock(&sim->state_mutex);
	}
}