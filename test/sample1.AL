func divides(n: int, divisor: int): int {
    var i = 1;
    while (!(i > n)) {
        const var d = divisor * i;

        if (d == n) {
            return 1;
        }

        i = i + 1;
    }

    return 0.0;
}

func isPrime(x: int): int {
    if (x == 0 || x == 1) {
        return 0;
    }

    var i = 2;
    while (!(i > x / 2)) {
        if (divides(x, i)) {
            return 0;
        }
        
        i = i + 1;
    }

    return 1;
}

// main function
func main(): void {
    var n = 1;
    var cnt = 0;

    while (cnt < 10) {
        if (isPrime(n)) {
            print(n);
            cnt = cnt + 1;
        }

        n = n + 1;
    }
}