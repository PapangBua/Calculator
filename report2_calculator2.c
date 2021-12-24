#include <stdio.h>
#define MAX_SIZE 1000
#define MIN_NUM -2147483648
char ch[MAX_SIZE],j; /* 1 文字先読み用 */
int chlen = 0;
/** プロトタイプ宣言 ***/
double expression();
double term();
double factor();
double factor2();
double constant();
double power();
int stop0=-1,stoperr=-1;
/*** 関数本体 ***/
double power(double a,int b){
    double sum = 1.0;
    for(int i=0;i<b;i++) sum*=a;
    return sum;
}
double expression(int start,int end) {
    if(stop0!=-1 || stoperr!=-1) return 0;
    if(start>end) {
            stoperr = start;
            return MIN_NUM;
    }
    int bracketno, lastposition=start-1; //collect last position with +/-
    double sum=0.0,temp=0;
    for(int i=start;i<=end;i++){
        if(ch[i]=='(') bracketno++;
        else if(ch[i]==')') {
                bracketno--;
                if(bracketno<0){
                    stoperr = i;
                    return 0;
                }
        }
        else if(i==start && ch[i]=='-');
        else if((ch[i]=='+' || ch[i]=='-')&& bracketno==0){
            temp = term(lastposition+1,i-1);
            if(temp!= MIN_NUM){
                if(lastposition==start-1 || ch[lastposition]=='+'){
                    sum+= temp;
                    lastposition = i;
                }
                else if(lastposition!=start-1 && ch[lastposition]=='-'){
                    sum-= temp;
                    lastposition = i;
                }
            }
        }
        else if(ch[i]>='0' && ch[i]<='9');
        else if(ch[i]=='+' || ch[i]=='-' || ch[i]=='*' || ch[i]=='/' || ch[i]=='.' || ch[i]=='^');
        else{
                stoperr=i;
                return 0;
        }
        printf("",temp);
    }
    if(lastposition!=start-1){
            temp = term(lastposition+1,end);
            if(temp!=MIN_NUM){
                if(lastposition==start-1 || ch[lastposition]=='+'){
                    sum+= temp;
                }
                else if(lastposition!=start-1 && ch[lastposition]=='-'){
                    sum-= temp;
                }
            }
    }
    else{
        sum= term(start,end);
    }
    if(bracketno!=0){
        for(int i=start;i<=end;i++){
            if(ch[i]=='(') stoperr = i;
            return 0;
        }
    }
    return sum;
}
double term(int start,int end) {
    if(stop0!=-1 || stoperr!=-1) return 0;
    if(start>end) {
            stoperr = start;
            return MIN_NUM;
    }
    int bracketno, lastposition=start-1,k; //collect last position with +/-
    double sum=1.0,temp=0.0;
    printf("",temp);
    for(int i=start;i<=end;i++){
        if(ch[i]=='(') bracketno++;
        else if(ch[i]==')') bracketno--;
        else if((ch[i]=='*' || ch[i]=='/')&& bracketno==0){
            temp = factor(lastposition+1,i-1);
            if(temp!=MIN_NUM){
                if(lastposition==start-1 || ch[lastposition]=='*'){
                    sum*= temp;
                    lastposition = i;
                }
                else if(lastposition!=start-1 && ch[lastposition]=='/'){
                    if(temp==0){
                        stop0=1;
                        return 0;
                    }
                    sum/= temp;
                    lastposition = i;
                }
            }
        }
    }
    if(lastposition!=start-1){
            temp = factor(lastposition+1,end);
            if(temp!=MIN_NUM){
                if(lastposition==start-1 || ch[lastposition]=='*'){
                    sum*= temp;
                }
                else if(lastposition!=start-1 && ch[lastposition]=='/'){
                    if(temp==0){
                        stop0=1;
                        return 0;
                    }
                    sum/= temp;
                }
            }
    }
    else{
        sum= factor(start,end);
    }
    return sum;
}
double factor(int start,int end) { // power
    if(stop0!=-1 || stoperr!=-1) return 0;
    if(start>end) {
            stoperr = start;
            return MIN_NUM;
    }
    int bracketno, k; //collect last position with +/-
    double sum=1.0,temp1,temp2,done=0;
    for(int i=start;i<=end;i++){
        if(ch[i]=='(') bracketno++;
        else if(ch[i]==')') bracketno--;
        else if(ch[i]=='^'&& bracketno==0){
            temp1 = factor2(start,i-1);
            temp2 = factor2(i+1,end);
            if(temp1!=MIN_NUM && temp2!=MIN_NUM){
                    sum = power(temp1,temp2);
                    done = 1;
                    break;
            }
        }
    }
    if(done!=1){
        sum= factor2(start,end);
    }
    return sum;
}
double factor2 (int start,int end){
    if(stop0!=-1 || stoperr!=-1) return 0;
    double k = constant(start,end);
    if(k!=MIN_NUM) return k;
    return expression(start+1,end-1);
}
double constant(int start,int end){
    if(stop0!=-1 || stoperr!=-1) return 0;
    int dot = 0,minus = 0;
    double ans = 0.0;
    if(ch[start]=='-') {
            minus = 1;
            start++;
    }
    for(int i=start;i<=end;i++){
        if(ch[i]=='.' && dot==0) dot+=1;
        else if(ch[i]=='.' && dot!=0){
            stoperr = i;
            return 0;
        }
        else dot*=10;
        if(ch[i]>='0' && ch[i]<='9') {
            ans*=10;
            ans+=(ch[i]-'0');
        }
        else if(ch[i]!='.'){
            return MIN_NUM;
        }
    }
    if(minus==1) ans*=-1;
    if(dot==0) return ans;
    return ans/dot;
}
void main(){
    double temp;
    printf("Input form : ");
    fgets(ch,MAX_SIZE,stdin);
    for(j=0;ch[j]!='\0';j++);
    chlen = j-1;
    temp = expression(0,chlen-1);
    if(stop0!=-1) printf("Number cannot be divided by zero\n");
    else if(stoperr!=-1) printf("Error on letter: %d ",stoperr+1);
    else if(temp-(int)(temp)==0) printf("Ans = %d\n",(int)(temp));
    else printf("Ans = %lf\n",temp);
}
