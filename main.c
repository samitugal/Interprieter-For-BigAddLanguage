#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

char *variableList[1000];
int vIndex = 0;
int variableValueList[1000] = { 0 };
int iIndex = 0;

bool isKeyword(char* str) {

	if (!strcmp(str, "int") ||
		!strcmp(str, "move") || !strcmp(str, "to") || !strcmp(str, "add") ||
		!strcmp(str, "sub") ||  !strcmp(str, "from") ||
		!strcmp(str, "loop") || !strcmp(str, "times") ||
		!strcmp(str, "out")) {
		return true;
	}

	return false;
}

bool isInteger(char* str) {

	int len = strlen(str);

	if (len > 100) {
		return false;
	}

	for (int i = 0; i < len; i++) {
		if (i > 0) {
			if (str[i] == '-' || len == 1) {
				return false;
			}
		}
		if (!(isdigit(str[i]) || str[i] == '-')) {
			return false;
		}
	}

	return true;
}

bool isIntegerVariable(char* str) {

	for (int i = 0; i < vIndex; i++) {
		if (!strcmp(str, variableList[i])) {
			return true;
		}
	}

	return false;
}

bool isVariable(char* str)
{
	int len = strlen(str);

	if (len > 20) {
		return false;
	}

	for (int i = 0; i < len; i++) {
		if (str[0] == '_') {
			return false;
		}
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == '_'))) {
			return false;
		}
	}

	return true;
}



const char* isalnumControl(char* str[]){
    int size = strlen(str);

	if(!isalnum(str[size-1])){
        str[size-1]='\0';

	}
	return str;

}

bool stringControl(char* str){
	char firstletter = str[0];
	char lastletter = str[strlen(str)-1];

	if(("%d",firstletter)==("%d",'"') && ("%d",lastletter)==("%d",'"')){
		return true;
	}
	else{
		return false;
	}
}


char commentLine[100]; char a[100]; char b[100];
void isComment(char* str) {
	bool trueComment = false;
	int j;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '{') {
			for (j = i; j < strlen(str); j++) {
				if (str[j] == '}') {
					substring(str, i + 1, j - i - 1, commentLine);

					substring(str, 0, i, a);
					substring(str, j + 1, strlen(str), b);
					strcat(a, b);
					strcpy(str, a);

					trueComment = true;

					isComment(str);
				}
			}
			if (!trueComment) {
				exit(0);
			}
		}
	}
}

int substring(char *source, int from, int n, char *target) {
	int length, i;
	for (length = 0; source[length] != '\0'; length++);
	if (from > length) {
		printf("Starting index is invalid.\n");
		return 1;
	}
	if ((from + n) > length) {
		n = (length - from);
	}
	for (i = 0; i < n; i++) {
		target[i] = source[from + i];
	}
	target[i] = '\0';
	return 0;
}


