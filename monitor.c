/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:14 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 11:42:23 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void   *monitor_routine(void *arg)
{
	// Thread entry point (cast arg → t_sim *).
    // Polls every ~1 ms; for each coder checks:
    //   now_ms() - last_compile_start_ms > time_to_burnout
    // If burnout detected: log "X burned out", set stop_flag,
    //   pthread_cond_broadcast all dongle cvars to unblock waiters.
    t_sim *new_arg;
    int i;

    i = 0;
    new_arg = (t_sim*)arg;
    while (new_arg->stop_flag == 0)
    {
        sleep_ms(1);
        pthread_mutex_lock(&new_arg->state_mutex);
        i = 0;
        while (i < new_arg->args.nb_coders)
        {
            if (elapsed_ms(new_arg->coders[i].last_compile_start_ms) > new_arg->args.time_to_burnout)
            {
                log_event(new_arg, new_arg->coders[i].id, "burned out");
                new_arg->coders[i].state = BURNED_OUT;
                new_arg->stop_flag = 1;
            }
            i++;
        }
        if (new_arg->args.nb_compiles_required > 0 && all_done(new_arg))
        {
            new_arg->stop_flag = 1;
            pthread_mutex_unlock(&new_arg->state_mutex);
            break;
        }
        pthread_mutex_unlock(&new_arg->state_mutex);
        if (new_arg->stop_flag == 1)
        {
            i = 0;
            while (i < new_arg->args.nb_coders)
            {
                pthread_cond_broadcast(&new_arg->dongles[i].cond);
                i++;
            }
        }
    }
    
    return(NULL);
}

static int  all_done(const t_sim *sim)
{
	// Returns 1 if every coder->compile_count >= nb_compiles_required
    int i;

    i = 0;
    while (i < sim->args.nb_coders)
    {
        if (sim->coders[i].compile_count < sim->args.nb_compiles_required)
            return(0);
        i++;
    }
    return(1);
}