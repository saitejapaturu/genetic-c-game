/******************************************************************************
** Student name: 	Sri Sai Teja Paturu
** Student number: 	s3644335
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "drive.h"
#include "invector.h"
#include "pop.h"
#include "gene.h"

int main(int argc, char *argv[])
{
	/* Creating variables */
	FILE *fp;
	InVTable invt;
	Pop_list *list;
	int i, numAllels, populationSize, numberGeneration;

	/* The only point at which srand should be called */
	srand(SRAND_SEED);

#ifdef DEBUG
	test_minfn();
	test_pcbmill();
#endif

	/* checks for incorrect number of arguments */
	if (argc != CMD_ARG_MAX - 1 && argc != CMD_ARG_MAX)
	{
		printf("Incorrect number of arguments. \n\n");
		return EXIT_FAILURE;
	}

	/* Check for null file */
	fp = fopen(argv[inputFile], "r");
	if (fp == NULL)
	{
		printf("Unable to open file. \n\n");
		return EXIT_FAILURE;
	}

	/* Checking for the file data. */
	invector_init(&invt);
	if (!readInvector(&invt, fp))
	{
		printf("Corrupt file. \n\n");
		fclose(fp);
		return EXIT_FAILURE;
	}

	fclose(fp);

	if (argc == CMD_ARG_MAX && freopen(argv[outputFile], "w", stdout) == NULL)
	{
		printf("Unable to open output file. \n\n");
		return EXIT_FAILURE;
	}

	printf("\n");

	pop_init(&list);

	/* Checking for minfn or pcbmill */
	if (strcmp(argv[geneType], "minfn") == 0)
		pop_set_fns(list, create_minfn_chrom, mutate_minfn, crossover_minfn, eval_minfn);
	else if (strcmp(argv[geneType], "pcbmill") == 0)
		pop_set_fns(list, create_pcbmill_chrom, mutate_pcbmill, crossover_pcbmill, eval_pcbmill);
	else
	{
		printf("Unknown gene type. \n\n");
		return EXIT_FAILURE;
	}

	/* Creating initial file. */
	numAllels = strtol(argv[alleleSize], NULL, 10);
	populationSize = strtol(argv[popSize], NULL, 10);
	numberGeneration = strtol(argv[numGen], NULL, 10);
	i = 0;
	createInitialPopulation(list, populationSize, numAllels);

/* Calculate the fitness sort and print. */
while(i<numberGeneration)
{
	calculateFitness(list, &invt);
	sortPopulation(list);
	/*pop_display(list);*/
	printf("Gen: %4d ", i);
	pop_print_fittest(list);
	list = mutateAndCrossOverPopulation(list, &invt);
	i++;
}

freePop(list);

	if (argc == CMD_ARG_MAX)
		fclose(stdout);

	return EXIT_SUCCESS;
}

void test_pcbmill(void)
{
	Gene *gene, *mutant, *parent1, *parent2;

	printf("PCBMILL gene:\n");
	/* TO DO - create a random pcbmill gene by calling create_rand_gene
	   The length of the gene's chromosome should be TEST_ALLELE_LEN */
	gene = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	gene_print(gene);

	/* TO DO - print the gene using gene_print */
	printf("\nMutate: ");
	/* TO DO - create a mutant copy of the gene using mutate_pcbmill */
	/* TO DO - print the mutant gene using gene_print */
	mutant = mutate_pcbmill(gene);
	gene_print(mutant);

	printf("\n");
	/* TO DO - free the original gene and the mutant */
	printf("PCBMILL genes:\n");
	/* TO DO - create 2 random pcbmill 'parent' genes using calls
	to create_rand_gene
	   The length of the gene's chromosome should be TEST_ALLELE_LEN */
	/* TO DO - print each gene */
	parent1 = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	gene_print(parent1);
	parent2 = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	gene_print(parent2);

	printf("\nCrossover: ");
	/* TO DO produce a new gene by calling crossover_pcbmill
	   with the parent genes */
	/* TO DO - print the new gene */
	gene = crossover_pcbmill(parent1, parent2);
	gene_print(gene);

	printf("\n");
	/* TO DO - free both parents and the child gene */
	gene_free(parent1);
	gene_free(parent2);
	gene_free(gene);
}

void test_minfn(void)
{
	Gene *gene, *mutant, *parent1, *parent2;

	printf("MINFN gene:\n");
	/* TO DO - create a random minfn gene by calling create_rand_gene
	   The length of the gene's chromosome should be TEST_ALLELE_LEN */
	gene = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	gene_print(gene);

	/* TO DO - print the gene */
	printf("\nMutate: ");
	/* TO DO - create a mutant copy of the gene using mutate_minfn */
	/* TO DO - print the mutant gene */
	mutant = mutate_minfn(gene);
	gene_print(mutant);

	printf("\n");

	/* TO DO - free the original gene and the mutant */
	printf("MINFN genes:\n");
	/* TO DO - create 2 random minfn 'parent' genes using calls
	to create_rand_gene
	   The length of the gene's chromosome should be TEST_ALLELE_LEN */
	/* TO DO - print each gene */
	parent1 = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	gene_print(parent1);
	parent2 = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	gene_print(parent2);

	printf("\nCrossover: ");
	/* TO DO produce a new gene by calling crossover_minfn
	   with the parent genes */
	/* TO DO - print the new gene */
	gene = crossover_minfn(parent1, parent2);
	gene_print(gene);

	printf("\n");

	/* TO DO - free both parents and the child gene */
	gene_free(parent1);
	gene_free(parent2);
	gene_free(gene);
}
