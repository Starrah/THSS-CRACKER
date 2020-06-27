/////////////////////////////////////////////////////////////////////////////
// Peano Arithmetic
/////////////////////////////////////////////////////////////////////////////

// Important note: DO NOT CHANGE the definitions of Nat, one(), add(x, y), or mult(x, y).
// Turned-in solutions that differ in the definitions will receive no credit!
datatype Nat = Zero | Succ(n: Nat)

function one(): Nat
{
	Succ(Zero)
}

function add(x: Nat, y: Nat): Nat
{
	match(x) {
		case Zero => y
		case Succ(n) => Succ(add(n, y))
	}
}

function mult(x: Nat, y: Nat): Nat
{
	match(x) {
		case Zero => Zero
		case Succ(n) => add(mult(n, y), y)
	}
}

// 2-1
// Add an ensures annotation sufficient
// to prove the statement:
//   forall x . ~(x + 1 = 0)
// Then prove the lemma by providing a body
// sufficient to establish your ensures annotation.
// Note: your ensures annotation should make use
// of the argument passed to the lemma.
lemma {:induction false} axm_zero(x: Nat)
	ensures add(x, one()) != Zero
{
	match(x) 
	{
		case Zero =>
			calc == 
			{
				add(x, one());
				add(Zero, one());
				one();
				!= Zero;
			}
		case Succ(n) =>
			calc == 
			{
				add(x, one());
				add(Succ(n), one());
				Succ(add(n, one()));
				!= Zero;
			}
	}
}
// 2-2
// This is provided as an example, you do not
// need to do anything.
// This lemma proves the statement:
//   forall x . x + 0 == x
// Note that it uses proof by contradiction,
// by assuming that its postcondition is false
// using an if statement, and deriving the
// negation of this assumption on all branches
// within the body.
lemma {:induction false} axm_pluszero(x: Nat)
	ensures add(x, Zero) == x
{
	match(x) 
	{
		case Zero =>
		calc == 
		{
			add(x, Zero);
			add(Zero, Zero);
			Zero;
			x;
		}
		case Succ(n) =>
		calc == 
		{
			add(x, Zero);
			add(Succ(n), Zero);
			Succ(add(n, Zero));
			{ 
				axm_pluszero(n); 
			} // inductive step
			Succ(n);
			x;
		}
	}
}

// 2-3
// Add an ensures annotation sufficient
// to prove the statement:
//   forall x . x * 0 = 0
// Then prove the lemma by providing a body
// sufficient to establish your ensures annotation.
// Note: your ensures annotation should make use
// of the arguments passed to the lemma.
// Note: do not remove the {:induction false} attribute.
// Solutions that do not verify with this attribute
// will recieve no credit!
lemma {:induction false} axm_timeszero(x: Nat)
ensures mult(x, Zero) == Zero
{
	match(x) 
	{
		case Zero =>
			calc == 
			{
				mult(x, Zero);
				mult(Zero, Zero);
				Zero;
			}
		case Succ(n) =>
			calc == 
			{
				mult(x, Zero);
				mult(Succ(n), Zero);
				add(mult(n, Zero), Zero);
				{ 
					axm_timeszero(n); 
				} // inductive step
				add(Zero, Zero);
				Zero;
			}
	}
}
// 2-4
// Add an ensures annotation sufficient
// to prove the statement:
//   forall x, y . x + 1 = y + 1 ==> x = y
// Then prove the lemma by providing a body
// sufficient to establish your ensures annotation.
// Note: your ensures annotation should make use
// of the arguments passed to the lemma.
lemma {:induction false}  axm_successor(x: Nat, y: Nat)	
requires add (x, one()) == add (y, one())
ensures x == y
{
	match(x) 
	{
		case Zero =>
		{
			match(y)
			{
				case Zero =>
					calc == 
					{
						add(x, one());
						add(Zero, one());
						one();
						add(Zero, one());
						add(y, one());
					}
					calc ==
					{
						x;
						Zero;
						y;
					}
				case Succ(n) =>
					calc == 
					{
						add(y, one());
						add(Succ(n), one());
						Succ(add(n, one()));
						!= Succ(Zero);
						{
							add_lemmatwo(Zero);
						}
						add(Zero, one());
						add(x, one());
					}
			}
		}
		case Succ(m) =>
		{
			match(y)
			{
				case Zero =>
					calc == 
					{
						add(x, one());
						add(Succ(m), one());
						Succ(add(m, one()));
						!= Succ(Zero);
						{
							add_lemmatwo(Zero);
						}
						add(Zero, one());
						add(y, one());
					}
				case Succ(n) =>
					calc ==
					{
						add(m, one());
						calc == 
						{
							Succ(add(m, one()));
							add(Succ(m), one());
							add(x, one());
							add(y, one());	
							add(Succ(n), one());
							Succ(add(n, one()));
						}	
						add(n, one());
					}
					{
						axm_successor(m, n);
					}
					calc == 
					{
						m;
						n;
					}
					calc ==
					{
						x;
						Succ(m);
						Succ(n);
						y;
					}
			}
		}
	}
}
// 2-5
// Add an ensures annotation sufficient
// to prove the statement:
//   forall x, y . x + (y + 1) = (x + y) + 1
// Then prove the lemma by providing a body
// sufficient to establish your ensures annotation.
// Note: your ensures annotation should make use
// of the arguments passed to the lemma.
lemma {:induction false} axm_plussuccessor(x: Nat, y: Nat)
ensures add(x, add(y, one())) == add(add(x, y), one())
{
	match(x) 
	{
		case Zero =>
			calc == 
			{
				add(x, add(y, one()));
				add(Zero, add(y, one()));
				add(y, one());
				add(add(Zero, y), one());
				add(add(x, y), one());
			}
		case Succ(n) =>
			calc == 
			{
				add(x, add(y, one()));
				add(Succ(n), add(y, one()));
				Succ(add(n, add(y, one())));
				{ 
					axm_plussuccessor(n, y);
				}
				Succ(add(add(n, y), one()));
				add(Succ(add(n, y)), one());
				add(add(Succ(n), y), one());
				add(add(x, y), one());
			} 
		
	}
}
// This lemma is provided for your benefit.
// It might come in handy in a subsequent proof.
lemma {:induction false} add_lemmatwo(x: Nat)
	ensures add(x, one()) == Succ(x)
{
	match(x)
	{
		case Zero =>
			calc ==
			{
				add(x, one());
				add(Zero, one());
				one();
				Succ(Zero);
				Succ(x);
			}
		case Succ(n) =>
			calc ==
			{
				add(x, one());
				add(Succ(n), one());
				Succ(add(n, one()));
				{
					add_lemmatwo(n);
				}
				Succ(Succ(n));
				Succ(x);
			}
	}
}

