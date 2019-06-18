module Hw1 where

data Cell = H | P | O | X deriving (Read,Show,Eq)
data Result = Fail | Caught (Int,Int) deriving (Read,Show,Eq)
data Direction = N | S | E | W deriving (Read,Show,Eq)

simulate :: [[Cell]] -> [(Direction, Direction)] -> Result
-- DO NOT CHANGE ABOVE THIS LINE, WRITE YOUR CODE BELOW --

find_hunter :: [[Cell]] -> Int -> Int -> (Int, Int)
find_hunter a 0 0 = if a!!0!!0 /= H then (-1,-1) else (0, 0)
find_hunter a len1 len2 = if a!!len1!!len2 /= H then 
													(if len2 /= 0 then find_hunter a (len1) (len2-1)
													else find_hunter a (len1-1) (length(a!!0)-1) )
						else (len2, len1)


find_prey :: [[Cell]] -> Int -> Int -> (Int, Int)
find_prey a 0 0 = if a!!0!!0 /= P then (-1,-1) else (0, 0)
find_prey a len1 len2 = if a!!len1!!len2 /= P then 
													(if len2 /= 0 then find_prey a (len1) (len2-1)
													else find_prey a (len1-1) (length(a!!0)-1) )
						else (len2, len1)


check_move :: [[Cell]] -> Direction -> (Int, Int) -> Int -> Int -> (Int, Int)
check_move a d co len1 len2
			| d == W = if fst(co) /= 0 then (if a!!snd(co)!!(fst(co)-1) /= X then ((fst(co)-1), snd(co)) else (fst(co), snd(co))) else (fst(co), snd(co))
			| d == E = if fst(co) /= len2 then (if a!!snd(co)!!(fst(co)+1) /= X then (fst(co)+1, snd(co)) else (fst(co), snd(co))) else (fst(co), snd(co))
			| d == N = if snd(co) /= 0 then (if a!!(snd(co)-1)!!fst(co) /= X then (fst(co), (snd(co)-1)) else (fst(co), snd(co))) else (fst(co), snd(co))
			| d == S = if snd(co) /= len1 then (if a!!(snd(co)+1)!!fst(co) /= X then (fst(co), (snd(co)+1)) else (fst(co), snd(co))) else (fst(co), snd(co))

simulate1 :: [[Cell]] -> [(Direction, Direction)] -> (Int, Int) -> (Int, Int) -> Result
simulate1 a [] (x1, x2) (y1, y2) = Fail
simulate1 a (d:xs) (x1, y1) (x2, y2) = if (check_move a (fst d) (x1, y1) ((length a)-1) (length(a!!0)-1)) == (check_move a (snd d) (x2, y2) ((length a)-1) (length(a!!0)-1)) 
						then Caught (check_move a (fst d) (x1, y1) ((length a)-1) (length(a!!0)-1)) 
						else simulate1 a xs (check_move a (fst d) (x1, y1) ((length a)-1) (length (a!!0)-1)) (check_move a (snd d) (x2, y2) ((length a)-1) (length (a!!0)-1))


simulate a d = simulate1 a d (find_hunter a ((length a) -1) (length (a!!0)-1)) (find_prey a ((length a)-1) (length (a!!0)-1))  