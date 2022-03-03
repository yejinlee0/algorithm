#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int aidx = 0, maxdepth = 0;
int number = 0, charnum = 0; 
// aidx는arr의 index이다. maxdepth는 허프만 트리의 최대 깊이이다.
// number는 입력 파일에 존재하는 문자들의 수를 전부 센 숫자이다.
// charnum은 입력 파일에 존재하는 문자들의 수를 중복을 제외하고 센 숫자이다.

typedef struct TreeNode {
	int weight; //빈도수를 저장하는 변수이다.
	char alpha; // 데어터를 저장하는 변수이다.
	char *code; // 허프만 코드를 저장하는 문자열이다.
	int depth; // 허프만 코드의 길이를 저장하는 변수이다.
	struct TreeNode *left_child;
	struct TreeNode *right_child;
}TreeNode;

typedef struct Node{
	int f; // 빈도수를 저장하는 변수이다.
	char data; // 데이터를 저장하는 변수이다.
	char *code; // 허프만 코드를 저장하는 문자열이다.
	int size; // 허프만 코드의 길이를 저장하는 변수이다.
}Node;

Node *arr;

typedef struct {
	TreeNode *ptree;
	int key;
}element;

typedef struct {
	element *heap; // 배열 개수만큼 동적 할당하여 사용한다.
	int heap_size;
}HeapType;

void init(HeapType *h) { // 자료형 HeapType인 배열을 초기화하는 함수이다.
	h->heap_size = 0;
	h->heap=(element*)malloc(charnum*(sizeof(element)));
}

