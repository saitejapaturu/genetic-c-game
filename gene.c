/******************************************************************************
** Student name: 	Sri Sai Teja Paturu
** Student number: 	s3644335
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "gene.h"

int * create_pcbmill_chrom(int numAlleles)
{
	int * p = safeMalloc(sizeof(*p) * numAlleles);
	int i;

	for(i = 0; i < numAlleles; i++)
	{
		p[i] = i;
	}

	for(i = 0; i < numAlleles; i++)
	{
		int temp = p[i];
		int random = rand() % numAlleles;
		p[i] = p[random];
		p[random] = temp;
	}
	return p;
}

int * create_minfn_chrom(int numAlleles)
{
	int * p = safeMalloc(sizeof(*p) * numAlleles);
	int i;

	for(i = 0; i < numAlleles; i++)
	{
		p[i] = rand() % (MINFN_MAX + 1);
	}

	return p;
}

Gene * mutate_pcbmill(Gene *g)
{

	Gene * mutate = safeMalloc(sizeof(*mutate));
	int i;

	mutate->raw_score = mutate->fitness = 0;
	mutate->num_alleles = g->num_alleles;
	mutate->chromosome = safeMalloc(sizeof(*mutate->chromosome) * mutate->num_alleles);

	for(i=0; i< mutate->num_alleles; i++){
	mutate->chromosome[i] = g->chromosome[i];
	}

	mutate->chromosome[2] = mutate->chromosome[4];
	mutate->chromosome[4] = g->chromosome[2];

	return mutate;
}

Gene * mutate_minfn(Gene *g)
{
	Gene *clone = safeMalloc(sizeof(*clone));
	int i;

	clone->raw_score = clone->fitness = 0;
	clone->num_alleles = g->num_alleles;
	clone->chromosome = safeMalloc(sizeof(*clone->chromosome) * clone->num_alleles);

	for (i = 0; i < clone->num_alleles; i++)
	{
		clone->chromosome[i] = g->chromosome[i];
	}
	#ifdef DEBUG
	clone->chromosome[2] = rand() % (MINFN_MAX + 1);
	#else
	clone->chromosome[rand() % clone->num_alleles] = rand() % (MINFN_MAX + 1);
	#endif

	return clone;
}

Gene * crossover_pcbmill(Gene *g1, Gene *g2)
{

	Gene * child = safeMalloc(sizeof(*child));
	int index1, index2 , i, currentChildIndex = 0, g2Index = 0;
	Boolean valid;

	child->raw_score = child->fitness = 0;
	child->num_alleles = g1->num_alleles;
	child->chromosome = safeMalloc(sizeof(*child->chromosome) * child->num_alleles);

	#ifdef DEBUG
		index1 = 2;
		index2 = 4;
	#else

		do
		{
			index1 = rand() % child->num_alleles;
			index2 = rand() % child->num_alleles;
		} while (index2 < index1);

	#endif

	for (i = index1; i <= index2; i++)
	{
		child->chromosome[currentChildIndex] = g1->chromosome[i];
		currentChildIndex++;
	}

	while (currentChildIndex < child->num_alleles)
	{
		valid = TRUE;
		/*Checking for repeated values*/
		for (i = 0; i < currentChildIndex; i++)
		{
			if (g2->chromosome[g2Index] == child->chromosome[i])
			{
				valid = FALSE;
			}
		}
		if (valid)
		{
			child->chromosome[currentChildIndex] = g2->chromosome[g2Index];
			currentChildIndex++;
		}
		g2Index++;
	}

	return child;
}

Gene * crossover_minfn(Gene *g1, Gene *g2)
{
	Gene *child = safeMalloc(sizeof(*child));
	int index, i;
		child->raw_score = child->fitness = 0;
	child->num_alleles = g1->num_alleles;
	child->chromosome = safeMalloc(sizeof(*child->chromosome) * child->num_alleles);
	#ifdef DEBUG
	index=2;

	#else
	index = rand() % child->num_alleles;

	#endif



	for(i = 0; i <= index; i++)
	{
		child->chromosome[i] = g1->chromosome[i];
	}
	for(i = index + 1; i < child->num_alleles; i++)
	{
		child->chromosome[i] = g2->chromosome[i];
	}
	return child;
}

double eval_pcbmill(InVTable *invt, Gene *gene)
{
	/* calculates (x1-x2)^2 + (y1-y2)^2 and square roots it and adds it to the total. */
	int * vector1, * vector2;
	int xdiff, ydiff, i;
	double result = 0.0;

	for (i = 1; i < gene->num_alleles; i++)
	{

		vector1 = invt->table[gene->chromosome[i]];
		vector2 = invt->table[gene->chromosome[i - 1]];
		xdiff = vector1[0] - vector2[0];
		ydiff = vector1[1] - vector2[1];
		result += sqrt(xdiff * xdiff + ydiff * ydiff);
	}

	return fabs(result);
}

double eval_minfn(InVTable *invt, Gene *gene)
{
	double totalRawScore;
	int i;

	totalRawScore = 0;
	for (i = 0; i < invt->tot; i++)
	{
		int *vector = invt->table[i];
		int j;
		double rawScore;

		rawScore = 0;
		for (j = 0; j < gene->num_alleles; j++)
		{
			rawScore += vector[j] * gene->chromosome[j];
		}
		rawScore -= vector[gene->num_alleles];

		totalRawScore += fabs(rawScore);
	}
	return totalRawScore;
}

Gene * gene_create_rand_gene(int numAlleles, CreateFn create_chrom)
{
	Gene * gene = safeMalloc(sizeof(*gene));
	gene->raw_score = gene->fitness = 0;
	gene->num_alleles = numAlleles;
	gene->chromosome = create_chrom(gene->num_alleles);

	return gene;
}

void gene_calc_fitness(Gene *gene, EvalFn evaluate_fn, InVTable *invTab)
{
	gene->raw_score = evaluate_fn(invTab, gene);
	gene->fitness = gene_get_fitness(gene);
}

void gene_normalise_fitness(Gene *gene, double total_fitness)
{
	gene->fitness = gene->fitness / total_fitness;
}

void gene_free(Gene *gene)
{
	free(gene->chromosome);
	free(gene);
}


double gene_get_fitness(Gene *gene)
{
	return 1 / (gene->raw_score + 1);
}

void gene_print(Gene *gene)
{
	int i;

	printf("chrom: ");
	for (i = 0; i < gene->num_alleles; i++)
	{
		printf("%2d", gene->chromosome[i]);
		if (i < gene->num_alleles - 1)
		{
			printf(",");
		}
		printf(" ");
	}
	printf("fit: %.3f raw: %.3f \n", gene->fitness, gene->raw_score);
}
