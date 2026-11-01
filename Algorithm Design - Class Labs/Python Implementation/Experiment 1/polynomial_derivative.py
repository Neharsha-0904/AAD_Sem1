def derivative(terms):
    # terms is a list of (coefficient, exponent)
    return [(c*e, e-1) for c,e in terms if e != 0 and c*e != 0]

def show(terms):
    if not terms: return "0"
    return " + ".join(f"{c}x^{e}" if e > 1 else (f"{c}x" if e == 1 else str(c))
                      for c,e in sorted(terms, key=lambda t:-t[1]))

if __name__ == "__main__":
    polynomial = [(3,2),(2,1),(5,0)]
    print("Derivative:", show(derivative(polynomial)))
