#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define swap(a,b) {int t; t=a; a=b; b=t;}

int listsize;
int* list;

void open_input(char *inname, int algonumber);

int partition(int left, int right);
void adjust(int root, int n);
void adjust2(int i, int size, int begin){ //시작 원소의 인덱스와 주어진 크기에 대하여 heap을 생성하고 정렬한다.
	int child, temp = list[begin+i-1];

	while(i <= size/2){
		child =2*i;

		if(child<size && list[begin+child-1]<list[begin+child])child++;
		if(temp >= list[begin+child-1])break;
		list[begin+i-1] = list[begin+child-1];
		i = child;
	}

	list[begin+i-1] = temp;
}
void insertionsort(int left, int right){ //입력되는 범위 내의 원소를 오름차순으로 삽입 정렬한다.
	int i, j, temp;
	for(i=left+1;i<=right;i++){
		temp = list[i];
		j=i-1;
		while(list[j]>temp&&j>=left){ //현재 위치보다 왼쪽에 있는 원소들과 비교한다.
			list[j+1]=list[j];
			j--;
		}
		list[j+1] = temp;
	}
}

int dpartition(int low, int high, int *lp){ //left pivot과 right pivot을 구해주는 함수이다.
	if(list[low]>list[high])swap(list[low],list[high]); //left pivot<right pivot을 만족하도록 한다.

	int lp_idx = low+1, rp_idx = high-1, i;
	int p = list[low], q = list[high];

	//배열의 원소들을 left pivot보다 작은 그룹, right pivot보다 큰 그룹, 그 사이에 있는 그룹으로 나눈다.

	for(i=low+1;i<=rp_idx;i++){
		if(list[i]<p){
			swap(list[i],list[lp_idx]);
			lp_idx++;
		}
		else if(list[i] >= q){
			while(list[rp_idx]>q && i<rp_idx)rp_idx--;
			swap(list[i],list[rp_idx]);
			rp_idx--;
			if(list[i]<p){
				swap(list[i],list[lp_idx]);
				lp_idx++;
			}
		}
	}

	lp_idx--;
	rp_idx++;
	
	swap(list[low],list[lp_idx]);
	swap(list[high],list[rp_idx]);
	
	//left pivot과 right pivot의 index를 반환한다.
	
	*lp = lp_idx;
	
	return rp_idx;
}

void heapsort(int begin, int end){
	//adjust2 함수를 이용해서 힙 정렬을 수행하는 함수이다.
	int size = end - begin, i;

	for(i=size/2;i>=1;i--) adjust2(i,size,begin);

	for(i=size;i>=1;i--){
		swap(list[begin],list[begin+i]);
		adjust2(1,i,begin);
	}
}

void algorithm_1();
void algorithm_2(int left, int right);
void algorithm_3();
void algorithm_4(int count, int low, int high){
	// low<high인 범위 내에서만 실행되도록 한다.
	if(low>=high) return;
	
	//입력 크기가 45보다 작거나 같은 경우에는 삽입정렬을 이용한다.
	if(count == 0){
		int size = high - low + 1;
		if(size <= 45){
			insertionsort(low, high);
			return;
		}
		else{
			heapsort(low,high);
			return;
		}
	}

	else{
		int lp;
		//dpartition 함수를 이용하여 left pivot과 right pivot의 index를 구하고 이를 기준으로 나눈 3개의 그룹에 대해 각각 재귀적으로 함수를 호출한다.
		int rp = dpartition(low,high,&lp);
		algorithm_4(count-1,low,lp-1);
		algorithm_4(count-1,lp+1,rp-1);
		algorithm_4(count-1,rp+1,high);
	}
	
}

