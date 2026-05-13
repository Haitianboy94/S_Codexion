/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:21 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/12 17:15:39 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int sim_init(t_sim *sim, const t_args *args)
{
	// Allocates coders[] and dongles[], inits mutexes,
    // seeds last_compile_start_ms = now_ms() for all coders.
    int i;
    int j;
    t_coder *coders;
    t_dongle *dongles;

    i = 0;
    j = 0;
    sim -> args = *args;
    coders = malloc(sizeof(t_coder) * args -> nb_coders);
    if (!coders)
        return (-1);
    sim -> coders = coders;
    dongles = malloc(sizeof(t_dongle) * args -> nb_coders);
    if (!dongles)
    {
        free(coders);
        return (-1);
    }
    sim -> dongles = dongles;
    while (i < args -> nb_coders)
    {
        if (dongle_init(&dongles[i], i) != 0)
        {
            while (i - 1 >= 0)
            {
                i--;
                dongle_destroy(&dongles[i]);
            }
            free(dongles);
            free(coders);
            return -1;
        }
        i++;
    }
    while (j < args -> nb_coders)
    {
        coders[j].id = j;
        coders[j].state = THINKING;
        coders[j].compile_count = 0;
        coders[j].last_compile_start_ms = now_ms();
        coders[j].sim = sim;
        coders[j].thread = 0;
        j++;
    }
    sim -> stop_flag = 0;
    sim -> start_ms = now_ms();

    if (pthread_mutex_init(&sim -> log_mutex, NULL) != 0)
    {
        i = 0;
        while (i < args -> nb_coders)
        {
            dongle_destroy(&dongles[i]);
            i++;
        }
        free(dongles);
        free(coders);
        return (-1);
    }
    
    if (pthread_mutex_init(&sim -> state_mutex, NULL) != 0)
    {
        i = 0;
        while (i < args -> nb_coders)
        {
            dongle_destroy(&dongles[i]);
            i++;
        }
        free(dongles);
        free(coders);
        pthread_mutex_destroy(&sim->log_mutex);
        return (-1);
    }
    return 0;
}

int sim_run(t_sim *sim)
{
    int i;

    i = 0;
	// pthread_create for monitor + all coder threads.
    // pthread_join in reverse order.
    // Returns 0 on clean stop, -1 on burnout.
    if (pthread_create(&sim -> monitor, NULL, monitor_routine, sim) != 0)
        return (-1);
    while (i < sim -> args.nb_coders)
    {
        if (pthread_create(&sim -> coders[i].thread, NULL, coder_routine, &sim -> coders[i]) != 0)
        {
            sim -> stop_flag = 1;
            break;
        }
        i++;
    }
    i = sim -> args.nb_coders - 1;
    while (i >= 0)
    {
        pthread_join(sim -> coders[i].thread, NULL);
        i--;
    }
    pthread_join(sim -> monitor, NULL);
    return (sim->stop_flag != 0) ? -1 : 0;
}

void    sim_destroy(t_sim *sim)
{
	// Frees coders[], calls dongle_destroy on each dongle,
    // destroys log_mutex and state_mutex.
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
