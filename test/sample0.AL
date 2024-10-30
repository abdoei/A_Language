func function_name(num b){
    num a = 5;
    return a * b;
}

func main(){
    num a = 5;
    num b = 6;
    num c = function_name(b);
    return a + c;
}