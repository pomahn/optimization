#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

static unsigned hash(const char* str);



int main()
{
    const char* string = "Pohoje vse ploxo\n";
    printf("%d", hash(string));
    return 0;
}

static unsigned hash(const char * str) 
{
	unsigned int hash = 0;
    while (*str)
    {
        hash += (*str);
        str++;
    }

	return hash;

}
