////////////////////////////////////////////////////////////////////////////////////
// PA 2
// Problem 1: A Verified Heap 70%
////////////////////////////////////////////////////////////////////////////////////

datatype Heap = H(a: array<int>, size: int, capacity: int)

predicate valid_heap(h: Heap)
	reads h.a
{
	h.a != null && h.a.Length == h.capacity + 1 && 0 <= h.size <= h.capacity &&
	    forall i :: 1 < i <= h.size ==> h.a[i] <= h.a[i / 2]
}


method init_heap(cap: int) returns (h: Heap)
	requires 0 <= cap
	ensures valid_heap(h)
	ensures h.size == 0 && h.capacity == cap
	ensures fresh(h.a)
{
	// TODO: Fill in the body to satisfy the specification
	var a := new int[cap + 1];
	var size := 0;
	var capacity := cap;
	h := H(a, size, capacity);
}

method insert(x: int, h: Heap) returns (h': Heap)
	requires valid_heap(h)
	requires h.size < h.capacity
	ensures valid_heap(h')
	ensures h'.size == h.size + 1 && h'.capacity == h.capacity
	//ensures h.a == h'.a
	//modifies h.a
	ensures fresh(h'.a)
{
	// TODO: Fill in the body to satisfy the specification
	var size := h.size + 1;
	var cap := h.capacity;
	var a := new int[cap + 1];
	var i := 1;
	while (i <= h.size)
		decreases h.size - i
		invariant forall j :: 1 <= j < i ==> j <= h.size && a[j] == h.a[j]
	{
		a[i] := h.a[i];
		i := i + 1;
	}
	assert forall i :: 1 <= i <= h.size ==> a[i] == h.a[i];
	a[size] := a[size / 2];

	var current_place := size;
	//如果父亲比自己小，就往上传导
	while(current_place > 1 && x > a[current_place / 2])
		invariant 1 <= current_place <= h.size + 1 //有界
		invariant forall i :: 1 < i <= h.size + 1 ==> a[i] <= a[i / 2]
		invariant forall i :: 1 < i <= h.size + 1 && i/2 == current_place ==> a[i] <= x
		decreases current_place
	{
		a[current_place] := a[current_place / 2];
		current_place := current_place / 2;
	}
	a[current_place]:= x;
	h' := H(a, size, cap);
}


lemma max_heap(h: Heap)
	requires valid_heap(h)
	requires h.size >= 1
	ensures forall i :: 1 < i <= h.size ==> h.a[i] <= h.a[1]
{
	assert forall i {:induction i} :: 1 < i <= h.size ==> ancestor (i, 1);
	assert forall i, j {:induction i, j} :: 1 <= i < j <= h.size && ancestor(j, i) ==> h.a[j] <= h.a[i];
}

predicate ancestor(a: int, b: int)
	decreases a
	requires 0 < a && 0 < b
{
	b < a && (b == a / 2 || ancestor(a/2, b))
}


method extract(h: Heap) returns (x: int, h': Heap)
	requires valid_heap(h)
	requires 0 < h.size
	ensures valid_heap(h')
	ensures h'.size == h.size - 1
	ensures forall i :: 1 <= i <= h'.size ==> h'.a[i] <= x
	ensures x == h.a[1]
	ensures fresh(h'.a)
{
	// TODO: Fill in the body to satisfy the specification
	max_heap(h);
	x := h.a[1];
	var a := new int[h.capacity + 1];
	var i := 1;
	while (i <= h.size)
		decreases h.size - i
		invariant forall j :: 1 <= j < i ==> j <= h.size && a[j] == h.a[j]
	{
		a[i] := h.a[i];
		i := i + 1;
	}
	assert forall i :: 1 <= i <= h.size ==> a[i] == h.a[i];
	if(h.size == 1)
	{
		h' := H(a, h.size - 1, h.capacity);
		return;
	}
	var pivot := a[h.size];
	var place := 1;
	//如果儿子有比自己大的，就往最大的儿子传导，注意考虑边界情况
  	while((place * 2 < h.size && a[place * 2] >= pivot) || (place * 2 + 1 < h.size && a[place * 2 + 1] >= pivot))
	invariant 1 <= place < h.size //不越界
	invariant forall i :: 1 < i < h.size ==> a[i] <= a[i / 2] //除了place之外都符合堆性质
	invariant forall i :: 1 <= i <= h.size ==> a[i] <= x
	invariant a[place] >= pivot
	decreases h.size - place * 2
	{
		//左儿子比自己大，而且是更大的儿子（右儿子小或者越界）
    	if (place * 2 < h.size && a[place * 2] >= pivot && (place * 2 + 1 >= h.size || a[place * 2] >= a[place * 2 + 1]))
		{
			assert a[place*2] <= a[(place*2) / 2]; 
      		a[place] := a[place * 2];
      		place := place * 2;
		}
    	else
		{
			//右儿子更大且没有越界
      		if (place * 2 + 1 < h.size && a[place * 2 + 1] >= pivot && a[place * 2 + 1] > a[place * 2])
			{
				assert a[place*2 + 1] <= a[(place*2 + 1) / 2]; 
        		a[place] := a[place * 2 + 1];
        		place := place * 2 + 1;
			}
			//不越界的儿子都不大于自己，这样就修改当前位置然后准备返回
			else
			{
				a[place] := pivot;
				place := h.size - 1;
			}
		}
	}
	//更新儿子
	assert forall i :: 1 < i < h.size && place == i / 2 ==> pivot >= a[i];
	a[place] := pivot;

    h' := H(a, h.size - 1, h.capacity);
}

////////////////////////////////////////////////////////////////////////////////////
// PA 2
// Problem 2: Heapsort 30%
////////////////////////////////////////////////////////////////////////////////////

class Heapsort
{
	var h: Heap;

	method sort(a: array<int>)
		requires 0 <= a.Length
		ensures forall i :: 0 <= i < a.Length - 1 ==> a[i] >= a[i + 1]
		modifies this, h.a, a
	{
		// TODO: Fill in the body to satisfy the specification
		this.h := init_heap(a.Length);
		assert this.h.size == 0;
		if(a.Length == 0) {
			return;
		}
		//插入
		while (this.h.size < a.Length)
		invariant valid_heap(this.h)
		invariant a.Length == this.h.capacity
		decreases a.Length - this.h.size
		{
			assert(this.h.size < this.h.capacity);
			var k := a[this.h.size];
			this.h := insert(k, this.h);
		}
		assert this.h.size == a.Length;

		//提取		
		while(this.h.size >= 1)
		invariant valid_heap(this.h)
		invariant forall i :: 0 <= i < a.Length - this.h.size - 1 ==> a[i] >= a[i + 1]
		invariant forall i :: 0 <= i <= a.Length - this.h.size - 1 && this.h.size > 0 ==> a[i] >= this.h.a[1]
		decreases this.h.size
		{
			var j;
			j, this.h := extract(this.h);
			assert this.h.size < a.Length;
			a[a.Length - this.h.size - 1] := j;
			if(this.h.size > 0)
			{
				assert a[a.Length - this.h.size - 1] >= this.h.a[1];
			}
		}

	}
}
