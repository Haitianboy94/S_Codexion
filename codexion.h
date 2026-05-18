/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:27:53 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 11:50:35 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CODEXION_H
# define CODEXION_H

#define _POSIX_C_SOURCE 199309L
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

struct timespec ts;

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

int	parse_args(int argc, char **argv, t_args *out);
int	validate_args(const t_args *a);
void   *coder_routine(void *arg);
int dongle_init(t_dongle *d, int id);
void    dongle_destroy(t_dongle *d);
void    dongle_take(t_dongle *d, t_coder *coder);
void    dongle_put(t_dongle *d, t_coder *coder);
int	heap_init(t_heap *h, int capacity);
void    heap_free(t_heap *h);
int	heap_push(t_heap *h, t_heapnode node);
t_heapnode heap_pop(t_heap *h);
t_heapnode heap_peek(const t_heap *h);
long    now_ms(void);
long    elapsed_ms(long since_ms);
void    sleep_ms(long ms);
void    log_init(t_sim *sim);
void    log_destroy(t_sim *sim);
void    log_event(t_sim *sim, int coder_id, const char *msg);
void   *monitor_routine(void *arg);
long    sched_priority(const t_sim *sim, int coder_id);
int sched_request(t_dongle *d, int coder_id, long priority);
int	sched_next(t_dongle *d);
int sim_init(t_sim *sim, const t_args *args);
int sim_run(t_sim *sim);
void    sim_destroy(t_sim *sim);
long ft_atol(char *str);
char	*ft_itoa(int n);

#endif