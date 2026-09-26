#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "sort.cpp"

struct Text{
    char* textPtr;
    size_t sizeTextBuffer;
    size_t realSizeText;
    size_t numLines;
    char** arrOfStrPtr;
};

struct String{
    char* strAddress;
    size_t strSize;
};

size_t getfileSize(const char* fileName);
void setTextCharacter(Text* text, size_t fileSize, int fileDesc);
void fillArrOfStr(String* arrOfStr, char** arrOfStrPtr, size_t numLines);


size_t countNumLines(char* text, size_t realSizeText);
void convertTextIntoArrOfStrPtr(char* text, char** arrOfStrPtr, size_t realSizeText, size_t numLines);

void printLines(char** arrOfStrPtr, size_t numLines);
void printLinesInFile(FILE* filePtr, String* arrOfStr, size_t numLines);
void printSizes(String* arrOfStr, size_t numLines);


bool isLetter(char c);
bool isEmptyLine(const char* str);
bool doesStringContainLetter(const char* str, size_t length);

int comparatorPtrOfStrUp(const void* str1, const void* str2);
int strCmpWithoutPunctuationAndRegister(String string1, String string2);
int strCmpWithoutPunctuationAndRegisterReverse(String string1, String string2);
int wordsComparatorUpNew(const void* ptrstr1, const void* ptrstr2);
int wordsComparatorUpReverseNew(const void* ptrstr1, const void* ptrstr2);


int main(int argc, char* argv[]){

    char* defaultNameFile = "Eugene_Onegin.txt";
    char* fileName = NULL;
    if(argc > 1){
        fileName = argv[1];
        assert(fileName != NULL);
    }
    else{
        fileName = defaultNameFile;
    }

    assert(fileName != NULL);
    size_t fileSize = getfileSize((const char*) fileName);
    int fileDesc = open((const char*) fileName, O_RDONLY);
    assert(fileDesc >= 0);

    Text text = {}; 
    setTextCharacter(&text, fileSize, fileDesc);

    String* arrOfStr = (String*) calloc(text.numLines, sizeof(String));
    assert(arrOfStr);
    assert(text.arrOfStrPtr != NULL);
    fillArrOfStr(arrOfStr, text.arrOfStrPtr, text.numLines);
    //printSizes(arrOfStr, text.numLines);

    FILE* filePtr = fopen("oneginSort.txt", "w");
    assert(filePtr != NULL);

    qSort(arrOfStr, text.numLines, sizeof(arrOfStr[0]), wordsComparatorUpNew);
    printLinesInFile(filePtr, arrOfStr, text.numLines);

    qsort(arrOfStr, text.numLines, sizeof(arrOfStr[0]), wordsComparatorUpReverseNew);
    printLinesInFile(filePtr, arrOfStr, text.numLines);

    qsort(arrOfStr, text.numLines, sizeof(arrOfStr[0]), comparatorPtrOfStrUp);
    printLinesInFile(filePtr, arrOfStr, text.numLines);

    free(arrOfStr);
    free(text.textPtr);
    free(text.arrOfStrPtr);

    close(fileDesc);
    fclose(filePtr);

    return 0;
}

size_t countNumLines(char* text, size_t realSizeText){
    assert(text != NULL);
    
    size_t numLines = 1;
    size_t i = 0;
    while(i < realSizeText){
        assert(i < realSizeText);
        if(text[i] == '\n'){
           text[i] =  '\0';
            //printf("num of line = %d, count symbol in string = %d\n", numLines, i);
            numLines++;
        }
        i++;
    }
    //printf("final num of lines = %d, final count symbol in text = %d\n", numLines, i);
    return numLines;
}

void convertTextIntoArrOfStrPtr(char* text, char** arrOfStrPtr, size_t realSizeText, size_t numLines){
    assert(arrOfStrPtr != NULL);
    assert(text != NULL);

    arrOfStrPtr[0] = text;
    size_t indexOfStr = 1;
    size_t i = 0;
    while(i < realSizeText && indexOfStr < numLines){

        assert(i < realSizeText);
        assert(indexOfStr < numLines);

        if(text[i] == '\0'){
            //printf("\nplus one slash 0, i = %d\n", i);
            arrOfStrPtr[indexOfStr] = &text[i + 1];
            assert(arrOfStrPtr[indexOfStr] != NULL); 
            //printf("%c\n", arrOfStrPtr[indexOfStr][0]);
            indexOfStr++;
        }
        i++;
    }
    
    text[i] = '\0';
    //printf("\nThats all))\n");
    //printf("\nindex = <%d>\n", indexOfStr);
}

