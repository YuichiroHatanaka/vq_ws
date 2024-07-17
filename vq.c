#include<stdio.h>
#include<string.h>

int vectorQuantizer(char rawMap[], char codebook[], char vq[], int width, int height, int rowElements, int columnElements, int start){
	FILE *rawMap_fp;
	FILE *codebook_fp;
	FILE *vq_fp;
	int rowVector;
	int columnVector;

//ファイルをオープンする
	if ((rawMap_fp = fopen(rawMap, "rb")) == NULL){
        	printf("ERROR: cannot open r file\n");
        	return 1;
        }
        if ((codebook_fp = fopen(codebook, "ab")) == NULL){
                printf("ERROR: cannot open codebook file\n");
                return 1;
        }
        if ((vq_fp = fopen(vq, "ab")) == NULL){
                printf("ERROR: cannot open vq file\n");
                return 1;
        }

//rawMapをいくつに分割するか？
	rowVector = (height + rowElements - 1) / rowElements;
	columnVector = (width + columnElements - 1) / columnElements;
	printf("地図は縦%d個、横%d個に分割されます\n", rowVector, columnVector);



//スタート地点に移動

//データを読み取る
	char data[columnElements];
//予め計算
	int sameRowElements = width * rowElements;

//同じ行のベクトルのうち一番右のベクトルで使用
	int remainderElements = width % columnElements;
	if(remainderElements == 0)
		remainderElements = columnElements;
	int additionalElements = columnElements - remainderElements;
	char remainderData[remainderElements];
	char additionalData[additionalElements];
//一番下のベクトル群で使用
	int remainderRow = height % rowElements;
	if(remainderRow == 0)
		remainderRow = rowElements;
	int additionalRow = rowElements - remainderRow;
	int remainderRowElements = width * remainderRow;
	char additionalRowData[columnElements];
	for(int i = 0; i < columnElements; i++)
		additionalRowData[i] = 0xCD;


	//一番下のベクトル群を除いたベクトル
	for(int i = 0; i < rowVector - 1; i++){
		printf("%d\n", i);
		//右端のベクトルを除いた同じ行のベクトル
		for(int j = 0; j < columnVector - 1; j++){
			printf("└%d\n", j);
			for(int k = 0; k < rowElements; k++){
				fseek(rawMap_fp, start + sameRowElements * i + columnElements * j + width * k, SEEK_SET);
				fread(&data, sizeof(char), columnElements, rawMap_fp);
				fwrite(&data, sizeof(char), columnElements, codebook_fp);
				printf(" └%d\n", k);
			}
			//代表ベクトルと比較
		}
		//右端のベクトル
		printf("└%d\n", columnVector - 1);
		for(int k = 0; k < rowElements; k++){
			fseek(rawMap_fp, start + sameRowElements * i + columnElements * (columnVector - 1) + width * k, SEEK_SET);
			fread(&remainderData, sizeof(char), remainderElements, rawMap_fp);
			for(int l = 0; l < additionalElements ; l++)
				additionalData[l] = 0xCD;
			printf(" └%d %s\n", k, additionalData);
			strcpy(data, remainderData);
			strcat(data, additionalData);
	//		string.hライブラリのstrcpyとstrcatを使い、dataにまとめたので不要
			fwrite(&remainderData, sizeof(char), remainderElements, codebook_fp);
			fwrite(&additionalData, sizeof(char), additionalElements, codebook_fp);
	//		fwrite(&data, sizeof(char), remainderElements, codebook_fp);
		}
		//代表ベクトルと比較
	}

	//一番下のベクトル群に含まれるベクトル
	//右端のベクトルを除いた同じ行のベクトル
	for(int j = 0; j < columnVector - 1; j++){
		printf("└%d end\n", j);
		printf("remaindarRow %d\n", remainderRow);
		for(int k = 0; k < remainderRow; k++){
			fseek(rawMap_fp, start + sameRowElements * (rowVector - 1) + columnElements * j + width * k, SEEK_SET);
			fread(&data, sizeof(char), columnElements, rawMap_fp);
			fwrite(&data, sizeof(char), columnElements, codebook_fp);
			printf(" └%d end\n", k);
		}
		for(int k = 0; k < additionalRow; k++){
			fwrite(&additionalRowData, sizeof(char), columnElements, codebook_fp);
			printf(" └%d\n", k);
		}
		//代表ベクトルと比較
	}
	//右端のベクトル
	printf("└%d\n", columnVector - 1);
	for(int k = 0; k < remainderRow; k++){
		fseek(rawMap_fp, start + sameRowElements * (rowVector - 1) + columnElements * (columnVector - 1) + width * k, SEEK_SET);
		fread(&remainderData, sizeof(char), remainderElements, rawMap_fp);
		for(int l = 0; l < additionalElements ; l++)
			additionalData[l] = 0xCD;
		printf(" └%d %s\n", k, additionalData);
		strcpy(data, remainderData);
		strcat(data, additionalData);
//		string.hライブラリのstrcpyとstrcatを使い、dataにまとめたので不要
		fwrite(&remainderData, sizeof(char), remainderElements, codebook_fp);
		fwrite(&additionalData, sizeof(char), additionalElements, codebook_fp);
//		fwrite(&data, sizeof(char), remainderElements, codebook_fp);
	}
	for(int k = 0; k < additionalRow; k++){
		fwrite(&additionalRowData, sizeof(char), columnElements, codebook_fp);
		printf(" └%d\n", k);
	}
	//代表ベクトルと比較

//ファイルを閉じる
        fclose(rawMap_fp);
        fclose(codebook_fp);
        fclose(vq_fp);

	return 0;
}

int main(void){
//	char rawMap[] = "/home/yuichirohatanaka/vq_ws/map/default.pgm";
	char rawMap[] = "sample1.pgm";
	char codebook[] = "codebook.txt";
	char vq[] = "vq.txt";
	int width = 9;
	int height = 7;
	int start = 30;
	int rowElements = 2;
	int columnElements = 2;
	
	vectorQuantizer(rawMap, codebook, vq, width, height, rowElements, columnElements, start);

	return 0;
}