int main(int argc, char* argv[]){
	clock_t start, finish;
	FILE *out;
	char outname[100] = "result_";
	int algonum = argv[2][0] - '0';
	char underbar = '_';
	int i;

	//output file 이름을 양식에 맞게 만들어서 변수 outname에 저장한다.
	strcat(outname,&argv[2][0]);
	strcat(outname,"_");
	strcat(outname,argv[1]);
	
	open_input(argv[1],algonum);
	int depthlimit = 2*log(listsize);
	//시작 시간을 잰다.
	start = clock();

	//입력 받은 알고리즘 번호에 알맞는 함수를 실행한다.
	switch(algonum){
		case 1: algorithm_1();break;
		case 2: algorithm_2(1,listsize);break;
		case 3: algorithm_3();break;
		case 4: if(listsize<=45)algorithm_1(); else algorithm_4(depthlimit,1,listsize); break;
		default: printf("Invalid algorithm number\n");break;
	}
	//끝나는 시간을 잰다.
	finish = clock();

	//output file을 양식에 맞게 작성한다. 이때 시간은 초단위로 작성한다.
	out = fopen(outname,"w");
	fprintf(out,"%s\n",argv[1]);
	fprintf(out,"%d\n",algonum);
	fprintf(out,"%d\n",listsize);
	fprintf(out,"%f\n",(double)(finish-start)/CLOCKS_PER_SEC);

	for(i=1;i<=listsize;i++){
		fprintf(out,"%d ",list[i]);
	}

	return 0;
}

void open_input(char *inname, int algonumber){
	FILE *inp = fopen(inname,"r");
	int i,j;
	
	//input file을 열어 배열의 크기와 원소들을 입력받아 전역 변수에 저장한다.
	fscanf(inp,"%d",&listsize);
	list=(int*)malloc((listsize+1)*sizeof(int));
	for(i=1;i<=listsize;i++){
		fscanf(inp,"%d",&list[i]);
	}

	fclose(inp);
}

int partition(int left, int right){
	//algorithm_2 함수인 quick 정렬에 사용되는 pivot을 구해주는 함수이다.
	int i, pivot = left;

	for(i=left;i<right;i++){
		if(list[i] < list[right]){
			swap(list[i],list[pivot]);
			pivot+=1;
		}
	}
	swap(list[right],list[pivot]);

	return pivot;
}

void adjust(int root, int n){ //algorithm_3인 heap 정렬에 사용되는 heap을 생성하고 정렬해주는 함수이다.
	int child;
	int rootkey;

	rootkey = list[root];
	child = 2*root;
	while(child <= n){
		if((child < n)&&(list[child] < list[child+1]))child++;
		if(list[child] < rootkey)break;
		else{
			list[child/2] = list[child];
			child *= 2;
		}
	}
	list[child/2] = rootkey;
}

void algorithm_1(){ //insertion sort
	int i,j;
	int key;

	for(i=2;i<=listsize;i++){
		key = list[i]; 
		//현재 삽입될 숫자인 i번째 정수를 key 변수로 복사한다.
		//현재 정렬된 배열은 i-1까지이므로 i-1번째부터 역순으로 조사한다.
		for(j=i-1;(j>=1)&&(list[j]>key);j--){
			list[j+1] = list[j];
		}
		list[j+1]=key;
	}
}

void algorithm_2(int left, int right){ //quick sort
	int pivot;

	if(right-left > 0){
		pivot = partition(left,right);
		//partition 함수를 이용하여 pivot을 구하고 pivot을 기준으로 나눈 2개의 그룹에 대하여 각각 재귀적으로 함수를 호출한다.
		algorithm_2(left,pivot-1);
		algorithm_2(pivot+1,right);
	}
}

void algorithm_3(){ //heap sort
	int i;
	//listsize/2부터 시작하여 heap을 생성한다.
	for(i=listsize/2;i>0;i--){
		adjust(i,listsize);
	}
	//heap 정렬을 하여 오름차순으로 배열을 정렬해준다.
	for(i=listsize-1;i>0;i--){
		swap(list[1],list[i+1]);
		adjust(1,i);
	}
}