void printLines(char** arrOfStrPtr, size_t numLines){
    assert(arrOfStrPtr != NULL);

    for(size_t i = 0; i < numLines; i++){
        //printf("\n%d\n", i);

        assert(i < numLines);
        assert(arrOfStrPtr[i] != NULL);

        size_t lengthOfElemArrOfStrPtr = strlen(arrOfStrPtr[i]);
        //printf("%d\n", lengthOfElemArrOfStrPtr);
        //printf("\n%d\n", lengthOfElemArrOfStrPtr);
        if(!isEmptyLine(arrOfStrPtr[i])){
            for(size_t j = 0; j < lengthOfElemArrOfStrPtr; j++){

                assert(j < strlen(arrOfStrPtr[i]));
                printf("%c", arrOfStrPtr[i][j]);
            }
            printf("\n");
        }
    }
}

void printLinesInFile(FILE* filePtr, String* arrOfStr, size_t numLines){
    assert(filePtr);
    assert(arrOfStr);
    fprintf(filePtr, "\n\n");

    for(size_t i = 0; i < numLines; i++){
        //printf("\n%d\n", i);
        assert(i < numLines);

        size_t lengthOfElemArrOfStrPtr = arrOfStr[i].strSize - 1;
        //printf("%d\n", lengthOfElemArrOfStrPtr);
        //printf("\n%d\n", lengthOfElemArrOfStrPtr);
        if(!isEmptyLine(arrOfStr[i].strAddress)){
            for(size_t j = 0; j < lengthOfElemArrOfStrPtr; j++){

                assert(j < arrOfStr[i].strSize - 1);
                fprintf(filePtr, "%c", arrOfStr[i].strAddress[j]);
            }
            fprintf(filePtr, "\n");
        }
    }
    fprintf(filePtr,"---------------------------------------------------------------------\n\n\n");
}

bool isLetter(char c){
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return true;
    }
    return false;
}

bool isEmptyLine(const char* str){
    assert(str != NULL);

    size_t i = 0;
    while(i < strlen(str)){
        assert(i < strlen(str));
        if(str[i] != ' '){
            return false;
        }
        i++;
    } 
    return true;
}

int comparatorPtrOfStrUp(const void* str1, const void* str2){
    assert(str1);
    assert(str2);

    String string1 = *((const String*) str1);
    String string2 = *((const String*) str2);

    if(string1.strAddress > string2.strAddress){
        return 1;
    }
    else if(string1.strAddress == string2.strAddress){
        return 0;
    }
    else{
        return -1;
    }
}

size_t getfileSize(const char* fileName){
    assert(fileName != NULL);

    struct stat fileInfo = {};
    stat(fileName, &fileInfo);
    return fileInfo.st_size;
}

void setTextCharacter(Text* text, size_t fileSize, int fileDesc){
    assert(text != NULL);
    text->sizeTextBuffer = fileSize + 1;

    text->textPtr = (char*) calloc(text->sizeTextBuffer, sizeof(char));
    assert(text->textPtr != NULL);

    text->realSizeText = read(fileDesc, (void*) text->textPtr, fileSize) + 1;
    text->numLines = countNumLines(text->textPtr, text->realSizeText) + 1;
    
    text->arrOfStrPtr = (char**) calloc(text->numLines, sizeof(char*));
    assert(text->arrOfStrPtr != NULL);

    convertTextIntoArrOfStrPtr(text->textPtr, text->arrOfStrPtr, text->realSizeText, text->numLines);
}

int strCmpWithoutPunctuationAndRegister(String string1, String string2){

    size_t i1 = 0;
    size_t i2 = 0;

    size_t size1 = string1.strSize - 1;
    size_t size2 = string2.strSize - 1;

    // printf("size1 cmp = %d\n", string1.strSize);
    // printf("size2 cmp = %d\n", string2.strSize);
    // getchar();

    const char* str1 = (const char*) string1.strAddress;
    const char* str2 = (const char*) string2.strAddress;
    assert(str1);
    assert(str2);

    while(i1 < size1 && i2 < size2){
        assert(i1 < size1);
        assert(i2 < size2);

        if(isLetter(str1[i1]) && isLetter(str2[i2])){
            char c1 = (char) tolower(str1[i1]);
            char c2 = (char) tolower(str2[i2]);
            if(c1 > c2){
                return 1;
            }
            else if(c2 > c1){
                return -1;
            }
            i1++;
            i2++;
        }
        else if(isLetter(str1[i1]) && !isLetter(str2[i2])){
            i2++;
        }
        else if(!isLetter(str1[i1]) && isLetter(str2[i2])){
            i1++;
        }
        else{
            i1++;
            i2++;
        }
    }

    assert(i1 <= size1);
    assert(i2 <= size2);

    if(i1 == size1){
        assert(str2 != NULL);
        assert(size2 >= i2);

        if(doesStringContainLetter((const char*) str2 + i2, size2 - i2)){
            return -1;
        }
    }
    if(i2 == size2){
        assert(str1 != NULL);
        assert(size1 >= i1);

        if(doesStringContainLetter((const char*) str1 + i1, size1 - i1)){
            return 1;
        }
    }
    return 0;
}

