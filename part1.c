#include <stdio.h>
#define BLUE "\x1b[34m"
#define RES "\x1b[0m"
#include <windows.h>

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void printb(char *str){
	enableANSI();
	printf("%s%s%s",BLUE,str,RES);
}
void make_gamep(int n){
  for(int i=0;i<2*n+1;i++){
  	printf("  ");
  	for(int j=0;j<n;j++){
  		if(i%2==0) printb("+---");
  		else printb("|   ");
	}
	if(i%2==0) printb("+");
	else printb("|");
	
	printb("\n");
  }
}
int main(){
	int n;
	printf("Enter size of game border: ");
	scanf("%d",&n);
	printf("\n");
	make_gamep(n);
}




