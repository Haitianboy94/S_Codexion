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
    sim -> args = *args;
    t_coder *coders;
    t_dongle *dongles;

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
