
def insert(a, x):
    a.append(x)
    place = (len(a) - 1)
    ancestor = place // 2
    while(place > 1 and a[ancestor] < x):
        a[place] = a[ancestor]
        place = ancestor
        ancestor = place // 2
    a[place] = x
    #print(a)



def extract(a):
    if len(a) <= 1:
        return
    x = a[1]
    a[1] = a[len(a) - 1]
    a = a[:-1]
    if len(a) <= 1:
        return x, a
    k = a[1]
    place = 1
    
    while((place * 2 < len(a) and a[place * 2] > k) or (place * 2 + 1 < len(a) and a[place * 2 + 1] > k)):
        if place * 2 < len(a) and a[place * 2] > k and (place * 2 + 1 >= len(a) or a[place * 2] >= a[place * 2 + 1]):
            a[place] = a[place * 2]
            place = place * 2
        else:
            if place * 2 + 1 < len(a) and a[place * 2 + 1] > k and a[place * 2 + 1] > a[place * 2]:
                a[place] = a[place * 2 + 1]
                place = place * 2 + 1
    a[place] = k
    print(x, a)
    return x, a

def adjust_heap(heap, k):
    length = len(heap)
    cont = 1
    while(k <= (length - 1) // 2 and cont == 1):
        if(heap[k] < heap[2 * k]):
            if(2 * k + 1 >= length):
                temp = heap[k]
                heap[k] = heap[2 * k]
                heap[2 * k] = temp
                k = 2 * k
            else:
                if(heap[2 * k] >= heap[2 * k + 1]):
                    temp = heap[k]
                    heap[k] = heap[2 * k]
                    heap[2 * k] = temp
                    k = 2 * k
                else:
                    temp = heap[k]
                    heap[k] = heap[2 * k + 1]
                    heap[2 * k + 1] = temp
                    k = 2 * k + 1

        else:
            if(2 * k + 1 <= length):
                if(heap[k] < heap[2 * k + 1]):
                    temp = heap[k]
                    heap[k] = heap[2 * k + 1]
                    heap[2 * k + 1] = temp
                    k = 2 * k + 1
                else:
                    cont = 0
            else:
                cont = 0
    print(heap)
    return heap
        
def heap_sort(a):
    heap = [0] + a[:]
    k = (len(heap) - 1) // 2 
    while(k > 0):
        heap = adjust_heap(heap, k)
        k -= 1
    print(heap)
    i = len(a)
    while(i > 0):
        x, heap = extract(heap)
        a[len(a) - i] = x
        i -= 1
    print(a)
        

a = [4, 1, 3, 4, 5, 7, 2, 6, 8, 9]
heap_sort(a)