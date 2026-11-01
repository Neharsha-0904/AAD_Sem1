import heapq
def min_rooms(intervals):
    if not intervals: return 0
    intervals=sorted(intervals)
    ends=[]
    for start,end in intervals:
        if ends and ends[0] <= start: heapq.heappop(ends)
        heapq.heappush(ends,end)
    return len(ends)
if __name__=="__main__": print(min_rooms([(0,30),(5,10),(15,20)]))
