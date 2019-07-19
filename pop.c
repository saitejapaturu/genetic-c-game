/******************************************************************************
** Student name: 	Sri Sai Teja Paturu
** Student number: 	s3644335
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "pop.h"

Boolean pop_init(Pop_list **pop)
{
	Pop_list * p = safeMalloc(sizeof(*p));
	p->head = NULL;
	p->count = 0;
	p->create_rand_chrom = NULL;
	p->mutate_gene = NULL;
	p->crossover_genes = NULL;
	p->evaluate_fn = NULL;

	*pop = p;

	return TRUE;
}

void pop_set_fns(Pop_list *p,CreateFn cf,MutateFn mf,CrossOverFn cof,EvalFn ef)
{
	p->create_rand_chrom = cf;
	p->mutate_gene = mf;
	p->crossover_genes = cof;
	p->evaluate_fn = ef;
}

void pop_print_fittest(Pop_list *p)
{
	gene_print(p->head->gene);
}

/* To create initial population */
void createInitialPopulation(Pop_list * p, int popSize, int alleleSize)
{
	int i;

	for(i = 0; i < popSize; i++)
	{
		Pop_node * node = createPopNode(alleleSize, p->create_rand_chrom);
		insertNode(p, node);
	}
}

/* Creates and returns a population node */
Pop_node * createPopNode(int alleleSize, CreateFn cf)
{
	Pop_node * node = safeMalloc(sizeof(*node));
	node->gene = gene_create_rand_gene(alleleSize, cf);
	node->next = NULL;

	return node;
}

/* Inserts a node in the list */
void insertNode(Pop_list * p, Pop_node * node)
{
	node->next = p->head;
	p->head = node;
	p->count++;
}

/* Calcuulates the fitness */
void calculateFitness(Pop_list * list, InVTable * invt)
{
	Pop_node * node;
	double totalFitness;

	totalFitness = 0;
	node = list->head;
	while(node != NULL)
	{
		gene_calc_fitness(node->gene, list->evaluate_fn, invt);
		totalFitness += node->gene->fitness;
		node = node->next;
	}

	node = list->head;
	while(node != NULL)
	{
		gene_normalise_fitness(node->gene, totalFitness);
		node = node->next;
	}
}

/* Evaluates the population */
void evaluatePopulation(Pop_list * p, InVTable * invt)
{
	Pop_node * node;
	double totalFitness;

	node = p->head;
	while(node != NULL)
	{
		gene_calc_fitness(node->gene, p->evaluate_fn, invt);
		node = node->next;
	}

	/* Normalising fitness. */
	totalFitness = 0;
	node = p->head;
	while(node != NULL)
	{
		totalFitness += node->gene->fitness;
		node = node->next;
	}
	node = p->head;
	while(node != NULL)
	{
		gene_normalise_fitness(node->gene, totalFitness);
		node = node->next;
	}
}

/* Mutates, cross over and frees the population */
 Pop_list * mutateAndCrossOverPopulation(Pop_list * p, InVTable * invt)
{
	Pop_list * newPopulation;
	int i;

	if(!pop_init(&newPopulation))
		return NULL;
	pop_set_fns(newPopulation, p->create_rand_chrom, p->mutate_gene, p->crossover_genes, p->evaluate_fn);

	sortPopulation(p);
	insertNode(newPopulation, clonePopNode(fittestPopNode(p)));
	for(i = 0; i < p->count - 1; i++)
	{
		Pop_node * newNode = safeMalloc(sizeof(*newNode));
		newNode->gene = p->crossover_genes((randomPopNode(p)->gene), (randomPopNode(p)->gene));
		newNode->next = NULL;


		if(rand() % 100 < MUTATE_RATE)
		{
			Gene * gene = p->mutate_gene(newNode->gene);
			gene_free(newNode->gene);
			newNode->gene = gene;
		}

		insertNode(newPopulation, newNode);
	}
freePop(p);
	return newPopulation;
}

/* Returns a random node */
Pop_node *randomPopNode(Pop_list *pop)
{
    double random  = (double) rand() / RAND_MAX;
    Pop_node *node = pop->head;
    double totalFitness = 0.0;
    while(node != NULL)
    {
        totalFitness = totalFitness + node->gene->fitness;

        if(totalFitness >= random)
            break;
        node = node->next;
    }
    return node;

}

/* Swaps the node */
void swapPopNode(Pop_node * previous, Pop_node * current)
{
	Gene * gene = previous->gene;
	previous->gene = current->gene;
	current->gene = gene;
}

/* Returns fittet Pop node */
Pop_node * fittestPopNode(Pop_list * p)
{
	Pop_node * node, * fittestNode;

	node = p->head;
	fittestNode = node;
	while(node != NULL)
	{
		if(node->gene->fitness > fittestNode->gene->fitness)
			fittestNode = node;
		node = node->next;
	}

	return fittestNode;
}

/* Sorts the population */
void sortPopulation(Pop_list * list)
{
	Pop_node ** nodes, * node;
	int i;

	nodes = safeMalloc(sizeof(*nodes) * list->count);
	node = list->head;
	i = 0;
	while(node != NULL)
	{
		nodes[i++] = node;
		node = node->next;
	}

	qsort(nodes, list->count, sizeof(*nodes), compareNodes);

	for(i = 0; i < list->count; i++)
	{
		nodes[i]->next = NULL;
	}

	list->head = NULL;
	for(i = list->count - 1; i>=0; i--)
	{
		nodes[i]->next = list->head;
		list->head = nodes[i];
	}

	free(nodes);
}

/* Compares the nodes */
int compareNodes(const void * p1, const void * p2)
{
	Pop_node ** node1 = (Pop_node **) p1, ** node2 = (Pop_node **) p2;
	double fitness1 = (*node1)->gene->fitness, fitness2 = (*node2)->gene->fitness;

	return fitness1 < fitness2 ? 1 : fitness1 > fitness2 ? -1 : 0;
}

/* Prints the list */
void printList(Pop_list * list)
{
	double totalFitness = 0;
	Pop_node * node = list->head;
	while(node != NULL)
	{
		gene_print(node->gene);
		totalFitness += node->gene->fitness;
		node = node->next;
	}
	printf("Total Fitness: %f \n\n", totalFitness);
}

/* Clones the given pop node. */
Pop_node * clonePopNode(Pop_node * node)
{
	Pop_node * clone = safeMalloc(sizeof(*clone));
	clone->next = NULL;
	clone->gene = safeMalloc(sizeof(*clone->gene));
	clone->gene->num_alleles = node->gene->num_alleles;
	clone->gene->raw_score = node->gene->raw_score;
	clone->gene->fitness = node->gene->fitness;
	clone->gene->chromosome = safeMalloc(sizeof(*clone->gene->chromosome) * clone->gene->num_alleles);

	memcpy(clone->gene->chromosome, node->gene->chromosome,
		sizeof(*clone->gene->chromosome) * clone->gene->num_alleles);

	return clone;
}

/* Prints the pop list */
void pop_display(Pop_list *pop)
{
    Pop_node *node = pop->head;
    while(node != NULL)
    {
        gene_print(node->gene);
        /*pop_print_fittest(pop);*/
        node = node->next;
    }
}

/* Frees the population */
void freePop(Pop_list * p)
{
	Pop_node * node = p->head;
	while(node != NULL)
	{
		Pop_node * temp = node;
		node = node->next;
		freePopNode(temp);
	}
	free(p);
}

/* Frees population node. */
void freePopNode(Pop_node * node)
{
	if(node->gene != NULL)
		gene_free(node->gene);
	free(node);
}
