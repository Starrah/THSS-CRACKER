#include <stdio.h>
struct Node{
	int value;
	int left;
	int right;
	int flag;
};
struct Node nodes[101];

void initTree()
{
	int i;
	for(i = 1; i < 101; i=i+1)
	{
		nodes[i].value = 0;
		nodes[i].left = -1;
		nodes[i].right = -1;
		nodes[i].flag = 0;
	}
	return;
}

void addLeftNode(int root, int value)
{
	if(nodes[root].flag==1)
	{
		int child = 2*root;
		nodes[child].flag = 1;
		nodes[child].value = value;
		nodes[root].left = child;
	}else
	{
		printf("Add node fail, Not a valid root node!");
	}
	return;
}

void addRightNode(int root, int value)
{
	if(nodes[root].flag==1)
	{
		int child = 2*root+1;
		nodes[child].flag = 1;
		nodes[child].value = value;
		nodes[root].right = child;
	}else
	{
		printf("Add node fail, Not a valid root node!");
	}
	return;
}

void preOrderTravel(int root)
{
	if(nodes[root].flag==1)
	{
		int left = nodes[root].left;
		int right = nodes[root].right;
		printf("%d->",nodes[root].value);
		preOrderTravel(left);
		preOrderTravel(right);
	}else
	{
		return;
	}
	return;
}

int main() 
{
	initTree();
	nodes[1].value = 1;
	nodes[1].flag = 1;
	addLeftNode(1,2);
	addRightNode(1,3);
	addLeftNode(2,4);
	addRightNode(2,5);
	addLeftNode(4,8);
	addRightNode(4,9);
	printf("pre-order traval result: \n");
	preOrderTravel(1);
	printf("$\n");
	return 0;
}
