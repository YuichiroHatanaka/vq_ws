#include"P5Header.h"

int main(){
        char rawMap[] = "/home/yuichirohatanaka/vq_ws/map/default.pgm";
        int width, height, start;

        P5Header(rawMap, &width, &height, &start);
        printf("\n画像サイズ\nwidth: %d\nheight: %d\nマップデータは%d番目のメモリからスタート\n", width, height, start);
        return 0;
}

