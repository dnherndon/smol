int two(){
    int x;
    x = 2;
    return x;
}
int main(){
    int x;
    x = two();
    return x + two();
}