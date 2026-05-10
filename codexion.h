/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:27:53 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:33:00 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CODEXION_H
# define CODEXION_H

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

typedef enum	e_sched	{
	FIFO,
	EDF
}	t_sched;
typedef enum    e_state {
	THINKING,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}	t_state;

typedef struct  s_args {
    int         nb_coders;
    long        time_to_burnout;
    long        time_to_compile;
    long        time_to_debug;
    long        time_to_refactor;
    int         nb_compiles_required;
    long        dongle_cooldown;
    t_sched     scheduler;
}	t_args;

typedef struct  s_heap_node {
    int         coder_id;
    long        priority;
}	t_heapnode;

typedef struct  s_heap {
    t_heapnode    *data;
    int         size;
    int         capacity;
}	t_heap;

typedef struct  s_dongle {
    int             id;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    t_heap          queue;
    long            available_at_ms;
    int             held_by;
}	t_dongle;

typedef struct  s_coder {
    int             id;
    t_state         state;
    long            last_compile_start_ms;
    int             compile_count;
    pthread_t       thread;
    struct s_sim   *sim;
}	t_coder;

typedef struct  s_sim {
    t_args          args;
    t_coder        *coders;
    t_dongle       *dongles;
    pthread_t       monitor;
    pthread_mutex_t log_mutex;
    pthread_mutex_t state_mutex;
    int             stop_flag;
    long            start_ms;
}	t_sim;

#endif