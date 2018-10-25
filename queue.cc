#include <iostream> //for "evicting key..." printout

struct node //node for Queue
{
	std::string* value;
	uint32_t size;
	node *next;
};

class Queue //Queue for evictor
{
public:
	node *head;
	Queue()
		{head = NULL;}

	/*~Queue()
	{
		node *prev;
		node *curr;
		while(curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
		}
	}*/
	
	void enqueue(std::string* val, uint32_t sz)
	{
		node *temp = new node;
		temp->value = val;
		temp->size = sz;
		temp->next = NULL;
		if(head == NULL)
			{head = temp;}
		else
		{
			node *traverse;
			traverse = head;
			while(traverse->next != NULL)
				{traverse = traverse->next;}
			traverse->next = temp;
		}
	}

	uint32_t rem(std::string val)
	{
		node *curr;
		node *prev;
		curr = head;
		uint32_t out;
		while((curr->next != NULL) && (*curr->value != val))
		{
			prev = curr;
			curr = curr->next;
		}
		if(*curr->value != val) {return 0;}
		else if (curr == head)
		{
			out = curr->size;
			head = curr->next;
			delete curr->value;
			delete curr;
			return out;
		}
		else
		{
			out = curr->size;
			prev->next = curr->next;
			delete curr->value;
			delete curr;
			return out;
		}
	}

	void dequeue()
	{
		node *save;
		save = head;
		head = save->next;
		delete save;
	}

	void display()
	{
		node *traverse;
		traverse = head;
		while(traverse != NULL)
		{
			std::cout << traverse->size << "\t" << traverse->value << "\t" << *traverse->value << std::endl;
			traverse = traverse->next;
		}
		std::cout << std::endl;
	}
};
void queue_test()
{
	Queue queue_boy;
	std::string a = "5";
	std::string b = "8";
	std::string c = "4";
	std::string d = "6";
	std::string* ap = &a;
	std::string* bp = &b;
	std::string* cp = &c;
	std::string* dp = &d;
	queue_boy.enqueue(ap,5);
	queue_boy.enqueue(bp,8);
	queue_boy.enqueue(cp,4);
	queue_boy.enqueue(dp,6);
	queue_boy.display();

	queue_boy.rem(d);
	queue_boy.display();

	queue_boy.rem(a);
	queue_boy.display();

	queue_boy.rem(a);
	queue_boy.display();

	queue_boy.dequeue();
	queue_boy.display();
}

int main()
{
	queue_test();
}