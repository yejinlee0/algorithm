#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct max_subrect{
	int max,row,col; // max : 최대값, row : 행, col : 열
	int **rect; // 입력으로 받은 2차원 배열의 정보가 저장된다.
}max_subrect;

max_subrect msr;

void open_input(char *inname); // input 파일을 읽는 함수

void algorithm_1(); // O(n^6) 알고리즘
void algorithm_2(); // O(n^4) 알고리즘
void algorithm_3(); // O(n^3) 알고리즘

int main(int argc, char* argv[]){
	clock_t start_t; // 알고리즘 시작 시간을 저장한다.
	FILE *out;
	char outname[100] = "result_"; // output file 이름을 저장한다.
	int algonum = argv[2][0] - '0'; // 입력으로 받은 알고리즘의 번호를 저장한다.
	strcat(outname,argv[1]);
	open_input(argv[1]); // input 파일을 읽는 함수를 호출한다.

	start_t = clock();	

	switch(algonum){
		case 1: algorithm_1();break;
		case 2: algorithm_2();break;
		case 3: algorithm_3();break;
		default: printf("Invalid algorithm number.\n");break; // 알고리즘 번호가 적절하지 않은 경우 오류메세지를 출력한다.
	}
	
	clock_t finish_t = clock(); // 알고리즘이 끝나는 시간을 저장한다.

	out=fopen(outname,"w"); // output file 을 연다.
	fprintf(out,"%s\n",argv[1]); // input file name를 출력한다.
	fprintf(out,"%d\n",algonum); // 알고리즘 번호를 출력한다.
	fprintf(out,"%d\n%d\n",msr.row,msr.col); // 행과 열을 출력한다.
	fprintf(out,"%d\n%f\n",msr.max,((double)(finish_t-start_t)*1000)/CLOCKS_PER_SEC); // 알고리즘 수행 시간을 출력한다.
	fclose(out);

	return 0;
}

void open_input(char *inname){
	FILE *in=fopen(inname,"r"); // input file을 연다.
	int i,j;
	fscanf(in,"%d %d", &msr.row, &msr.col); // input file에서 행과 열에 대한 정보를 읽는다.
	msr.rect=(int**)malloc((msr.row)*sizeof(int*)); // input file에 저장된 2D array의 정보를 읽기 위해 알맞은 size의 공간을 할당한다.
	for(i=0;i<msr.row;i++){
		msr.rect[i]=(int*)malloc((msr.col)*sizeof(int));
	}

	for(i=0;i<msr.row;i++){
		for(j=0;j<msr.col;j++){
			fscanf(in,"%d",&msr.rect[i][j]); // input file의 2D array 정보를 읽는다.
		}
	}
	msr.max=msr.rect[0][0]; // 우선, 2D array의 첫 번째 원소를 최대 값으로 저장한다.
	fclose(in);
}

void algorithm_1(){
	int x,y,z,w,i,j,sum=0; // sum은 각각의 경우에 대한 합을 저장한다.
	for(x=0;x<msr.row;x++){ // x,y는 각각 row의 시작점과 끝점에 해당한다.
		for(y=x;y<msr.row;y++){
			for(z=0;z<msr.col;z++){ // z,w는 각각 column의 시작점과 끝점에 해당한다.
				for(w=z;w<msr.col;w++){
					sum=0; // 합을 저장할 변수를 초기화한다.
					for(i=x;i<=y;i++){
						for(j=z;j<=w;j++){
							sum+=msr.rect[i][j]; // 정해진 범위의 row와 column에 대한 합을 계산한다.
						}
					}
					if(sum>msr.max){ // 계산된 합이 최대값보다 크면 최대값에 저장한다.
						msr.max=sum;
					}
				}
			}
		}

	}

}

void algorithm_2(){
	int x,y,z,w,i,j,sum=0;
	for(x=0;x<msr.row;x++){ // x는 row의 시작점에 해당한다.
		for(z=0;z<msr.col;z++){ // z는 column의 끝점에 해당한다.
			int *Save_sum=(int*)calloc(msr.col,sizeof(int)); // Save_sum 배열은 각 index에 해당되는 column의 합을 저장한다. 0으로 초기화하여 할당한다.
			for(y=x;y<msr.row;y++){
				for(w=z;w<msr.col;w++){
					Save_sum[w]+=msr.rect[y][w]; // 정해진 시작점부터 해당되는 값을 Save_sum 의 알맞은 위치에 더한다.
				}
				sum=0; // 합을 저장할 변수를 초기화한다.
				for(i=z;i<msr.col;i++){
					sum+=Save_sum[i]; // column이 증가하는 방향으로 Save_sum 의 값을 sum에 더한다.
					if(sum>msr.max){ // 계산된 합이 최대값보다 크면 최대값에 저장한다.
						msr.max=sum;			
					}
				}

			}
			free(Save_sum);
		}
	}

}

void algorithm_3(){
	int left,right,i,j,sum; // left,right는 각각 column에서 왼쪽과 오른쪽 위치를 저장한다.
	for(left=0;left<msr.col;left++){
		int *Row_sum=(int*)calloc(msr.row,sizeof(int)); // Row_sum 배열은 각 index에 해당되는 row의 합을 저장한다. 0으로 초기화하여 할당한다.
		for(right=left;right<msr.col;right++){
			for(i=0;i<msr.row;i++){
				Row_sum[i]+=msr.rect[i][right];
			}
		
			sum=0; // 합을 저장할 변수를 초기화한다.
			for(j=0;j<msr.row;j++){
				sum+=Row_sum[j]; // row가 증가하는 방향으로 해당되는 값을 더한다.
				if(sum>msr.max){ // 계산된 합이 최대값보다 크면 최대값에 저장한다.
					msr.max=sum;
				}
				if(sum<0){
					sum=0;
				}
			}
		}
		free(Row_sum);
	}

}
