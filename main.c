#include <stdbool.h>
#include <stdio.h>
#include <string.h> 


extern bool parseAndExecute(char *input);
extern void executeLook(const char *noun);
extern void executeGo(const char *noun);


typedef struct object {
	const char *description;
	const char *tag;
	struct object *location;
} OBJECT;

extern OBJECT objs[];

#define field	(objs + 0)
#define cave	(objs + 1)
#define silver  (objs + 2)
#define gold	(objs + 3)
#define guard	(objs + 4)
#define player	(objs + 5)

#define endOfObjs	(objs + 6)


OBJECT objs[] = {
	{"an open field", "field"	, NULL  },
	{"a little cave", "cave"	, NULL  },
	{"a silver coin", "silver"	, field },
	{"a gold coin"	, "gold"	, cave	},
	{"a burly guard", "guard"	, field	},
	{"yourself"	, "yourself"	, field	}
};



static bool objectHasTag(OBJECT *obj, const char *noun)
{
	return noun != NULL && *noun != '\0' && strcmp(noun, obj->tag) == 0;
}

static OBJECT *getObject(const char *noun)
{
	OBJECT *obj, *res = NULL;
	for (obj = objs; obj < endOfObjs; obj++)
	{
		if (objectHasTag(obj, noun))
		{
			res = obj;
		}
	}
	return res;
}

OBJECT *getVisible(const char *intention, const char *noun)
{
	OBJECT *obj = getObject(noun);
	if (obj == NULL)
	{
		printf("I don't understand %s.\n", intention);
	}
	else if (!(obj == player ||
		obj == player->location ||
		obj->location == player ||		
		obj->location == player->location ||
		obj->location == NULL ||
		obj->location->location == player ||
		obj->location->location == player->location))
	{
		printf("You don't see any %s here.\n", noun);
		obj = NULL;
	}
	return obj;
}	
				
int listObjectsAtLocation(OBJECT *location)
{
	int count = 0;
	OBJECT *obj;
	for (obj = objs; obj < endOfObjs; obj++)
	{
		if (obj != player && obj->location == location)
		{
			if (count++ == 0)
			{
				printf("You see:\n");
			}
			printf("%s\n", obj->description);
		}
	}
	return count;
}


void executeLook(const char *noun)
{
	if (noun != NULL && strcmp(noun, "around") == 0)
	{
		printf("\nYou are in %s \n", player->location->description);
		listObjectsAtLocation(player->location);
	}
	else
	{
		printf("I don't understand what you want to see.\n");
	}
}


void executeGo(const char *noun)
{
	OBJECT *obj = getVisible("where you want to go", noun);
	if (obj == NULL)
	{
	 // handled by getVisible
	}
	else if (obj->location == NULL && obj != player->location)
	{
		printf("OK.\n");
		player->location = obj;
		executeLook("around");
	}
	else
	{
		printf("You can't get much closer than this.\n");
	}
}

bool parseAndExecute(char *input)
{
	char *verb = strtok(input, " \n");
	char *noun = strtok(NULL, " \n");
	if (verb != NULL)
	{
		if (strcmp(verb, "quit") == 0)
		{
			return false;
		}
		else if (strcmp(verb, "look") == 0)
		{
			executeLook(noun);
		}
		else if (strcmp(verb, "go") == 0)
		{
			executeGo(noun);	
		}
		else if (strcmp(verb, "help") == 0)
		{
			printf("In detail help will be appear here soon, current commands are look,go,quit and help.\n");
		}
		else 
		{
			printf("I don't know how to do that.\n");
		}
	}
	return true;
}


static char input[100] = "look around";

static bool getInput(void)
{
   printf("\n--> ");
   return fgets(input, sizeof input, stdin) != NULL;
}

int main()
{
   printf("\nWelcome to Test.\n");
   while (parseAndExecute(input) && getInput());
   printf("\nBye!\n");
   return 0;
}
