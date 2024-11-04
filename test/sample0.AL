func function_name(int b): int{
    var a: int = 5;
    return a * b;
}


// main function
func main(){
    var a: int = 5;
    var b: int = 6;
    var c: int = function_name(b);
    return a + c;
}