// This lemma is provided for your benefit.
// It might come in handy in a subsequent proof.
lemma {:induction false} plus_exchange(x: Nat, y: Nat, z: Nat)
	ensures add(add(x, y), z) == add(add(x, z), y)
{
	match(z)
	{
		case Zero =>
			calc ==
			{
				add(add(x, y), z);
				add(add(x, y), Zero);
				{
					axm_pluszero(add(x, y));
				}
				add(x, y);
				{
					axm_pluszero(x);
				}
				add(add(x, Zero), y);
				add(add(x, z), y);
			}
		case Succ(n) =>
			calc ==
			{
				add(add(x, y), z);
				add(add(x, y), Succ(n));
				{
					add_lemmatwo(n);
				}
				add(add(x, y), add(n, one()));
				{
					axm_plussuccessor(add(x, y), n);
				}
				add(add(add(x, y), n), one());
				{
					plus_exchange(x, y, n);
				}
				add(add(add(x, n), y), one());
				{
					add_lemmatwo(add(add(x, n), y));
				}
				Succ(add(add(x, n), y));
				add(Succ(add(x, n)), y);
				{
					add_lemmatwo(add(x, n));
				}
				add(add(add(x, n), one()), y);
				{
					axm_plussuccessor(x, n);
				}
				add(add(x, add(n, one())), y);
				{
					add_lemmatwo(n);
				}
				add(add(x, Succ(n)), y);
				add(add(x, z), y);
			}
	}
}

// 2-6
// Prove the following lemma by providing a body
// sufficient to establish the ensures annotation.
// Hint: if you get stuck, you should consider
// making use of existing lemmas, or defining a
// new one.
// Note: do not change the provided postcondition,
// you will not receive credit if you do
lemma {:induction false} axm_timessuccessor(x: Nat, y: Nat)
	ensures mult(x, add(y, one())) == add(mult(x, y), x)
{
	match(x) 
	{
		case Zero =>
			calc == 
			{
				mult(x, add(y, one()));
				mult(Zero, add(y, one()));
				Zero;
				{
					axm_pluszero(Zero);
				}
				add(Zero, Zero);
				add(mult(Zero, y), Zero);
				add(mult(x, y), x);
			}
		case Succ(n) =>
			calc == 
			{
				mult(x, add(y, one()));
				mult(Succ(n), add(y, one()));
				add(mult(n, add(y, one())), add(y, one()));
				{
					axm_timessuccessor(n, y);
				}
				add(add(mult(n, y), n), add(y, one()));
				{
					axm_plussuccessor(add(mult(n, y), n), y);
				}
				add(add(add(mult(n, y), n), y), one());
				{
					plus_exchange(mult(n, y), n, y);
				}
				add(add(add(mult(n, y), y), n), one());
				add(add(mult(Succ(n), y), n), one());
				{
					axm_plussuccessor(mult(Succ(n), y), n);
				}
				add(mult(Succ(n), y), add(n, one()));
				{
					add_lemmatwo(n);
				}
				add(mult(Succ(n), y), Succ(n));
				add(mult(x, y), x);
			}
	}
}
