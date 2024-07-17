#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int P5Header(char rawMap[], int *rawWidth, int *rawHeight, int *start) {
   FILE *map_file;
   char line[MAX_LINE_LENGTH];
   char P5[] = "P5\n";
   char pixelValue[] = "255\n";
   int width = 0;
   int height = 0;
   int P5Length = 3;
   int commentLength = 0;
   int sizeLength = 0;
   int pixelValueLength = 4;

// ファイルをテキスト形式で開く
// マップデータ自体はバイナリ形式で記述されているが、ヘッダはテキスト形式で記述されてる
// そのためマップデータはhexeditで開かないと確認できないが、ヘッダはgeditで開いても確認可能
    map_file = fopen( rawMap, "r");
    if (map_file == NULL) {
        perror("ファイルを開くことができません");
        return 1;
    }
// １行目のデータがP5かを確認し、マップ形式がバイナリ形式のグレースケールかを確認
// fgetによって得たデータは改行' \n 'が後ろについているので注意
   	fgets(line, MAX_LINE_LENGTH, map_file);
	printf("%s", line);
	if(strcmp(line, P5) == 0){
	       printf("ファイルはP5形式です\n");
	}else{
		printf("not P5\n");
		return 1;	
	}

// ２行目のデータから画像の幅と高さのデータを読み取る
// １行目のファイル形式のデータとその次の画像サイズのデータの間にコメント(#)がある場合があるので最初に確認
// コメント行でないことを確認した後画像の幅と高さのデータを入手
	for(;;){
		fgets(line, MAX_LINE_LENGTH, map_file);
		if(line[0] == '#'){
			printf("コメントがあります\n%s\n", line);
			commentLength = strlen(line);
			printf("コメントは%d文字です\n", commentLength);
		}else{
			printf("下記のデータからマップサイズを読み取ります\n%s\n", line);
			sscanf(line,"%d %d", &width, &height);
			*rawWidth = width;
			*rawHeight = height;
			sizeLength = strlen(line);
			break;
		}
	}

// ３行目は輝度を表す画素値の範囲があります
// 今回は２５５かを調べるだけにします
	fgets(line, MAX_LINE_LENGTH, map_file);
        printf("%s", line);
        if(strcmp(line, pixelValue) == 0){
               printf("グレースケールの画素値の範囲は0~255です\n");
        }else{
                printf("not P5\n");
                return 1;
        }

 // ファイルを閉じる
    fclose(map_file);

 // マップデータの先頭メモリを計算
    *start = P5Length + commentLength + sizeLength + pixelValueLength;

    return 0;
}

int main(){
	char rawMap[] = "/home/yuichirohatanaka/vq_ws/map/default.pgm";
	int width, height, start;

	P5Header(rawMap, &width, &height, &start);
	printf("\n画像サイズ\nwidth: %d\nheight: %d\nマップデータは%d番目のメモリからスタート\n", width, height, start);
	return 0;
}
