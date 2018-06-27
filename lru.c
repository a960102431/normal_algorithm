#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define CAPACITY 100
#define DEFAULTMODE 0666
#define FILE "lru.data"

typedef struct Node *LNode;
typedef LNode LHead;
typedef struct LCUNode *LLCU;
typedef struct NodeData *LNodeData;

struct Node
{
	int key;
	char data[100];
	LNode next;
};

struct NodeData
{
	int key;
	char data[100];
};

struct LCUNode
{
	LHead head;
	int capacity;
	int num;
};

LHead init_list()
{
	LHead head = (LHead)(malloc(sizeof(struct Node)));
	head->next = NULL;
}

void list_lpush(LHead head, LNode lnode)
{
	lnode->next = head->next;
	head->next = lnode;
}

void list_rdelete(LHead head)
{
	LNode lnode = head->next;
	LNode front = head;
	while(lnode && lnode->next != NULL)
	{
		lnode = lnode->next;
		front = front->next;
	}
	front->next = NULL;
	free(lnode);
}

LNode list_pop(LHead head, int key)
{
	LNode lnode = head->next;
	LNode front = head;
	while(lnode && lnode->key != key)
	{
		lnode = lnode->next;
		front = front->next;
	}
	if(lnode == NULL)
		return NULL;
	front->next = lnode->next;
	return lnode;
}

void init_data(int num, char *file)
{
	int i = 0;
	struct NodeData data;
	int fd;
	fd = open(file, O_WRONLY|O_CREAT, DEFAULTMODE);
	for(i = 0; i < num; i++)
	{
		data.key = i; 
		write(fd, (char *)(&data), sizeof(data));
	}
	close(fd);
}

LNode lcu_read(LLCU lcu, int key)
{
	LNode node = list_pop(lcu->head, key);
	LNode rnode = (LNode)(malloc(sizeof(struct Node)));
	LNodeData rlcunode = NULL;
	int fd;
	if(node){
		list_lpush(lcu->head, node);
		return node;
	}else{
		fd = open(FILE, O_RDONLY);
		int i = 0,j = 0;
		char buffer[sizeof(struct NodeData) * 10];
		char *p = buffer;
		int num;
		int find = 0;
		while((num = read(fd,buffer,sizeof(struct NodeData) * 10)) && find == 0)
		{
			p = buffer;
			for(i = 0; i < num / sizeof(struct NodeData); i++)
			{
				p += sizeof(struct NodeData);
				rlcunode = (LNodeData)p;//printf("%d  ",rlcunode->key);
				if(rlcunode->key == key) {
					rnode->key = rlcunode->key;
					rnode->next = NULL;
					j = 0;
					while(j < 100)
					{
						rnode->data[j] = rlcunode->data[j];
						j++;
					}
					find = 1;
					break;
				}
			}
		}
		close(fd);
		if(find)
		{
			list_lpush(lcu->head, rnode);
			if(lcu->num < lcu->capacity)
			{
				lcu->num++;
			} else {
				list_rdelete(lcu->head);
			}
		}
	}
		
}
void print_head(LHead head)
{
	LNode node = head->next;
	printf("list data:", node->key);
	while(node){
		printf("%d  ", node->key);
		node = node->next;
	}
}
int main(int argc, char argv[])
{
	//init_data(100, FILE);return;
	LLCU lcu = (LLCU)(malloc(sizeof(struct LCUNode)));
	lcu->head = init_list();
	lcu->capacity = 10;
	lcu->num = 0;
	int count = 0;
	int key;
	printf("input key:");
	scanf("%d", &key);
	while(key > 0)
	{
		lcu_read(lcu, key);
		print_head(lcu->head);
		printf("\n");
		printf("input key:");
		scanf("%d", &key);
	}
}
