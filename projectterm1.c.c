#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#define BLUE "\x1b[36m"
#define YELLO "\x1b[33m"
#define RED "\x1b[31m"
#define RES "\x1b[0m"

int blockx,blocko;
int ss;
int ow,xw;
int xo, yo, xx, yx;
int n;
int k;
int wallh[100][100]; //نگهداری وضعیت دیوار های قایم
int wallv[100][100]; //نگهداری وضعیت دیوار های افقی
int ewayO[100][100]; // نگهداری وضعیت خانه ها در بررسی راهخروج
int ewayX[100][100]; // نگهداری وضعیت خانه ها در بررسی راهخروج
//علامت برای اینکه راه خروج هست یانه
int foundO;
int foundX;
//نوع بازی
int typegame;

//برای رفتن به مختصات
void gotoxy(int x,int y){
	int t, m;
	t=x-1, m=y-1;
	COORD pos={t,m};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
//برای سیو بازی
int save(){
	FILE *saveg;
	if(typegame==2) saveg=fopen("lastgameTOW.dat","wb");
	if(typegame==3) saveg=fopen("lastgamePC.dat","wb");
	if(typegame==4) saveg=fopen("lastgamePT.dat","wb");
	if(!saveg){
		printf("game can't be saved!! try again later...");
		return;
	}
	fwrite(&typegame,sizeof(int),1,saveg);
	fwrite(&blocko,sizeof(int),1,saveg);
	fwrite(&blockx,sizeof(int),1,saveg);
	fwrite(&n,sizeof(int),1,saveg);
	fwrite(&xo,sizeof(int),1,saveg);
	fwrite(&yo,sizeof(int),1,saveg);
	fwrite(&xx,sizeof(int),1,saveg);
	fwrite(&yx,sizeof(int),1,saveg);
	fwrite(&k,sizeof(int),1,saveg);
	fwrite(&ow,sizeof(int),1,saveg);
	fwrite(&xw,sizeof(int),1,saveg);
	fwrite(wallh,sizeof(int),10000,saveg);
	fwrite(wallv,sizeof(int),10000,saveg);
	fclose(saveg);
	printf("game saved!!");
}
int load_lastgame(int o){
	FILE *save;
	
	if(o==1){
		save=fopen("lastgamePC.dat","rb");
	}
	if(o==2){
		save=fopen("lastgameTOW.dat","rb");
	}
	if(o==3){
		save=fopen("lastgamePT.dat","rb");
	}
	
	if(!save){
		printf("game can't be opend!! try again later...");
		return;
	}
	fread(&typegame,sizeof(int),1,save);
	fread(&blocko,sizeof(int),1,save);
	fread(&blockx,sizeof(int),1,save);
	fread(&n,sizeof(int),1,save);
	fread(&xo,sizeof(int),1,save);
	fread(&yo,sizeof(int),1,save);
	fread(&xx,sizeof(int),1,save);
	fread(&yx,sizeof(int),1,save);
	fread(&k,sizeof(int),1,save);
	fread(&ow,sizeof(int),1,save);
	fread(&xw,sizeof(int),1,save);
	fread(&wallh,sizeof(int),10000,save);
	fread(&wallv,sizeof(int),10000,save);
	fclose(save);
}
//چهار تابع پایین برای حرکت کاراکتر در محیط
void goup(int x, int y){
	gotoxy(x,y);
	printf(" ");
	gotoxy(x,y-2);
	if(k==-1) printf("O\a");
	else printf("X\a");
}
void godown(int x, int y){
	gotoxy(x,y);
	printf(" ");
	gotoxy(x,y+2);
	if(k==-1) printf("O\a");
	else printf("X\a");
}
void goleft(int x, int y){
	gotoxy(x,y);
	printf(" ");
	gotoxy(x-4,y);
	if(k==-1) printf("O\a");
	else printf("X\a");
}
void goright(int x, int y){
	gotoxy(x,y);
	printf(" ");
	gotoxy(x+4,y); 
	if(k==-1) printf("O\a");
	else printf("X\a");
}

//برای ساختن مپ بازی
void make_gamep(int n){
	printf("O walls: %d          \a",ow,blocko);
	if(k==-1){
		if(blockx!=0) printf("O\n");
		else printf("X\n");
	}else{
		if(blocko!=0) printf("X\n");
		else printf("O\n");
	}
	printf("X walls: %d          ",xw,blockx);
	printf("size of game is: %d\n",n);
//	ساخت بوردر بازی
	for(int i=0;i<2*n+1;i++){
	 	for(int j=0;j<n;j++){
	 		if(i%2==0) printb("+---");
	 		else{
				printb("|   ");
			}
		}
		if(i%2==0) printb("+");
		else printb("|");
		
		printb("\n");
	}
//	ساخت دیوار های قائم
	for(int i=1;i<n+3;i++){
		for(int j=1;j<n+2;j++){
			if(wallh[i][j]==2){
				gotoxy(4*i-3,2*j+2);
				printf("%s8%s",YELLO,RES);
			}
		}
	}
//	ساخت دیوار های افقی
	for(int i=1;i<n+3;i++){
		for(int j=1;j<n+2;j++){
			if(wallv[i][j]==2){
				gotoxy(4*j-2,2*i+1);
				printf("%sooo%s",YELLO,RES);
			}
		}
	}
}
//برای طلسم کردن یا پاداش x 
void telesmpax(){
	int top=rand()%(2)+1;
	if(top==1){
		int t=rand()%(3)+1;
		if(t==1){
			srand(time(NULL));
			int kahesh=rand()%(3)+1;
			if(kahesh==1) xw-=2;
			if(kahesh==2) xw-=3;
			if(kahesh==3) xw-=5;
			if(xw<0) xw=0;
		}
		if(t==2){
			for(int j=1;j<n+2;j++){
				for(int i=1;i<n+2;i++){
					wallv[j][i]=0;
					wallh[j][i]=0;
				}
			}
			for(int i=1;i<=n;i++){
				wallh[1][i]=2;
				wallh[n+1][i]=2;
				wallv[1][i]=2;
				wallv[n+1][i]=2;
			}
		}
		if(t==3){
			
			blockx+=2;
		}
	}else{
		int p=rand()%(2)+1;
		if(p==1){
			
			
			int afzayesh=rand()%(3)+1;
			if(afzayesh==1) xw+=2;
			if(afzayesh==2) xw+=3;
			if(afzayesh==3) xw+=5;
		}
		if(p==2){
			int afza_kahesh=rand()%(2)+1;
			if(afza_kahesh==1){
				
				ow-=1;
				xw+=1;
				if(ow<0) ow=0;
			}
			if(afza_kahesh==2){
				
				ow-=2;
				xw+=2;
				if(ow<0) ow=0;
			}
		}
	}
}
//برای طلسم کردن یا پاداش o
//توضیحات تابع اولی برای تابع دوم هم هست
void telesmpao(){
	
	int top=rand()%(2)+1;
	if(top==1){
//		طبسم
		int t=rand()%(3)+1;
		if(t==1){
//			کم کردن دیوار
			int kahesh=rand()%(3)+1;
			if(kahesh==1) ow-=2;
			if(kahesh==2) ow-=3;
			if(kahesh==3) ow-=5;
			if(ow<0) ow=0;
		}
		if(t==2){
//			از بین بردن دیوار ها
			for(int j=1;j<n+2;j++){
				for(int i=1;i<n+2;i++){
					wallv[j][i]=0;
					wallh[j][i]=0;
				}
			}
//			قرار دادن دیوار های مرزی
			for(int i=1;i<=n;i++){
				wallh[1][i]=2;
				wallh[n+1][i]=2;
				wallv[1][i]=2;
				wallv[n+1][i]=2;
			}
		}
		if(t==3){
//			بلاک کردن بازیکن
			blocko+=2;
		}
	}else{
//		پاداش
		int p=rand()%(2)+1;
		if(p==1){
//			افزایش دیوار
			srand(time(NULL));
			int afzayesh=rand()%(3)+1;
			if(afzayesh==1) ow+=2;
			if(afzayesh==2) ow+=3;
			if(afzayesh==3) ow+=5;
		}
		if(p==2){
//		افزایش دیوار خود و کاهش دیوار حریف
			int afza_kahesh=rand()%(2)+1;
			if(afza_kahesh==1){
				xw-=1;
				ow+=1;
				if(xw<0) xw=0;
			}
			if(afza_kahesh==2){
				xw-=2;
				ow+=2;
				if(xw<0) xw=0;
			}
		}
	}
}


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
//ابن تابع چک میکند دیوار میشه گذاشت یا نه و اگه میشه در ارایه های دیوار ذخیره کند
int chekputwall(int x,int y,char c){
//	تبدیل به مختصات محیط
	int x1=4*x-3;
	int y1=2*y+1;
	if(c=='h'){
		if(y1==3||y1==2*n+3){
			return 0;
		}
		if((wallv[y][x]==2&&wallv[y][x-1]==2)||wallh[x][y-1]==2||wallh[x][y]==2) return 0;
		
		wallh[x][y-1]=2;
		wallh[x][y]=2;
		return 1;
	}
	else{
		if(x1==1||x1==4*n+1){
			return 0;
		}
		if((wallh[x][y]==2&&wallh[x][y-1]==2)||wallv[y][x-1]==2||wallv[y][x]==2) return 0;
		
		wallv[y][x-1]=2;
		wallv[y][x]=2;
		return 1;
	}
}
//دو تابع پایین چک میکنند که دیوار  تمام راه های خروجی رو نبنده
void chek_exit_wayO(int x,int y){
	if(foundO!=1){
		if(ewayO[x][y]==1||ewayO[x][y]==3){
			return;
		}
		
		if(y==n){
			foundO=1;
			return;
		}
		ewayO[x][y]=1;
//		اگر بین دوخانه دیوار نبود تابع خودش رو فراخوانی کنه
		if(wallh[x+1][y]!=2){
			chek_exit_wayO(x+1,y);
		}
		if(wallh[x][y]!=2){
			chek_exit_wayO(x-1,y);
		}
		if(wallv[y+1][x]!=2){
			chek_exit_wayO(x,y+1);
		}
		if(wallv[y][x]!=2){
			chek_exit_wayO(x,y-1);
		}
		ewayO[x][y]=0;
		return;
	}
	else{
		return;
	}
}
void chek_exit_wayX(int x,int y){
	if(foundX!=1){
		if(ewayX[x][y]==1){
			return;
		}
		if(y==1){
			foundX=1;
			return;
		}
		
		ewayX[x][y]=1;
		
		if(wallh[x+1][y]!=2){
			chek_exit_wayX(x+1,y);
		}
		if(wallh[x][y]!=2){
			chek_exit_wayX(x-1,y);
		}
		if(wallv[y][x]!=2){
			chek_exit_wayX(x,y-1);
		}
		if(wallv[y+1][x]!=2){
			chek_exit_wayX(x,y+1);
		}
		ewayX[x][y]=0;
		return;
	}
	else{
		return;
	}
}

//وقتی دیوار گذاشته شد و راهی نبود این تابع دیوار رو برمیداره
void deletewall(int a,int b,char c){
	if(c=='h'){
		wallh[a][b-1]=0;
		wallh[a][b]=0;
	}
	else{
		wallv[b][a-1]=0;
		wallv[b][a]=0;
	}
}
//دو تابع پایین ارایه مربوط به نگهداری شرابط خانه ها در بررسی راه خروج رو برای استفاده دوباره صفر مینه
void makeewayXzero(){
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			ewayX[i][j]=0;
		}
	}
}
void makeewayOzero(){
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			ewayO[i][j]=0;
		}
	}
}
//دو تابع پایین سوالاتی در مورد بازی مورد نظر کاربر میپرسند
void describtion(){
	enableANSI();
	printf("%sif you wanna go:%s\n",YELLO,RES);
	printf("%sup   enter 'u'\n%s",YELLO,RES);
	printf("%sdown   enter 'd'\n%s",YELLO,RES);
	printf("%sright   enter 'r'\n%s",YELLO,RES);
	printf("%sleft   enter 'l'\n%s",YELLO,RES);
	printf("%sif you wanna put wall enter 'w'\n%s",YELLO,RES);
	printf("%sif you wanna save game enter 's'\n\a%s",
	YELLO,RES);
	printb("if you wanna :\n");
	printb("play your last saved game enter '1'\n");
	printb("play a new game enter '2'\n\a");
	printb("play a whit computer enter '3'\n");
	printb("play ptgame enter '4'\n");
}
void describtion2(){
	printb("play your last saved game enter (PC)'1'\n");
	printb("play your last saved game enter (TOW PLAYER)'2'\n");
	printb("play your last saved game enter (TELESM-PADASH)'3'\n");
}
//تابع زیر کار دریافت اطلاعات و اماده سازی اطلاعات برای تابع ساخت نقشه رو داره 
void question(){
	int wall;
	printf("Enter size of game border: ");
	scanf("%d",&n);
				
	printf("Enter the wall that you needes: ");
	scanf("%d",&wall);
	printf("\n");
				
					
	for(int i=1;i<=n;i++){
		wallh[1][i]=2;
		wallh[n+1][i]=2;
		wallv[1][i]=2;
		wallv[n+1][i]=2;
	}
	xw=ow=wall;
	system("cls");
				
	make_gamep(n);
					
	gotoxy(n/2*4+3,4);
	printf("O");
	gotoxy(n/2*4+3,2*n+2);
	printf("X");
	yo=4; xo=n/2*4+3; xx=n/2*4+3; yx=2*n+2;
	gotoxy(1,2*n+4);
					
	k=-1;
}