int strCmpWithoutPunctuationAndRegisterReverse(String string1, String string2){

    size_t size1 = string1.strSize - 1;
    size_t size2 = string2.strSize - 1; 

    int i1 = size1 - 1;
    int i2 = size2 - 1;

    const char* str1 = (const char*) string1.strAddress;
    const char* str2 = (const char*) string2.strAddress;
    assert(str1);
    assert(str2);

    while(i1 >= 0 && i2 >= 0){
        assert(i1 >= 0);
        assert(i2 >= 0);
        
        if(isLetter(str1[i1]) && isLetter(str2[i2])){
            char c1 = (char) tolower(str1[i1]);
            char c2 = (char) tolower(str2[i2]);
            if(c1 > c2){
                return 1;
            }
            else if(c2 > c1){
                return -1;
            }
            i1--;
            i2--;
        }
        else if(isLetter(str1[i1]) && !isLetter(str2[i2])){
            i2--;
        }
        else if(!isLetter(str1[i1]) && isLetter(str2[i2])){
            i1--;
        }
        else{
            i1--;
            i2--;
        }
    }

    if(i2 >= 0){
        assert(str2 != NULL);
        if(doesStringContainLetter(str2, i2 + 1)){
            return -1;
        }
    }
    else if(i1 >= 0){
        assert(str1 != NULL);
        if(doesStringContainLetter(str1, i1 + 1)){
            return 1;
        }
    }
    return 0;
}

bool doesStringContainLetter(const char* str, size_t length){
    assert(str != NULL);

    size_t i = 0;
    while(i < length){
        assert(i < length);
        if(isLetter(str[i])){
            return true;
        }
        i++;
    }
    return false;
}

void fillArrOfStr(String* arrOfStr, char** arrOfStrPtr, size_t numLines){
    assert(arrOfStr != NULL);
    assert(arrOfStrPtr != NULL);

    size_t i = 0;
    while(i < numLines){
        arrOfStr[i].strAddress = arrOfStrPtr[i];
        arrOfStr[i].strSize = strlen(arrOfStrPtr[i]) + 1;
        // printf("size = %zu\n", arrOfStr[i].strSize);
        // getchar();
        i++;
    }
    return;
}

int wordsComparatorUpNew(const void* ptrstr1, const void* ptrstr2){
    assert(ptrstr1 != NULL);
    assert(ptrstr2 != NULL);
    
    String string1 = *((const String*) ptrstr1);
    String string2 = *((const String*) ptrstr2);

    // printf("size1 = %d\n", string1.strSize);
    // printf("size2 = %d\n", string2.strSize);
    // printf("symv 1 = %c\n", string1.strAddress[0]);
    // printf("symv 2 = %c\n", string2.strAddress[0]);
    // getchar();
    // const char* str1 = (const char*) string1.strAddress;
    // const char* str2 = (const char*) string2.strAddress;
    
    // assert(str1 != NULL);
    // assert(str2 != NULL);

    int res = strCmpWithoutPunctuationAndRegister(string1, string2);
    return res;
}

int wordsComparatorUpReverseNew(const void* ptrstr1, const void* ptrstr2){
    assert(ptrstr1 != NULL);
    assert(ptrstr2 != NULL);
    
    String string1 = *((const String*) ptrstr1);
    String string2 = *((const String*) ptrstr2);
    // printf("size1 = %zu\n", string1.strSize);
    // printf("size2 = %zu\n", string2.strSize);
    // printf("symv 1 = %c\n", string1.strAddress[0]);
    // printf("symv 2 = %c\n", string2.strAddress[0]);
    //getchar();
    
    // const char* str1 = (const char*) string1.strAddress;
    // const char* str2 = (const char*) string2.strAddress;
    
    // assert(str1 != NULL);
    // assert(str2 != NULL);

    int res = strCmpWithoutPunctuationAndRegisterReverse(string1, string2);
    return res;
}

void printSizes(String* arrOfStr, size_t numLines){
    assert(arrOfStr);
    size_t i = 0;
    while(i < numLines){
        printf("num of line = %d\n", i + 1);
        printf("size of line = %d\n", arrOfStr[i].strSize);
        i++;
    }
}