/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:35:56 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:53:17 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void   *coder_routine(void *arg)
{
	// Thread entry point (cast arg → t_coder *).
    // Loop: take left dongle → take right dongle → log compiling →
    //        sleep compile → put dongles → log debugging →
    //        sleep debug → log refactoring → sleep refactor → repeat.
    // Exits when sim->stop_flag is set.
    t_coder *c;
    int left;
    int right;

    c = (t_coder*)arg;
    left = coder_left_dongle_id(c);
    right = coder_right_dongle_id(c);
    while (c->sim->stop_flag == 0)
    {
        dongle_take(&c->sim->dongles[left], c);
        dongle_take(&c->sim->dongles[right], c);
        if (elapsed_ms(c->last_compile_start_ms) >= c->sim->args.time_to_burnout)
        {
            c->state = BURNED_OUT;
            c->sim->stop_flag = 1;
            dongle_put(&c->sim->dongles[right], c);
            dongle_put(&c->sim->dongles[left], c);
            return NULL;
        }
        // Compile phase
        c -> state = COMPILING;
        log_event(c->sim, c->id, "is compiling");
        sleep_ms(c->sim->args.time_to_compile);
        c->compile_count ++;
        c->last_compile_start_ms = now_ms();
        dongle_put(&c->sim->dongles[right], c);
        dongle_put(&c->sim->dongles[left], c);
        // Debugging phase
        c->state = DEBUGGING;
        log_event(c->sim, c->id, "is debugging");
        sleep_ms(c->sim->args.time_to_debug);
        // Refactoring phase
        c->state = REFACTORING;
        log_event(c->sim, c->id, "is refactoring");
        sleep_ms(c->sim->args.time_to_refactor);
        // Thinking process
        c->state = THINKING;
        log_event(c->sim, c->id, "is thinking");

        if ((c->sim -> args.nb_compiles_required > 0 &&
            (c->compile_count) >= c -> sim->args.nb_compiles_required))
        {
            c->sim->stop_flag = 1;
            break;
        }
    }
    return NULL;
}

static int  coder_left_dongle_id(const t_coder *c)
{
	return (c->id - 1 + c->sim -> args.nb_coders) % c->sim->args.nb_coders;
}

static int  coder_right_dongle_id(const t_coder *c)
{
	return (c->id % c->sim->args.nb_coders);
}
