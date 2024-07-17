#include<stdio.h>

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
        if ((codebook_fp = fopen(codebook, "w+b")) == NULL){
                printf("ERROR: cannot open codebook file\n");
                return 1;
        }
        if ((vq_fp = fopen(vq, "wb")) == NULL){
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

	for(int i = 0; i < rowElements - 1; i++){
		for(int j = 0; j < columnElements - 1; j++){
			for(int k = 0; k < rowElements; k++){
				fseek(rawMap_fp, start + sameRowElements * i + columnElements * j , SEEK_SET);
				fread(&data, sizeof(char), columnElements, rawMap_fp);
				fwrite(&data, sizeof(char), columnElements, codebook_fp);
			}

		}
	}

//ファイルを閉じる
        fclose(rawMap_fp);
        fclose(codebook_fp);
        fclose(vq_fp);

	return 0;
}

int main(void){
	char rawMap[] = "/home/yuichirohatanaka/vq_ws/map/default.pgm";
	char codebook[] = "codebook.txt";
	char vq[] = "vq.txt";
	int width = 4000;
	int height = 4000;
	int start = 54;
	int rowElements = 10;
	int columnElements = 10;
	
	vectorQuantizer(rawMap, codebook, vq, width, height, rowElements, columnElements, start);

	return 0;
}