void parse(){

    char code[10000];
    gets(code);
    isComment(code);

    int lineCount = 0;
	char *line[1000];
	char *splitCode = strtok(code, ".");
	while (splitCode != NULL) {
		line[lineCount++] = splitCode;
		splitCode = strtok(NULL, ".");
	}

	for (int i = 0; i < lineCount; i++) {
		int wordCount = 0;
		char *word[1000];
		char *splitLine = strtok(line[i], " \n\r[]\"");
		while (splitLine != NULL) {
			word[wordCount++] = splitLine;
			splitLine = strtok(NULL, " \n\r");}


        if(word[0]==NULL){
            printf("ERROR : Command Line Empty. \n");
            return 0 ;
        }

        // Int => Variable => End of Line
        else if(!strcmp(word[0], "int")){

                //NULL
                if (word[1] == NULL) {
                printf(" \n");
				printf("ERROR :  Variable is expected.\n");
				return 0;
			    }
                //Keyword
                else if(isKeyword(word[1])){
                    printf("ERROR : %s is a keyword. Variable expected. \n" , word[1]);
                    return 0 ;
                }
                else if(isVariable(isalnumControl(word[1]))){
                    //No need to any error message.
                    variableList[vIndex++]= word[1];
                    variableValueList[iIndex++] = 0;
                }
                // Something
                else{
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[1]);
				    return 0;
                }
                //Pattern of keyword int is "Int => Variable => End of Line" must be 2 words. If word count shows more than 2 words, It means user forgot to use "."(End of Line)
                if(wordCount > 2 ){
                    printf("ERROR : End of Line Expected. \n");
                }

        }

        //Move => Identifier  => to =>Identifier
		//Move => InsConstant => to =>Identifier
        else if(!strcmp(word[0], "move")){
                int Int = 0 ;
                if(word[1]==NULL){
                    printf("ERROR : Variable or IntConstant Expected. \n");
                    return 0 ;
                }
                else if(isKeyword(word[1])){
                    printf("ERROR : %s is a keyword. Variable expected. \n" , word[1]);
                    return 0 ;
                }
                else if (isInteger(word[1])){
                    Int = atoi(word[1]);
                }
                else if (isIntegerVariable(word[1])){
                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[1])){
                            Int = variableValueList[i];
                            break;
                        }
                    }
                }
                else{
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[1]);
                    return 0 ;
                }

                if(word[2] == NULL){
                    printf("ERROR : Keyword 'to' expected. \n");
                    return 0 ;
                }

                else if (!strcmp(word[2],"to")){
                }

                else if (isKeyword(word[2]) && strcmp(word[2],"to")){
                    printf("ERROR : Keyword '%s' found but Keyword 'to' expected \n",word[2]);
                    return 0 ;
                }
                else if (isVariable(word[2])){
                    printf("ERROR : %s variable found , Keyword 'to' expected. \n", word[2]);
                    return 0 ;
                }
                else {
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[2]);
                    return 0;
                }

                if(word[3] == NULL){
                    printf("ERROR : Variable expected. \n");
                    return 0;
                }
                else if(isIntegerVariable(word[3])){
                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[3])){
                            variableValueList[i]=Int;
                        }
                    }
                }
                else if (isKeyword(word[3])){
                    printf("ERROR : Keyword '%s' found but variable expected. \n",word[3]);
                    return 0 ;

                }
                else if (isInteger(word[3])){
                    printf("ERROR : IntConstant '%s' found but variable expected. \n", word[3]);
                    return 0 ;
                }
                else{
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[3]);
                    return 0 ;
                }

                if(wordCount > 4 ){
                    printf("ERROR : End of Line expected. \n");
                    return 0;
                }

        }

        //Add => IntConstant => to => InsConstant
	    //Add => IntConstant => to => Identifier
	    //Add => Identifier  => to => InsConstant
	    //Add => Identifier  => to => Identifier

        else if(!strcmp(word[0],"add")){
                int Int = 0 ;
                if (word[1] == NULL){
                    printf("ERROR : Variable or IntConstant expected. \n");
                    return 0;
                }
                else if (isInteger(word[1])){
                    Int = atoi(word[1]);
                }
                else if (isIntegerVariable(word[1])){
                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[1])){
                            Int = variableValueList[i];
                            break;
                        }
                    }
                }
                else if (isKeyword(word[1])){
                    printf("ERROR : Keyword '%s' found but Variable or IntConstant. \n");
                    return 0;
                }
                else {
                    printf("ERROR : Cannot resolve symbol '%s'. \n",word[1]);
                    return 0;
                }

                if (word[2]==NULL){
                    printf("ERROR : Keyword 'to' expected . \n");
                }

                else if (!strcmp(word[2],"to")){

                }
                else if (isKeyword(word[2]) && strcmp(word[2],"to")){
                    printf("ERROR : Keyword '%s' found but Keyword 'to' expected \n",word[2]);
                    return 0 ;
                }

                else if (isVariable(word[2])){
                    printf("ERROR : %s variable found , Keyword 'to' expected. \n", word[2]);
                    return 0 ;
                }

                else {
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[2]);
                    return 0;
                }

                if (word[3] == NULL){
                    printf("ERROR : Variable or IntConstant expected. \n");
                    return 0;
                }

                else if (isIntegerVariable(word[3])){
                    for (int i = 0 ; i < vIndex ; i++){
                            if (!strcmp(variableList[i], word[3])) {
                                variableValueList[i] += Int;
                                break;}

                    }
                }

                else if (isInteger(word[3])){
                    printf("ERROR : IntConstant '%s' found but Variable expected. \n",word[3]);
                    return 0;
                }

                else if (isKeyword(word[3])){
                    printf("ERROR : Keyword '%s' found but Variable expected. \n",word[3]);
                    return 0;
                }

                else {
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[3]);
                    return 0;
                }

                if (wordCount > 4 ){
                    printf("ERROR : End of Line expected. \n");
                    return 0;
                }




        }
        else if(!strcmp(word[0],"sub")){
                int Int = 0 ;
                if (word[1] == NULL){
                    printf("ERROR : Variable or IntConstant expected. \n");
                    return 0;
                }
                else if (isInteger(word[1])){
                    Int = atoi(word[1]);
                }
                else if (isIntegerVariable(word[1])){
                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[1])){
                            Int = variableValueList[i];
                            break;
                        }
                    }
                }
                else if (isKeyword(word[1])){
                    printf("ERROR : Keyword '%s' found but Variable or IntConstant. \n");
                    return 0;
                }
                else {
                    printf("ERROR : Cannot resolve symbol '%s'. \n",word[1]);
                    return 0;
                }

                if (word[2]==NULL){
                    printf("ERROR : Keyword 'to' expected . \n");
                    return 0;
                }

                else if (!strcmp(word[2],"from")){

                }
                else if (isKeyword(word[2]) && strcmp(word[2],"to")){
                    printf("ERROR : Keyword '%s' found but Keyword 'from' expected \n",word[2]);
                    return 0 ;
                }

                else if (isVariable(word[2])){
                    printf("ERROR : %s variable found , Keyword 'from' expected. \n", word[2]);
                    return 0 ;
                }

                else {
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[2]);
                    return 0;
                }

                if (word[3] == NULL){
                    printf("ERROR : Variable or IntConstant expected. \n");
                    return 0;
                }

                else if (isIntegerVariable(word[3])){
                    for (int i = 0 ; i < vIndex ; i++){
                            if (!strcmp(variableList[i], word[3])) {
                                variableValueList[i] -= Int;
                                break;}

                    }
                }

                else if (isInteger(word[3])){
                    printf("ERROR : IntConstant '%s' found but Variable expected. \n",word[3]);
                    return 0;
                }

                else if (isKeyword(word[3])){
                    printf("ERROR : Keyword '%s' found but Variable expected. \n",word[3]);
                    return 0;
                }

                else {
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[3]);
                    return 0;
                }

                if (wordCount > 4 ){
                    printf("ERROR : End of Line expected. \n");
                    return 0;
                }

        }
        else if(!strcmp(word[0],"out")){
                if(word[1] == NULL){
                    printf("ERROR : Nothing found . \n");
                    return 0;
                }

                else if (isInteger(word[1])){
                    printf("%s ",word[1]);
                }
                else if (isIntegerVariable(word[1])){
                    int tempInt = 0 ;

                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[1])){
                            tempInt = variableValueList[i];
                        }
                    }
                    printf("%d ",tempInt);
                }
                else if (!strcmp(word[1],"newline")){
                    printf("\n");

                }
                else if(stringControl(word[1])){
                    printf("%s ",word[1]);
                }
                else if (("%d",word[1][0])==("%d",'"')){
                    int last = 0;
                    for(int j = 0 ; j < wordCount ; j++){
                        if(("%d",word[j][strlen(word[j])-1])==("%d",'"')){
                            last = j;
                            for(int i = 1 ; i<j+1 ;i++ ){
                                printf("%s ",word[i]);
                            }
                        }

                    }
                    if(last == 0 ){
                        printf("ERROR : ' %c ' expected. ",34);
                        return 0 ;
                    }

                    if(!strcmp(word[last+1],",")){

                    }

                    else if (isKeyword(word[last+1])){
                        printf("ERROR : %s is a keyword but ' , ' required. \n",word[last+1]);
                        return 0 ;
                    }

                    else if (isInteger(word[last+1])){
                        printf("ERROR : %s is a IntConstant but ' , ' required . \n ",word[last+1]);
                        return 0 ;
                    }

                    else if(isIntegerVariable(word[last+1])){
                        printf("ERROR : %s is a Variable but ' , ' required. \n",word[last+1]);
                        return 0;
                    }
                    else{
                        printf("ERROR : Unrecognized token. \n");
                    }

                    if(isKeyword(word[last+2])){
                        printf("ERROR : %s is a keyword. \n");
                        return 0;
                    }
                    else if (isInteger(word[last+2])){
                        printf(" %s ",word[last+2]);
                    }

                    else if(isIntegerVariable(word[last+2])){
                        int tempInt = 0 ;
                        for(int i = 0 ; i < vIndex ; i++){
                            if(!strcmp(word[i],word[last+2])){
                                tempInt = variableValueList[i];
                            }
                        }
                        printf("% d ",tempInt);

                    }
                    else if(stringControl(word[last+2])){
                        printf("%s ",word[last+2]);}

                    else if (("%d",word[last+2][0])==("%d",'"')){
                    int last = 0;
                    for(int j = last ; j < wordCount ; j++){
                        if(("%d",word[j][strlen(word[j])-1])==("%d",'"')){
                            last = j;
                            for(int i = last+2 ; i<j+1 ;i++ ){
                                printf("%s ",word[i]);
                            }
                        }

                    }
                        if(last == 0 ){
                           printf("ERROR : ' %c ' expected. ",34);
                           return 0 ;
                    }

                }
                     else{
                        printf("ERROR : Unrecognized token. \n");
                        return 0;
                }


                }

                else {
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[1]);
                    return 0;

                }

                if(!strcmp(word[2],",")){

                }

                else if (isKeyword(word[2])){
                    return 0;
                }
                else if(isIntegerVariable(word[2])){
                    printf("ERROR : required ' , ' . \n");
                    return 0;
                }

                else if (isInteger(word[2])){
                    printf("ERROR : required ' , ' . \n");
                    return 0;
                }

                if(stringControl(word[3])){
                    printf("%s ",word[3]);
                }

                 else if (isInteger(word[3])){
                    printf("%s ",word[3]);
                 }

                 else if (isIntegerVariable(word[3])){
                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[3])){
                            printf("%d",variableValueList[i]);
                        }
                    }
                 }

                else if (("%d",word[3][0])==("%d",'"')){
                    int last = 0;
                    for(int j = 0 ; j < wordCount ; j++){
                        if(("%d",word[j][strlen(word[j])-1])==("%d",'"')){
                            last = j;
                            for(int i = 3 ; i<j+1 ;i++ ){
                                printf("%s ",word[i]);
                            }
                        }

                    }
                    if(last == 0 ){
                        printf("ERROR : ' %c ' expected. ",34);
                        return 0 ;
                    }

                }
                else{
                    printf("ERROR : Unrecognized token. \n");
                    return 0;
                }

        }

        else if(!strcmp(word[0],"loop")){
                int loopTime = 0 ;
                if (isInteger(word[1])){
                    loopTime = atoi(word[1]);
                }

                else if (isIntegerVariable(word[1])){
                    for(int i = 0 ; i < vIndex ; i++ ){
                        if(!strcmp(variableList[i],word[1])){
                            loopTime = variableValueList[i];
                        }
                    }
                }

                else if (isKeyword(word[1])){
                    printf("ERROR : '%s' is a keyword. Variable or IntConstant expected. ",word[1]);
                    return 0 ;
                }

                else{
                    printf("ERROR : Unexpected token. Variable or IntConstant expected. ");
                    return 0;
                }

                //Word 2
                if (isInteger(word[2])){
                    printf("ERROR : '%s' is a IntConstant. Keyword times expected . " , word[2]);
                    return 0 ;
                }

                else if (isIntegerVariable(word[2])){
                    printf("ERROR : '%s' is a variable. Keyword times expected . " , word[2]);
                    return 0 ;
                }

                else if (isKeyword(word[2]) && (strcmp(word[2],"times"))){
                    printf("ERROR : '%s' is a keyword but Keyword times expected . " , word[2]);
                    return 0 ;
                }

                else if (!strcmp(word[2],"times")){

                }

                else if(word[2] == NULL){
                    printf("ERROR : Keyword times expected. ");
                    return 0;
                }

                else{
                    printf("ERROR : Unrecognized token. Keyword times expected . ");
                    return 0 ;

                }

                //Word 3

                for(int i = 0 ; i < loopTime ; i++){
                  if(!strcmp(word[3],"add")){
                    int tempInt = 0 ;

                    //Word 4
                    if(word[4] == NULL){
                        printf("ERROR : IntConstant or variable expected. ");
                        return 0 ;
                    }

                    else if (isKeyword(word[4])){
                        printf("ERROR: '%s' is a keyword. IntConstant or variable expected.",word[4]);
                        return 0;
                    }

                    else if (isInteger(word[4])){
                        tempInt = atoi(word[4]);
                    }

                    else if (isIntegerVariable(word[4])){
                        for(int i = 0 ; i < vIndex ; i++){
                                if(!strcmp(variableList[i],word[4])){
                                    tempInt = variableValueList[i];
                                }
                        }
                    }

                    else {
                        printf("ERROR : Unexpected token. IntConstant or variable expected.");
                        return 0 ;
                    }

                    //Word 5
                    if(isKeyword(word[5]) && strcmp(word[5],"to")){
                        printf("ERROR : '%s' is a keyword but keyword 'to' expected. ",word[5]);
                        return 0 ;
                    }

                    else if (word[5] == NULL){
                        printf("ERROR : Keyword to expected. ");
                        return 0;
                    }

                    else if (isInteger(word[5])){
                        printf("ERROR : '%s' is a IntConstant. Keyword to expected.",word[5]);
                        return 0 ;
                    }

                    else if (isIntegerVariable(word[5])){
                        printf("ERROR : '%s' is a variable. Keyword to expected.",word[5]);
                        return 0 ;
                    }

                    else if (!strcmp(word[5],"to")){
;
                    }

                    //word 6

                    if (word[6] == NULL){
                        printf("ERROR : Variable expected. ");
                        return 0;
                    }

                    else if (isKeyword(word[6])){
                        printf("ERROR : '%s' is a keyword. Variable expected. ",word[6]);
                        return 0;
                    }

                    else if(isInteger(word[6])){
                        printf("ERROR : '%s' is a IntConstant. Variable expected. ",word[6]);
                        return 0;
                    }

                    else if(isIntegerVariable(word[6])){

                        for(int i = 0 ; i < vIndex ; i++){
                            if(!strcmp(variableList[i],word[6])){
                                variableValueList[i]+=tempInt;
                            }
                        }
                    }

                }

                else if(!strcmp(word[3],"sub")){
                    int subInt = 0 ;

                    //WORD 4
                    if(word[4] == NULL){
                        printf("ERROR : Variable or IntConstant expected.");
                        return 0;
                    }

                    else if (isInteger(word[4])){
                        subInt = atoi(word[4]);
                    }

                    else if (isIntegerVariable(word[4])){
                        for(int i = 0 ; i < vIndex ; i++){
                            if(!strcmp(variableList[i],word[4])){
                                subInt = variableValueList[i];
                            }
                        }
                    }

                    else if (isKeyword(word[4])){
                        printf("ERROR : Keyword '%s' found. Variable or IntConstant expected.",word[4]);
                        return 0;
                    }

                    else{
                        printf("ERROR : Unexpected token.");
                        return 0;
                    }

                    //WORD 5
                    if(word[5] == NULL){
                        printf("ERROR : Keyword 'from' expected.");
                        return 0;
                    }

                    else if (isInteger(word[5])){
                        printf("ERROR : IntConstant '%s' found. Keyword 'from' expected. ",word[5]);
                        return 0;
                    }

                    else if (isIntegerVariable(word[5])){
                        printf("ERROR : Variable '%s' found. Keyword 'from' expected. ",word[5]);
                        return 0;
                    }

                    else if (isKeyword(word[5]) && strcmp(word[5],"from")){
                        printf("ERROR : Keyword '%s' found but Keyword 'from' expected. ",word[5]);
                        return 0;
                    }

                    else if (!strcmp(word[5],"from")){

                    }

                    else{
                        printf("ERROR : Unexpected token. Keyword 'from' expected. ");
                        return 0;
                    }

                    //WORD 6

                    if(word[6] == NULL){
                        printf("ERROR : Variable expected. ");
                        return 0;
                    }

                    else if (isInteger(word[6])){
                        printf("ERROR : IntConstant '%s' found. Variable expected. ",word[6]);
                        return 0;
                    }

                    else if (isIntegerVariable(word[6])){
                        for(int i = 0 ; i < vIndex ; i++){
                            if(!strcmp(variableList[i],word[6])){
                                variableValueList[i]-=subInt;
                            }
                        }
                    }

                    else if (isKeyword(word[6])){
                        printf("ERROR : Keyword '%s' found. Variable expected.");
                        return 0 ;
                    }

                    else{
                        printf("ERROR : Unexpected token. Variable expected. ");
                        return 0;
                    }

                }

               else if(!strcmp(word[3],"out")){
                 if(word[1] == NULL){
                    printf("ERROR : Nothing found . \n");
                    return 0;
                }

                else if (isInteger(word[4])){
                    printf("%s ",word[4]);
                }
                else if (isIntegerVariable(word[4])){
                    int tempInt = 0 ;

                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[1])){
                            tempInt = variableValueList[i];
                        }
                    }
                    printf("%d ",tempInt);
                }
                else if (!strcmp(word[4],"newline")){
                    printf("\n");

                }
                else if(stringControl(word[4])){
                    printf("%s ",word[4]);
                }
                else if (("%d",word[4][0])==("%d",'"')){
                    int last = 0;
                    for(int j = 0 ; j < wordCount ; j++){
                        if(("%d",word[j][strlen(word[j])-1])==("%d",'"')){
                            last = j;
                            for(int i = 1 ; i<j+1 ;i++ ){
                                printf("%s ",word[i]);
                            }
                        }

                    }
                    if(last == 0 ){
                        printf("ERROR : ' %c ' expected. ",34);
                        return 0 ;
                    }

                    if(!strcmp(word[last+1],",")){

                    }

                    else if (isKeyword(word[last+1])){
                        printf("ERROR : %s is a keyword but ' , ' required. \n",word[last+1]);
                        return 0 ;
                    }

                    else if (isInteger(word[last+1])){
                        printf("ERROR : %s is a IntConstant but ' , ' required . \n ",word[last+1]);
                        return 0 ;
                    }

                    else if(isIntegerVariable(word[last+1])){
                        printf("ERROR : %s is a Variable but ' , ' required. \n",word[last+1]);
                        return 0;
                    }
                    else{
                        printf("ERROR : Unrecognized token. \n");
                    }

                    if(isKeyword(word[last+2])){
                        printf("ERROR : %s is a keyword. \n");
                        return 0;
                    }
                    else if (isInteger(word[last+2])){
                        printf(" %s ",word[last+2]);
                    }

                    else if(isIntegerVariable(word[last+2])){
                        int tempInt = 0 ;
                        for(int i = 0 ; i < vIndex ; i++){
                            if(!strcmp(word[i],word[last+2])){
                                tempInt = variableValueList[i];
                            }
                        }
                        printf("% d ",tempInt);

                    }
                    else if(stringControl(word[last+2])){
                        printf("%s ",word[last+2]);}

                    else if (("%d",word[last+2][0])==("%d",'"')){
                    int last = 0;
                    for(int j = last ; j < wordCount ; j++){
                        if(("%d",word[j][strlen(word[j])-1])==("%d",'"')){
                            last = j;
                            for(int i = last+2 ; i<j+1 ;i++ ){
                                printf("%s ",word[i]);
                            }
                        }

                    }
                        if(last == 0 ){
                           printf("ERROR : ' %c ' expected. ",34);
                           return 0 ;
                    }

                }
                     else{
                        printf("ERROR : Unrecognized token. \n");
                        return 0;
                }


                }

                else {
                    printf("ERROR : Cannot resolve symbol '%s' \n",word[1]);
                    return 0;

                }

                if(!strcmp(word[5],",")){

                }

                else if (isKeyword(word[5])){
                    return 0;
                }
                else if(isIntegerVariable(word[5])){
                    printf("ERROR : required ' , ' . \n");
                    return 0;
                }

                else if (isInteger(word[5])){
                    printf("ERROR : required ' , ' . \n");
                    return 0;
                }

                if(stringControl(word[6])){
                    printf("%s ",word[6]);
                }

                 else if (isInteger(word[6])){
                    printf("%s ",word[6]);
                 }

                 else if (isIntegerVariable(word[6])){
                    for(int i = 0 ; i < vIndex ; i++){
                        if(!strcmp(variableList[i],word[6])){
                            printf("%d",variableValueList[i]);
                        }
                    }
                 }

                else if (("%d",word[6][0])==("%d",'"')){
                    int last = 0;
                    for(int j = 0 ; j < wordCount ; j++){
                        if(("%d",word[j][strlen(word[j])-1])==("%d",'"')){
                            last = j;
                            for(int i = 3 ; i<j+1 ;i++ ){
                                printf("%s ",word[i]);
                            }
                        }

                    }
                    if(last == 0 ){
                        printf("ERROR : ' %c ' expected. ",34);
                        return 0 ;
                    }

                }
                else{
                    printf("ERROR : Unrecognized token. \n");
                    return 0;
                }

        }

        }


        }
        else{
            printf("Unexpected Command \n");
            return 0 ;
        }

		}




}


int main()
{
    parse();

}
