#include "Common/Services/ConsoleService/ConsoleService.h"

int main(void)
{
	ConsoleService_Init();

	printf("You wrote: %s\n\r", "Hallo");
	printf("You wrote: %d\n\r", 239);
	printf("You wrote: %x\n\r", 239);
	//scanf("%d", i);
	
    while(1) 
    {
    }
}