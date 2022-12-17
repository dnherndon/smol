int five(){
    int x;
    int y;
    x = 5;
    y = x;
    return y;
}
int two(){
    int x;
    x = five();
    return x;
}
int main(){
    int x;
    x = two();
    return x + two() + five();
}