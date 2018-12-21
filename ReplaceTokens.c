/*
トークンごとの文字列置換．
*/
#include <stdio.h>
#include <string.h>

#define CHARBUFF 256
#define LEN(arr) sizeof(arr)/sizeof(*arr)

typedef struct{
    char before[CHARBUFF];
    char after[CHARBUFF];
}ReplaceList;

void splitFileName(char*, char*, char*);
void replaceAndOutput(FILE*, FILE*, ReplaceList*, int);

int main(void){
    char buf[CHARBUFF];

    /*
    文書ファイルの読み込み
    */
    // 文書ファイルはユーザ指定
    char docName[CHARBUFF];
    printf("Input FileName of Document: ");
    fgets(docName, LEN(docName), stdin);
    docName[strlen(docName)-1] = '\0';

    FILE *fpDoc = fopen(docName, "r");
    if(fpDoc == NULL){
        printf("Can't find DocumentFile!\n");
        return -1;
    }

    /*
    置換リストのファイル名は固定にして読み込み
    */
    FILE *fpRpl = fopen("ReplaceList.txt", "r");
    ReplaceList rl[CHARBUFF];
    int rlLen = 0; // 初期はリストrlのインデックス．転じてrlの長さ．
    int rlTmp=0;
    int isAttr = 1;
    while(fgets(buf, LEN(buf), fpRpl) != NULL){
        if(isAttr){
            isAttr = 0;
            continue;
        }

        sscanf(buf, "%[^->]->%s\n", rl[rlLen].before, rl[rlLen].after);
        // 今回のコードではスペース区切りになっているため，句読点「.」「,」を含んだトークンを区別できない．
        // したがって句読点を含んだ場合のものも置換リストに追加していく．
        rlTmp = rlLen;
        rlLen++;
        sprintf(rl[rlLen].before, "%s.", rl[rlTmp].before);
        sprintf(rl[rlLen].after, "%s.", rl[rlTmp].after);
        rlLen++;
        sprintf(rl[rlLen].before, "%s,", rl[rlTmp].before);
        sprintf(rl[rlLen].after, "%s,", rl[rlTmp].after);
        rlLen++;
    }
    fclose(fpRpl);

    /*
    置換後の文書ファイルの生成．
    入力の文書ファイル名に「_replaced」を加えたファイル名で出力．
    */
    char fName[CHARBUFF] = {'\0'}; // なぜか文字化け文字が最初に格納されていたので初期化
    char fExtension[CHARBUFF];
    // 入力文書ファイルの名前をファイル名と拡張子に分解
    splitFileName(fName, fExtension, docName);
    // 「_replaced」を追加する．
    sprintf(docName, "%s_replaced.%s", fName, fExtension);
    FILE *fpAfterDoc = fopen(docName, "a");

    /*
    置換・ファイル出力処理
    */
    replaceAndOutput(fpAfterDoc, fpDoc, rl, rlLen);
    
    /*
    ファイル操作変数を開放する
    */
    fclose(fpDoc);
    fclose(fpAfterDoc);

    /*
    処理終了の標準出力
    */
    printf("Outputted: %s\n", docName);

    return 0;
}

/*

*/
void splitFileName(char *fName, char *fExtension, char *docName){
    char *p;
    p = strtok(docName, ".");
    sprintf(fName, "%s", p);
    p = strtok(NULL, ".");
    sprintf(fExtension, "%s", p);

    /* こっちでも可．ただし引数にstrlen()に基づくint型変数を追加すること．
    int i;
    int isExtension = 0;
    for(i=0; i<docLen; i++){
        if(docName[i] == '.'){
            isExtension = 1;
            continue;
        }

        if(!isExtension){
            sprintf(fName, "%s%c", fName, docName[i]);
        }else{
            sprintf(fExtension, "%s%c", fExtension, docName[i]);
        }
    }
    */
}

/*

*/
void replaceAndOutput(FILE *fpAfterDoc, FILE *fpDoc, ReplaceList *rl, int rlLen){
    int rl_i, isReplaced=0;
    char *p, buf[CHARBUFF];
    while(fgets(buf, LEN(buf), fpDoc)){
        p = strtok(buf, " ");
        while(p != NULL){
            for(rl_i=0; rl_i<rlLen; rl_i++){
                if(!strcmp(p, rl[rl_i].before)){
                    fwrite(rl[rl_i].after, 1, strlen(rl[rl_i].after), fpAfterDoc);
                    isReplaced = 1;
                    break;
                }
            }

            if(!isReplaced){
                fwrite(p, 1, strlen(p), fpAfterDoc);
            }else{
                isReplaced = 0;
            }

            //printf("%s(%ld): %s\n", p, strlen(p), (!strcmp(p, "\r\n"))?"True":"False");
            if(strcmp(p, "\r\n") != 0){
                fwrite(" ", 1, 1, fpAfterDoc);
            }

            p = strtok(NULL, " ");
        }
    }
}