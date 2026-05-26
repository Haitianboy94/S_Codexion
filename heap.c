/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:05 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/26 12:26:25 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *h, int capacity)
{
	h->data = malloc(sizeof(t_heapnode) * capacity);
	if (!h->data)
		return (-1);
	h->size = 0;
	h->capacity = capacity;
	return (0);
}

void	heap_free(t_heap *h)
{
	free (h->data);
	h->data = NULL;
	h->size = 0;
	h->capacity = 0;
}

int	heap_push(t_heap *h, t_heapnode node)
{
	if (h->size >= h->capacity)
		return (-1);
	h->data[h->size] = node;
	h->size++;
	heap_sift_up(h, h->size -1);
	return (0);
}

t_heapnode	heap_pop(t_heap *h)
{
	t_heapnode	mini;

	mini = h->data[0];
	h->data[0] = h->data[h->size - 1];
	h->size--;
	heap_sift_down(h, 0);
	return (mini);
}