//تابع بازی دونفره
void towplayer(){
	char movedir;
	while(1){
		scanf(" %c",&movedir);
		if(movedir!='l'&&movedir!='u'&&movedir!='d'&&movedir!='r'&&movedir!='w'&&movedir!='s'){
			printf("%stry again!\n%s",RED,RES);
			scanf("%c",&movedir);
			continue;
		}
		if(k==-1){
			if(movedir=='u'){
				if(xo>1&&xo<4*n+1&&yo-2>2&&yo-2<2*n+3&&wallv[(yo-2)/2][(xo+1)/4]!=2){
					if(xx==xo&&yx==yo-2){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
	
						make_gamep(n);
						goup(xo,yo);
						yo-=2;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='d'){
				if(xo>1&&xo<4*n+1&&yo+2>4&&yo+2<2*n+3&&wallv[yo/2][(xo+1)/4]!=2){
					if(xo==xx&&yo+2==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						godown(xo,yo);
						yo+=2;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				if(yo==2*n+2){
					printf("O is winer!!");
					break;
				}
				
			}
			if(movedir=='r'){
				if(xo+4>1&&xo+4<4*n+1&&yo>=4&&yo<2*n+3&&wallh[(xo+5)/4][(yo-2)/2]!=2){
					if(xo+4==xx&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goright(xo,yo);
						xo+=4;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='l'){
				if(xo-4>1&&xo-4<4*n+1&&yo>=4&&yo<2*n+3&&wallh[(xo+1)/4][(yo-2)/2]!=2){
					if(xo-4==xx&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goleft(xo,yo);
						xo-=4;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='w'){
				if(ow==0){
					printf("no enough wall!! %stry again%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				int a,b;
				char c;
				printf("\n");
				scanf("%d %d %c",&a,&b,&c);
				if(c!='h'&&c!='v'){
					printf("%stry again enter your move%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				} 
				if(chekputwall(a,b,c)){
					foundX=0;
					chek_exit_wayX((xx+1)/4,(yx-2)/2);
					if(foundX){
						ow-=1;
						system("cls");
						
						make_gamep(n);
						gotoxy(xo,yo);
						printf("O");
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
					else{
						deletewall(a,b,c);
						printf("there is no way%s try again%s\n",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				makeewayXzero();
			}
			if(movedir=='s'){
				save();
				break;
			}
		}
		
		
		else{
			if(movedir=='u'){
				if(xx>1&&xx<4*n+1&&yx-2>2&&yx-2<2*n+3&&wallv[(yx-2)/2][(xx+1)/4]!=2){
					if(yx-2==yo&&xx==xo){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goup(xx,yx);
						yx-=2;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				if(yx==4){
					printf("X is winer!!");
					break;
				}
			
			}
			if(movedir=='d'){
				if(xx>1&&xx<4*n+1&&yx+2>4&&yx+2<2*n+3&&wallv[yx/2][(xx+1)/4]!=2){
					if(yx+2==yo&&xx==xo){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						godown(xx,yx);
						yx+=2;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				
			}
			if(movedir=='r'){
				if(xx+4>1&&xx+4<4*n+1&&yx>=4&&yx<2*n+3&&wallh[(xx+5)/4][(yx-2)/2]!=2){
					if(xx+4==xo&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goright(xx,yx);
						xx+=4;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='l'){
				if(xx-4>1&&xx-4<4*n+1&&yx>=4&&yx<2*n+3&&wallh[(xx+1)/4][(yx-2)/2]!=2){
					if(xx-4==xo&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
					
						make_gamep(n);
						goleft(xx,yx);
						xx-=4;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			
			if(movedir=='w'){
				if(xw==0){
					printf("no enough wall!! %stry again%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				int a,b;
				char c;
				printf("\n");
				scanf("%d %d %c",&a,&b,&c);
				if(c!='h'&&c!='v'){
					printf("%stry again enter your move%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				} 
				if(chekputwall(a,b,c)){
					foundO=0;
					chek_exit_wayO((xo+1)/4,(yo-2)/2);
					if(foundO){
						xw-=1;
						system("cls");
						
						make_gamep(n);
						gotoxy(xo,yo);
						printf("O");
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
					else{
						deletewall(a,b,c);
						printf("there is no way%s try again%s\n",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				makeewayOzero();
			}
			if(movedir=='s'){
				save();
				break;
			}
		}
	k*=-1;
	}
}
//تابع بازی با کامپیوتر
void playwithPC(){
	char movedir;
	while(1){
		if(k==-1){
			scanf(" %c",&movedir);
			if(movedir!='l'&&movedir!='u'&&movedir!='d'&&movedir!='r'&&movedir!='w'&&movedir!='s'){
				printf("%stry again!\n%s",RED,RES);
				scanf("%c",&movedir);
				continue;
			}
			if(movedir=='u'){
				if(xo>1&&xo<4*n+1&&yo-2>2&&yo-2<2*n+3&&wallv[(yo-2)/2][(xo+1)/4]!=2){
					if(xx==xo&&yx==yo-2){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
	
						make_gamep(n);
						goup(xo,yo);
						yo-=2;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='d'){
				if(xo>1&&xo<4*n+1&&yo+2>4&&yo+2<2*n+3&&wallv[yo/2][(xo+1)/4]!=2){
					if(xo==xx&&yo+2==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						godown(xo,yo);
						yo+=2;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				if(yo==2*n+2){
					printf("O is winer!!");
					break;
				}
				
			}
			if(movedir=='r'){
				if(xo+4>1&&xo+4<4*n+1&&yo>=4&&yo<2*n+3&&wallh[(xo+5)/4][(yo-2)/2]!=2){
					if(xo+4==xx&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goright(xo,yo);
						xo+=4;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='l'){
				if(xo-4>1&&xo-4<4*n+1&&yo>=4&&yo<2*n+3&&wallh[(xo+1)/4][(yo-2)/2]!=2){
					if(xo-4==xx&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goleft(xo,yo);
						xo-=4;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='w'){
				if(ow==0){
					printf("no enough wall!! %stry again%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				int a,b;
				char c;
				printf("\n");
				scanf("%d %d %c",&a,&b,&c);
				if(c!='h'&&c!='v'){
					printf("%stry again enter your move%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				} 
				if(chekputwall(a,b,c)){
					foundX=0;
					chek_exit_wayX((xx+1)/4,(yx-2)/2);
					if(foundX){
						ow-=1;
						system("cls");
						
						make_gamep(n);
						gotoxy(xo,yo);
						printf("O");
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
					else{
						deletewall(a,b,c);
						printf("there is no way%s try again%s\n",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				makeewayXzero();
			}
			if(movedir=='s'){
				save();
				break;
			}
		}
		
		
		else{
			
			int movedirpc= rand()%(14)+1;
			// for up
			if(1<=movedirpc&&movedirpc<=7){
				if(xx>1&&xx<4*n+1&&yx-2>2&&yx-2<2*n+3&&wallv[(yx-2)/2][(xx+1)/4]!=2){
					if(yx-2==yo&&xx==xo){
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goup(xx,yx);
						yx-=2;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					
					continue;
				}
				if(yx==4){
					printf("X is winer!!");
					break;
				}
			
			}
			//2 for down
			if(movedirpc==14){
				if(xx>1&&xx<4*n+1&&yx+2>4&&yx+2<2*n+3&&wallv[yx/2][(xx+1)/4]!=2){
					if(yx+2==yo&&xx==xo){
						
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						godown(xx,yx);
						yx+=2;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					
					continue;
				}
				
			}
			//3 for right
			if(movedirpc==10||movedirpc==11){
				if(xx+4>1&&xx+4<4*n+1&&yx>=4&&yx<2*n+3&&wallh[(xx+5)/4][(yx-2)/2]!=2){
					if(xx+4==xo&&yo==yx){
						
						continue;
					}
					else{
						system("cls");
						
						make_gamep(n);
						goright(xx,yx);
						xx+=4;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					
					continue;
				}
			
			}
			//4 for left
			if(movedirpc==12||movedirpc==13){
				if(xx-4>1&&xx-4<4*n+1&&yx>=4&&yx<2*n+3&&wallh[(xx+1)/4][(yx-2)/2]!=2){
					if(xx-4==xo&&yo==yx){
						continue;
					}
					else{
						system("cls");
					
						make_gamep(n);
						goleft(xx,yx);
						xx-=4;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					
					continue;
				}
			
			}
			// 5 for wall
			if(movedirpc>=8&&movedirpc<=9){
				if(xw==0){
					
					continue;
				}
				int chekwall=1;
				while(chekwall){
					int  typewall=rand()%(2)+1;
					char c;
					int a, b;
					if(typewall==1) c='v';
					else c='h';
					int xowall=rand()%(4)+1;
					
					
					if(xowall==1){
						a=(xo+1)/4;
						b=(yo-2)/2;
					}else{
						if(xowall==2){
							a=(xo+1)/4;
							b=((yo-2)/2)+1;
						}else{
							if(xowall==3){
								a=((xo+1)/4)+1;
								b=(yo-2)/2;
							}
							else{
								a=((xo+1)/4)+1;
								b=((yo-2)/2)+1;
							}
						}
					}
					
					if(chekputwall(a,b,c)){
						foundO=0;
						chek_exit_wayO((xo+1)/4,(yo-2)/2);
						if(foundO){
							xw-=1;
							system("cls");
							
							make_gamep(n);
							gotoxy(xo,yo);
							printf("O");
							gotoxy(xx,yx);
							printf("X");
							gotoxy(1,2*n+4);
							chekwall=0;
						}
						else{
							deletewall(a,b,c);
							continue;
						}
					}
					else{
						continue;
					}
					makeewayOzero();
					}	
				}
			}
		k*=-1;
	}
}
//تابع بازی طلسم و پاداش
void playptgame(){
//	برای شمارش تعداد ممنوعیت هر بازیکن
	blockx=0;
	blocko=0;
	char movedir;
	while(1){
		if(k==-1){
			if(blocko!=0){
//				برای وقتی که بازیکن بلاکه
				k*=-1;
				blocko-=1;
				telesmpao();
				continue;
			}
			scanf(" %c",&movedir);
			if(movedir!='l'&&movedir!='u'&&movedir!='d'&&movedir!='r'&&movedir!='w'&&movedir!='s'){
				printf("%stry again!\n%s",RED,RES);
				scanf("%c",&movedir);
				continue;
			}
			if(movedir=='u'){
				if(xo>1&&xo<4*n+1&&yo-2>2&&yo-2<2*n+3&&wallv[(yo-2)/2][(xo+1)/4]!=2){
					if(xx==xo&&yx==yo-2){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpao();
						make_gamep(n);
						goup(xo,yo);
						yo-=2;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='d'){
				if(xo>1&&xo<4*n+1&&yo+2>4&&yo+2<2*n+3&&wallv[yo/2][(xo+1)/4]!=2){
					if(xo==xx&&yo+2==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpao();
						make_gamep(n);
						godown(xo,yo);
						yo+=2;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
//				وقتی بازیکن به اخر برسه اعلام میکنه برنده شده
				if(yo==2*n+2){
					printf("O is winer!!");
					break;
				}
				
			}
			if(movedir=='r'){
				if(xo+4>1&&xo+4<4*n+1&&yo>=4&&yo<2*n+3&&wallh[(xo+5)/4][(yo-2)/2]!=2){
					if(xo+4==xx&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpao();
						make_gamep(n);
						goright(xo,yo);
						xo+=4;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='l'){
				if(xo-4>1&&xo-4<4*n+1&&yo>=4&&yo<2*n+3&&wallh[(xo+1)/4][(yo-2)/2]!=2){
					if(xo-4==xx&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpao();
						make_gamep(n);
						goleft(xo,yo);
						xo-=4;
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='w'){
				if(ow==0){
					printf("no enough wall!! %stry again%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				int a,b;
				char c;
				printf("\n");
				scanf("%d %d %c",&a,&b,&c);
				if(c!='h'&&c!='v'){
					printf("%stry again enter your move%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				} 
				if(chekputwall(a,b,c)){
					foundX=0;
					chek_exit_wayX((xx+1)/4,(yx-2)/2);
					if(foundX){
						ow-=1;
						system("cls");
						telesmpao();
						make_gamep(n);
						gotoxy(xo,yo);
						printf("O");
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
					else{
						deletewall(a,b,c);
						printf("there is no way%s try again%s\n",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				makeewayXzero();
			}
			if(movedir=='s'){
				save();
				break;
			}
		}
		
		
		else{
			if(blockx!=0){
				k*=-1;
				blockx-=1;
				telesmpax();
				continue;
			}
			scanf(" %c",&movedir);
			if(movedir!='l'&&movedir!='u'&&movedir!='d'&&movedir!='r'&&movedir!='w'&&movedir!='s'){
				printf("%stry again!\n%s",RED,RES);
				scanf("%c",&movedir);
				continue;
			}
			if(movedir=='u'){
				if(xx>1&&xx<4*n+1&&yx-2>2&&yx-2<2*n+3&&wallv[(yx-2)/2][(xx+1)/4]!=2){
					if(yx-2==yo&&xx==xo){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpax();
						make_gamep(n);
						goup(xx,yx);
						yx-=2;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				if(yx==4){
					printf("X is winer!!");
					break;
				}
			
			}
			if(movedir=='d'){
				if(xx>1&&xx<4*n+1&&yx+2>4&&yx+2<2*n+3&&wallv[yx/2][(xx+1)/4]!=2){
					if(yx+2==yo&&xx==xo){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpax();
						make_gamep(n);
						godown(xx,yx);
						yx+=2;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				
			}
			if(movedir=='r'){
				if(xx+4>1&&xx+4<4*n+1&&yx>=4&&yx<2*n+3&&wallh[(xx+5)/4][(yx-2)/2]!=2){
					if(xx+4==xo&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpax();
						make_gamep(n);
						goright(xx,yx);
						xx+=4;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}
			if(movedir=='l'){
				if(xx-4>1&&xx-4<4*n+1&&yx>=4&&yx<2*n+3&&wallh[(xx+1)/4][(yx-2)/2]!=2){
					if(xx-4==xo&&yo==yx){
						printf("%stry again!\n%s",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
					else{
						system("cls");
						telesmpax();
						make_gamep(n);
						goleft(xx,yx);
						xx-=4;
						gotoxy(xo,yo);
						printf("O");
						gotoxy(1,2*n+4);
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
			
			}			
			if(movedir=='w'){
				if(xw==0){
					printf("no enough wall!! %stry again%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				int a,b;
				char c;
				printf("\n");
				scanf("%d %d %c",&a,&b,&c);
				if(c!='h'&&c!='v'){
					printf("%stry again enter your move%s\n",RED,RES);
					scanf("%c",&movedir);
					continue;
				} 
				if(chekputwall(a,b,c)){
					foundO=0;
					chek_exit_wayO((xo+1)/4,(yo-2)/2);
					if(foundO){
						xw-=1;
						system("cls");
						telesmpax();
						make_gamep(n);
						gotoxy(xo,yo);
						printf("O");
						gotoxy(xx,yx);
						printf("X");
						gotoxy(1,2*n+4);
					}
					else{
						deletewall(a,b,c);
						printf("there is no way%s try again%s\n",RED,RES);
						scanf("%c",&movedir);
						continue;
					}
				}
				else{
					printf("%stry again!\n%s",RED,RES);
					scanf("%c",&movedir);
					continue;
				}
				makeewayOzero();
			}
			if(movedir=='s'){
				save();
				break;
			}
		}
	k*=-1;
	}
}


int main(){
	srand(time(NULL));
	int wall;
	describtion();
	scanf(" %d",&typegame);
	if(typegame!=2&&typegame!=1&&typegame!=3&&typegame!=4){
		printf("%stry again%s\n",RED,RES);
		scanf(" %d",&typegame);
	}
	if(typegame==2){
		question();
		towplayer();
	}
	else{
		if(typegame==1){
			system("cls");
			describtion2();
//			برای مشخص کردن لود نوع بازی سیو شده
			int typesaved;
//			تا وقتی که عدد درست وارد نکنه کار نمیکنه
			while(1){
				scanf("%d",&typesaved);
				if(typesaved>=1&&typesaved<=3) break;
				else{
					printf("%stry again!!%s",RED,RES);
					continue;
				}
			}
			
			
			
			load_lastgame(typesaved);
			
			system("cls");
			make_gamep(n);
			
			gotoxy(xo,yo);
			printf("O");
			gotoxy(xx,yx);
			printf("X");
			gotoxy(1,2*n+4);
			if(typesaved==1) playwithPC();
			if(typesaved==2) towplayer();
			if(typesaved==3) playptgame();
		}
		else{
			if(typegame==4){
				question();
				playptgame();
			}
			else{
				question();
				playwithPC();
			}
		}
	} 
}