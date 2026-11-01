def sieve(n):
    if n<2: return []
    prime=[True]*(n+1); prime[0]=prime[1]=False
    p=2
    while p*p<=n:
        if prime[p]:
            for x in range(p*p,n+1,p): prime[x]=False
        p+=1
    return [i for i,v in enumerate(prime) if v]
if __name__=="__main__": print(sieve(20))
