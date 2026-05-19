/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 16:18:00 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 13:28:45 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int     heap_empty(const t_heap *h)
{
	return(h->size == 0);
}

t_heapnode heap_peek(const t_heap *h)
{
	// Returns minimum node without removing
	return(h->data[0]);
}

void     heap_sift_up(t_heap *h, int i)
{
	int parent;
	t_heapnode tmp;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->data[i].priority < h->data[parent].priority)
		{
			tmp = h->data[i];
			h->data[i] = h->data[parent];
			h->data[parent] = tmp;
			i = parent;
		}
		else
			break;
	}
}

void     heap_sift_down(t_heap *h, int i)
{
	int left;
	int right;
	int smallest;
	t_heapnode tmp;

	while (true)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < h->size && h->data[left].priority < h->data[smallest].priority)
			smallest = left;
		if (right < h->size && h->data[right].priority < h->data[smallest].priority)
			smallest = right;
		if (smallest == i)
			break;
		tmp = h->data[i];
		h->data[i] = h->data[smallest];
		h->data[smallest] = tmp;
		i = smallest;
	}
}
