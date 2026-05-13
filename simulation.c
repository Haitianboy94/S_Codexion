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

    return 0;
}

int sim_run(t_sim *sim)
{
	// pthread_create for monitor + all coder threads.
    // pthread_join in reverse order.
    // Returns 0 on clean stop, -1 on burnout.
}

void    sim_destroy(t_sim *sim)
{
	// Frees coders[], calls dongle_destroy on each dongle,
    // destroys log_mutex and state_mutex.
}