void insert_min_heap(HeapType *h, element item) { // 최소 힙을 만들기 위해 삽입해주는 함수이다.
	int i;
	i = ++(h->heap_size);

	while ((i != 1) && ((item.key)<(h->heap[i / 2].key))) {
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item;
}

element delete_min_heap(HeapType *h) { // 최소 힙에서 heap[1]을 반환해주고 나머지를 다시 정렬해주는 함수이다.
	int parent, child;
	element item, temp;

	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	while (child <= h->heap_size) {
		if ((child <= h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
			child++;
		if (temp.key < h->heap[child].key)break;
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

TreeNode *make_tree(TreeNode *left, TreeNode *right) { // 허프만 트리를 만들기 위해 초기화해주는 함수이다.
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
	if (node == NULL) {
		fprintf(stderr, "memory error\n");
		exit(1);
	}
	node->left_child = left;
	node->right_child = right;
	
	node->code = (char*)calloc(charnum+5,sizeof(char));
	
	return node;
}

void destroy_tree(TreeNode *root) { // 트리에 할당된 공간을 반환해주는 함수이다.
	if (root == NULL)	
		return;
	destroy_tree(root->left_child);
	destroy_tree(root->right_child);
	free(root);
}

void huffman_code(TreeNode *r, int n, char *code) { // 허프만 코드를 생성해주는 함수이다.
	if (r) {
		n++;
		code[n] = '1';
		huffman_code(r->right_child, n, code);
		code[n] = '0';
		huffman_code(r->left_child, n, code);
		code[n] = '\0';
		if (r->left_child == NULL&&r->right_child == NULL) {
			strcpy(r->code, code);
			r->depth = n;
			if(n>maxdepth)maxdepth=n;
		}
	}
}

void sub_order(TreeNode *r, int i) { // 트리를 순회하며 알맞은 값을 대입해주는 함수이다.
	if (r) {
		sub_order(r->right_child, i);
		sub_order(r->left_child, i);
		if ((r->left_child == NULL || r->right_child == NULL) && r->depth == i) {			
			arr[aidx].data = (r->alpha);
			strcpy(arr[aidx].code, r->code);
			arr[aidx].size = r->depth;
			arr[aidx++].f = r->weight;
		}
	}
}

void level_order(TreeNode *r) { // sub_order를 호출해주는 기능을 하는 함수이다.
	int i;
	for (i = 0; i <=maxdepth; i++)sub_order(r, i);

}

void huffman_tree(int freq[], int n) { // 빈도수를 저장한 배열을 이용해서 허프만 트리를 만드는 함수이다.
	int i, j = 0;
	char *cd = (char*)calloc(1,sizeof(char));
	TreeNode *node, *x;
	HeapType heap;
	element e, e1, e2;

	init(&heap);
	
	for (i = 0; i < n; i++) {
		if (freq[i] != 0) { // 1번 이상 나온 문자로만 생성한다.
			node = make_tree(NULL, NULL);
			// make_tree 함수를 이용해서 왼쪽, 오른쪽 가지를 초기화하고 code 변수에 대한 공간을 동적 할당한다.
			e.key = freq[i];
			node->weight = freq[i];
			node->alpha = i;
			e.ptree = node;
			insert_min_heap(&heap, e);
			j++;
		}
	}

	for (i = 1; i < j; i++) {
		e1 = delete_min_heap(&heap);
		e2 = delete_min_heap(&heap);
		x = make_tree(e1.ptree, e2.ptree);
		e.key = e1.key + e2.key;
		x->weight = e1.key + e2.key;
		e.ptree = x;
		insert_min_heap(&heap, e);
	}

	e = delete_min_heap(&heap);
	huffman_code(e.ptree, -1, cd);
	
	level_order(e.ptree);
}

void compress(char *inname) { // -c 명령어를 입력받았을 때 압축을 해주는 함수이다.
	char tmp = 0;
	int i,j,m;
	
	char *outname=(char*)malloc(sizeof(char)*(strlen(inname)+4));
	strcpy(outname, inname);
	strcat(outname,".zz");

	FILE *in = fopen(inname, "r");
	FILE *out = fopen(outname, "w");	
		
	fprintf(out, "%d/", charnum); // 사용된 문자의 개수를 적는다.

	for (i = 0; i < charnum; i++) {
		fprintf(out, "%c%c", arr[i].data,arr[i].size);
		for(j=0;j<(int)(arr[i].size);j++){
			fprintf(out,"%c",arr[i].code[j]);
		}
	}
	
	fprintf(out, "\n");
	
	int location = ftell(out);

	if(fseek(out,4,SEEK_CUR)!=0){
		printf("Error : the file pointer error\n");
		return;
	}

	char *bitbuf = (char*)calloc(number+2,sizeof(char));
	int allflg=0, bidx=0, idx = 0, bitBufIdx = 0, bitShiftCnt = 7, totalbit = 0, flag = 0;
	j=0;
	// bidx는 buf의 인덱스를 나타내는 변수이다.
	// 8개의 비트씩 끊어서 정보를 출력 파일에 작성한다.
	
	while(!feof(in)){
		char *buf = (char*)calloc(number+2,sizeof(char));
	
		allflg=0;
		bidx=0;

		while(!feof(in)){
			int k = fscanf(in,"%c",&buf[bidx++]);

			if(k==-1){
				allflg=1;
				break;
			}

			if(((int)(buf[bidx-1]))==10){
				break;
			}
		}

		if(bidx==1&&allflg==1)break;
		

		int nullflg=0,len=0;
		
		for(m=0;m<bidx;m++){
			if(buf[m]=='\0')nullflg=1;
		}
		
		if(nullflg==0){
			len = strlen(buf);
		}
		else{
			len = bidx;
		}

		idx=0;

		// 위에는 입력 파일에서 글자를 읽고 글자 개수를 센 다음 아래에서 허프만 코드로 바꾸어 출력 파일에 비트 단위로 저장한다.

		for(i=0;i<len;i++){
			for(j=0;j<charnum;j++){
				if((int)(buf[i])==(int)(arr[j].data)){
					idx = j;
					break;
				}
			}
	
			char *HuffmanCode = arr[idx].code;

			for(j=0;j<strlen(HuffmanCode);j++){
				char value = 0;
				if(HuffmanCode[j]=='0')value=0;
				else if(HuffmanCode[j]=='1')value=1;
				else {
					printf("Error : Invalid value of code\n");
					return;
				}
				
				value = value << bitShiftCnt;
				bitShiftCnt--;

				bitbuf[bitBufIdx] |= value; // or 연산을 수행한다.
				flag = 1;
	
				totalbit++;

				if(bitShiftCnt<0){
					bitShiftCnt = 7;
					bitBufIdx++;
					if(bitBufIdx>=number+2){
						fwrite(bitbuf,1,number+2,out);
						flag=0;
						bitBufIdx=0;
						memset(bitbuf,0,number+2);
					}
				}
			}	
		}	
		if(allflg==1)break;
	}

	if(flag==1){
		fwrite(bitbuf,1,bitBufIdx+1,out);
	}

	if(fseek(out,location,SEEK_SET)==0){
		fwrite(&totalbit,sizeof(totalbit),1,out);
	}

	else {
		printf("Error : unable to record total number of bits\n");
	}
	
	fclose(in);
	fclose(out);
}

int decompress(char *inname) { // .zz로 끝나는 입력 파일의 압축을 푸는 함수이다.
	char *outname=(char*)malloc(sizeof(char)*(strlen(inname)+4)); // .yy로 끝나는 출력 파일의 이름을 저장하는 변수이다.
	strcpy(outname, inname);
	strcat(outname,".yy");

	FILE *in = fopen(inname, "r");
	if(in==NULL)return 2; // 파일이 존재하지 않으면 2를 반환한다.
	
	FILE *out = fopen(outname, "w");

	int i, j, Symbol = 0;

	fscanf(in,"%d%*c",&Symbol);

	TreeNode *huffRoot = (TreeNode*)malloc(sizeof(TreeNode));
	huffRoot->left_child = NULL;
	huffRoot->right_child = NULL;

	TreeNode *current = huffRoot;

	for(i=0;i<Symbol;i++){
		char ChAndLen[2];
		fscanf(in, "%c", &ChAndLen[0]); // 심볼 데이터를 읽는다.
		fscanf(in, "%c", &ChAndLen[1]); // 심볼 허프만 길이를 읽는다.

		char *buf = (char*)calloc(((int)ChAndLen[1])+2,sizeof(char));
		fread(buf,sizeof(char),(int)ChAndLen[1],in); // 허프만 코드를 읽는다.
		
		buf[(int)ChAndLen[1]]='\0';

		current = huffRoot;
		
		// 허프만 코드를 읽어서 허프만 트리를 생성한다.

		for(int i=0;i<((int)ChAndLen[1]);i++){
			if(buf[i]=='0'){
				if(current->left_child==NULL){
				current->left_child	= (TreeNode*)malloc(sizeof(TreeNode));
				current->left_child->left_child=NULL;
				current->left_child->right_child=NULL;
				}
				current = current->left_child;
			}
			else if(buf[i]=='1'){
				if(current->right_child==NULL){
				current->right_child = (TreeNode*)malloc(sizeof(TreeNode));
				current->right_child->left_child=NULL;
				current->right_child->right_child=NULL;
				}
				current = current->right_child;
			}
			else{
				printf("Error : fail to make tree\n");
				return 2;
			}
		}
		current->alpha = ChAndLen[0];
	}

	fscanf(in,"%*c"); // 줄바꿈 문자는 무시한다.

	int numBits = 0;
	fread(&numBits,sizeof(int),1,in); // 4바이트 정수형인 비트 수가 저장된 정수를 읽는다.
	current = huffRoot;

	char *buf2 = (char*)calloc(numBits+2,sizeof(char));
	int bidx=0;

	// 비트 단위로 적혀있는 정보를 비트 연산을 이용해서 char 형 정보로 전환하여 출력 파일에 작성한다.

	while(1){
		int sz = fread(buf2,1,numBits,in);

		if(sz==0)
			break;
		else{
			for(i=0;i<sz;i++){
				for(j=0;j<8;j++){
					if((char)(buf2[i]&0x80)==0){
						current = current->left_child;
					}
					else {
						current = current->right_child;
					}

					buf2[i] = buf2[i] << 1;
					numBits--;
					
					if(current->left_child==NULL && current->right_child==NULL){
						fprintf(out, "%c",current->alpha);
						
						current = huffRoot;
					}

					if(numBits==0){

						fclose(in);
						fclose(out);
						return 1;
					}
				}
			}
		}
	}
	
	fclose(in);
	fclose(out);

	return 1;
}

int open_input(char *inname);

int main(int argc, char* argv[]){
	int check = 0;

	if(argv[1][1]=='c'){ //compressing
		check = open_input(argv[2]); // 함수를 이용해서 파일을 열어 정보를 읽는다.
		if(check==2){ // 파일이 존재하지 않으면 오류 메세지를 출력하고 프로그램을 종료한다.
			printf("Error : Input file does not exist!\n");
			return 0;
		}

		compress(argv[2]); // 함수를 이용해서 파일을 압축한다.
	}

	else if(argv[1][1]=='d'){ //decompressing
		check = decompress(argv[2]); // 함수를 이용해서 파일의 압축을 푼다.
		if(check==2){ // 입력 파일이 존재하지 않으면 오류 메세지를 출력하고 프로그램을 종료한다.
			printf("Error : Input file does not exist!\n");
			return 0;
		}
	}

	else{ //option이 -c 나 -d가 아닌 모든 경우에는 오류로 처리한다.
		printf("Error : Invalid option! Only use '-c' or '-d'\n");
		return 0;
	}

	return 0;
}
int open_input(char *inname){ // 파일을 열어 정보를 읽는 함수이다.

	FILE *inp = fopen(inname, "r"); // 입력 받은 이름의 파일을 연다.
	if(inp==NULL)return 2; // 파일이 존재하지 않으면 2를 반환한다.
		
	char tmp = 0; // 하나의 문자씩 입력 받는 버퍼이다.
	int ftmp = 0, r = 0, j, i = 0, idx = 0, hidx = 0; // idx는 tmp의 10진수 아스키코드를 저장한다.
	int freq[128] = { 0 }; // 문자의 빈도수를 저장할 배열이다.
	int dup[128] = { 0 }; // 입력 파일의 문자를 셀 때 중복을 방지하기 위해 사용하는 배열이다.
	
	while (fscanf(inp, "%c", &tmp) != -1) {	
		idx = tmp;
		freq[idx]++;
		if (dup[idx] == 0) {
			dup[idx] = 2;
			charnum++;
		}
		number++;
	}
	
	fclose(inp);
	
	arr = (Node*)malloc((charnum+1)*(sizeof(Node))); // 사용된 문자의 수만큼 공간을 할당한다.
	for(i=0;i<charnum;i++){
		arr[i].code=(char*)calloc(charnum+5,sizeof(char));
	}
	
	huffman_tree(freq, 128);
	
	return 1;
}
