#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node_data_t
{
	int id;
	char type;
	char name[20];
}node_data_t;

typedef struct element_t
{
	node_data_t node_data;
	struct element_t* parent;
	struct element_t* first_child;
	struct element_t* next_sibling;
}element_t;

typedef struct tree_head_t
{
	int size;
}tree_head_t;




int main()
{
	int size = 8;
	element_t* elements = malloc(size*sizeof(element_t));

	elements[0].node_data.id = 0;
	elements[0].node_data.type = 'D';
	strcpy(elements[0].node_data.name, "Home");
	elements[0].parent = NULL;
	elements[0].first_child = &(elements[1]);
	elements[0].next_sibling = NULL; 
	

	elements[1].node_data.id = 1;
	elements[1].node_data.type = 'D';
	strcpy(elements[1].node_data.name, "Downloads");
	elements[1].parent = &(elements[0]);
	elements[1].first_child = NULL;
	elements[1].next_sibling = &(elements[2]);


	elements[2].node_data.id = 2;
	elements[2].node_data.type = 'D';
	strcpy(elements[2].node_data.name, "Documents");
	elements[2].parent = &(elements[0]);
	elements[2].first_child = &(elements[3]);
	elements[2].next_sibling = &(elements[5]);

	elements[3].node_data.id = 3;
	elements[3].node_data.type = 'F';
	strcpy(elements[3].node_data.name, "file3");
	elements[3].parent = &(elements[2]);
	elements[3].first_child = NULL;
	elements[3].next_sibling = &(elements[4]);

	elements[4].node_data.id = 4;
	elements[4].node_data.type = 'F';
	strcpy(elements[4].node_data.name, "file4");
	elements[4].parent = &(elements[2]);
	elements[4].first_child = NULL;
	elements[4].next_sibling = NULL;

	elements[5].node_data.id = 5;
	elements[5].node_data.type = 'D';
	strcpy(elements[5].node_data.name, "Music");
	elements[5].parent = &(elements[0]);
	elements[5].first_child = &(elements[6]);
	elements[5].next_sibling = &(elements[7]);

	elements[6].node_data.id = 6;
	elements[6].node_data.type = 'F';
	strcpy(elements[6].node_data.name, "file6");
	elements[6].parent = &(elements[5]);
	elements[6].first_child = NULL;
	elements[6].next_sibling = NULL;

	elements[7].node_data.id = 7;
	elements[7].node_data.type = 'F';
	strcpy(elements[7].node_data.name, "file7");
	elements[7].parent = &(elements[0]);
	elements[7].first_child = NULL;
	elements[7].next_sibling = NULL;

	tree_head_t tree_head;
	tree_head.size = 8;	

	FILE* file = fopen("data", "wb");
	fwrite(&(tree_head), sizeof(tree_head), 1, file);
	int i = 0;
	char relation;
	while (i < 8)
	{
		fwrite(&(elements[i].node_data), sizeof(elements[i].node_data), 1, file);
		if(elements[i].first_child == NULL)
		{
			if (elements[i].next_sibling == NULL)
			{
				if (elements[i].parent->next_sibling == NULL)
					relation = 'N';
				else
					relation = 'U';
			}
			else
				relation = 'S';
		}
		else
		{
			relation = 'D';
		}
		fwrite(&relation, sizeof(char), 1, file);
		i=i+1;
	}
	fclose(file);

	tree_head_t my_tree_head;
	file = fopen("data", "r");
	fread(&my_tree_head, sizeof(tree_head_t), 1, file);
	printf("Number of nodes = %d\n", my_tree_head.size);


	element_t* my_elements = malloc(my_tree_head.size*sizeof(element_t));
	char my_relation = 'A';
	i = 0;
	while((i<my_tree_head.size) && (!(my_relation=='N')))
	{
		fread(&(my_elements[i].node_data), sizeof(my_elements[i].node_data), 1, file);
		printf("%s,", my_elements[i].node_data.name);
		my_elements[i].parent = NULL;
		my_elements[i].first_child = NULL;
		my_elements[i].next_sibling = NULL;

		if (i > 0)
		{
			if(my_relation == 'D')
			{
				my_elements[i].parent = &(my_elements[i-1]);
				my_elements[i-1].first_child = &(my_elements[i]);
			}
			else if(my_relation =='S')
			{
				my_elements[i].parent = my_elements[i-1].parent;
				my_elements[i-1].next_sibling = &(my_elements[i]);
			}
			else if(my_relation == 'U')
			{
				my_elements[i].parent = my_elements[i-1].parent->parent;
				my_elements[i-1].parent->next_sibling = &(my_elements[i]);
			}
			else
			{
				printf("File Corrupted\n");
			}
		}
		fread(&my_relation, sizeof(char), 1, file);
		printf("%c\n", my_relation);
		i=i+1;
	}

	i = 0;
	while(i< my_tree_head.size)
	{
		printf("Node name is %s, Node name is %s\n", 
				elements[i].node_data.name, my_elements[i].node_data.name);
		if(!(elements[i].parent == NULL))
		printf("Parent name is %s, Parent name is %s\n", 
				elements[i].parent->node_data.name,
				my_elements[i].parent->node_data.name);
		if(!(elements[i].next_sibling == NULL))
		printf("Sibling name is %s, Sibling name is %s\n", 
				elements[i].next_sibling->node_data.name,
				my_elements[i].next_sibling->node_data.name);
		if(!(elements[i].first_child == NULL))
		printf("First child name is %s, First child name is %s\n", 
				elements[i].first_child->node_data.name,
				my_elements[i].first_child->node_data.name);
		i=i+1;
	}

}
