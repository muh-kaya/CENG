module Hw2 where

import Data.List -- YOU MAY USE THIS MODULE FOR SORTING THE AGENTS

data Level = Newbie | Intermediate | Expert deriving (Enum, Eq, Ord, Show, Read)
data Hunter = Hunter {hID::Int, hlevel::Level, hEnergy::Int, hNumberOfCatches::Int, hActions::[Direction]} deriving (Eq, Show, Read)
data Prey = Prey {pID::Int, pEnergy::Int, pActions::[Direction]} deriving (Eq, Show, Read)
data Cell = O | X | H Hunter | P Prey | T deriving (Eq, Show, Read)
data Direction = N | S | E | W deriving (Eq, Show, Read)
type Coordinate = (Int, Int)
-- DO NOT CHANGE THE DEFINITIONS ABOVE. --


-- INSTANCES OF Ord FOR SORTING, UNCOMMENT AND COMPLETE THE IMPLEMENTATIONS --
instance Ord Hunter where
	compare first second
		| hlevel first > hlevel second = GT
		| hlevel first < hlevel second = LT
		| hEnergy first > hEnergy second = GT
		| hEnergy first < hEnergy second = LT
		| hNumberOfCatches first > hNumberOfCatches second = GT
		| hNumberOfCatches first < hNumberOfCatches second = LT
		| hID first < hID second = GT
		| hID first > hID second = LT

instance Ord Prey where
	compare first second
		| pEnergy first > pEnergy second = GT
		| pEnergy first < pEnergy second = LT
		| pID first < pID second = GT
		| pID first > pID second = LT

-- WRITE THE REST OF YOUR CODE HERE --
take_prey :: Cell -> Prey
take_prey (P x) = x

take_hunter :: Cell -> Hunter
take_hunter (H x) = x

check_hunter :: Cell -> Bool
check_hunter (H _) = False
check_hunter (x) = True

check_prey :: Cell -> Bool
check_prey (P _) = False
check_prey (x) = True


find_prey :: [[Cell]] -> Int -> Int -> [(Prey, Coordinate)]
find_prey a 0 0 = if check_prey (a!!0!!0) then [] else [(take_prey (a!!0!!0), (0, 0))]
find_prey a len1 len2 = if check_prey(a!!len1!!len2) then 
													(if len2 /= 0 then find_prey a (len1) (len2-1)
													else find_prey a (len1-1) (length(a!!0)-1) )
						else [(take_prey (a!!len1!!len2), (len2, len1))] ++ (if len2 /= 0 then find_prey a (len1) (len2-1)
													else find_prey a (len1-1) (length(a!!0)-1) )

find_hunter :: [[Cell]] -> Int -> Int -> [(Hunter, Coordinate)]
find_hunter a 0 0 = if check_hunter (a!!0!!0) then [] else [(take_hunter (a!!0!!0), (0, 0))]
find_hunter a len1 len2 = if check_hunter(a!!len1!!len2) then 
													(if len2 /= 0 then find_hunter a (len1) (len2-1)
													else find_hunter a (len1-1) (length(a!!0)-1) )
						else [(take_hunter (a!!len1!!len2), (len2, len1))] ++ (if len2 /= 0 then find_hunter a (len1) (len2-1)
													else find_hunter a (len1-1) (length(a!!0)-1) )

sort_combine_lists :: [(Hunter, Coordinate)] -> [(Prey, Coordinate)] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
sort_combine_lists a b = ( (sort a), reverse (sort b) )

sort_lists :: ([(Hunter, Coordinate)], [(Prey, Coordinate)]) -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
sort_lists (a, b) = ( (sort a), reverse (sort b) )

sort_finish :: ([(Hunter, Coordinate)], [(Prey, Coordinate)]) -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
sort_finish (a, b) = ( reverse (sort a), (sort b))

check_catching :: ([(Hunter, Coordinate)], [(Prey, Coordinate)]) -> Int -> Int -> Int -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
check_catching (h, p) 0 0 count = if count /= 2 then (if (snd(h!!0) == snd(p!!0)) then (increase_hunter_enery_20 h 0, delete (p!!0) p) else (h, p)) else (h, p)
check_catching (h, p) lenh lenp count = if count /= 2 then (if (snd(h!!lenh) /= snd(p!!lenp)) then (if lenp == 0 then check_catching (h, p) (lenh-1) ((length p)-1) 0
																								else check_catching (h, p) lenh (lenp-1) count)
															else (if lenp /= 0 then check_catching (increase_hunter_enery_20 h lenh, delete (p!!lenp) p) lenh (lenp-1) (count+1) else  (increase_hunter_enery_20 h lenh, delete (p!!0) p)   ))  else  check_catching (h, p) (lenh-1) ((length p)-1) 0

increase_hunter_enery_20 :: [(Hunter, Coordinate)] -> Int -> [(Hunter, Coordinate)]
increase_hunter_enery_20 a index = take index a ++ [(Hunter {hID = hID (fst(a!!index)), hlevel = hlevel (fst(a!!index)), hEnergy = (if hEnergy (fst(a!!index))<=80 then (hEnergy (fst(a!!index)))+20 else 100), hNumberOfCatches = (hNumberOfCatches (fst(a!!index)) + 1), hActions = hActions (fst(a!!index))}, snd(a!!index))] ++ drop (index+1) a


decrease_energy_p :: Prey -> Int -> Prey
decrease_energy_p p trap
		| trap == 0 = Prey {pID = pID p, pEnergy = (pEnergy p)-1, pActions = (tail (pActions p))}
		| trap == 1 = Prey {pID = pID p, pEnergy = (if pEnergy p <=10 then 0 else (pEnergy p-11)), pActions = (tail (pActions p))}

decrease_0 :: Prey -> Prey
decrease_0 p = Prey {pID = pID p, pEnergy = 0, pActions = (tail (pActions p))}

decrease_energy_h :: Hunter -> Hunter
decrease_energy_h h = Hunter {hID = hID h, hlevel = hlevel h, hEnergy = (hEnergy h)-1, hNumberOfCatches = hNumberOfCatches h, hActions = (tail (hActions h))}

increase_energy_h :: Hunter -> Hunter
increase_energy_h h = Hunter {hID = hID h, hlevel = hlevel h, hEnergy = (hEnergy h) + 1, hNumberOfCatches = hNumberOfCatches h, hActions = (tail (hActions h))}

increase_energy_p :: Prey -> Prey
increase_energy_p p = Prey {pID = pID p, pEnergy = (pEnergy p)+1, pActions = (tail (pActions p))}

check_move_h :: [[Cell]] -> Direction -> (Hunter, Coordinate) -> Int -> Int -> (Hunter, Coordinate)
check_move_h a d (h, co) len1 len2
			| (hEnergy h < 10) = (increase_energy_h h, co)
			| d == W = if fst(co) /= 0 then (if a!!snd(co)!!(fst(co)-1) /= X then (decrease_energy_h h, ((fst(co)-1), snd(co))) else (decrease_energy_h h, (fst(co), snd(co)))) else (decrease_energy_h h, (fst(co), snd(co)))
			| d == E = if fst(co) /= len2 then (if a!!snd(co)!!(fst(co)+1) /= X then (decrease_energy_h h, (fst(co)+1, snd(co))) else (decrease_energy_h h, (fst(co), snd(co)))) else (decrease_energy_h h, (fst(co), snd(co)))
			| d == N = if snd(co) /= 0 then (if a!!(snd(co)-1)!!fst(co) /= X then (decrease_energy_h h, (fst(co), (snd(co)-1))) else (decrease_energy_h h, (fst(co), snd(co)))) else (decrease_energy_h h, (fst(co), snd(co)))
			| d == S = if snd(co) /= len1 then (if a!!(snd(co)+1)!!fst(co) /= X then (decrease_energy_h h, (fst(co), (snd(co)+1))) else (decrease_energy_h h, (fst(co), snd(co)))) else (decrease_energy_h h, (fst(co), snd(co)))

check_move_p :: [[Cell]] -> Direction -> (Prey, Coordinate) -> Int -> Int -> (Prey, Coordinate)
check_move_p a d (p, co) len1 len2
			| (pEnergy p < 10) = if a!!snd(co)!!(fst(co)) /= T then (increase_energy_p p, co) else (decrease_0 p, co)
			| d == W = if fst(co) /= 0 then (if a!!snd(co)!!(fst(co)-1) /= X then (if a!!snd(co)!!(fst(co)-1) /= T then (decrease_energy_p p 0, ((fst(co)-1), snd(co))) else (decrease_energy_p p 1, ((fst(co)-1), snd(co))))  else (decrease_energy_p p 0, (fst(co), snd(co)))) else (if a!!snd(co)!!(fst(co)) /= T then (decrease_energy_p p 0, (fst(co), snd(co))) else (decrease_energy_p p 1, (fst(co), snd(co)))) 
			| d == E = if fst(co) /= len2 then (if a!!snd(co)!!(fst(co)+1) /= X then (if a!!snd(co)!!(fst(co)+1) /= T then (decrease_energy_p p 0, (fst(co)+1, snd(co))) else (decrease_energy_p p 1, ((fst(co)+1), snd(co)))) else (decrease_energy_p p 0, (fst(co), snd(co)))) else (if a!!snd(co)!!(fst(co)) /= T then (decrease_energy_p p 0, (fst(co), snd(co))) else (decrease_energy_p p 1, (fst(co), snd(co))))
			| d == N = if snd(co) /= 0 then (if a!!(snd(co)-1)!!fst(co) /= X then (if a!!(snd(co)-1)!!(fst(co)) /= T then (decrease_energy_p p 0, (fst(co), (snd(co)-1))) else (decrease_energy_p p 1, ((fst(co)), snd(co)-1))) else (decrease_energy_p p 0, (fst(co), snd(co)))) else (if a!!snd(co)!!(fst(co)) /= T then (decrease_energy_p p 0, (fst(co), snd(co))) else (decrease_energy_p p 1, (fst(co), snd(co))))
			| d == S = if snd(co) /= len1 then (if a!!(snd(co)+1)!!fst(co) /= X then (if a!!(snd(co)+1)!!fst(co) /= T then (decrease_energy_p p 0, (fst(co), (snd(co)+1))) else (decrease_energy_p p 1, ((fst(co)), snd(co)+1))) else (decrease_energy_p p 0, (fst(co), snd(co)))) else (if a!!snd(co)!!(fst(co)) /= T then (decrease_energy_p p 0, (fst(co), snd(co))) else (decrease_energy_p p 1, (fst(co), snd(co))))


move_h :: [[Cell]] -> [(Hunter, Coordinate)] -> Int -> [(Hunter, Coordinate)]
move_h a h 0 = [check_move_h a (head (hActions (fst(h!!0)))) (h!!0) ((length a)-1) (length (a!!0)-1)]
move_h a h index = [check_move_h a (head (hActions (fst(h!!index)))) (h!!index) ((length a)-1) (length (a!!0)-1)] ++ move_h a h (index-1)

move_p :: [[Cell]] -> [(Prey, Coordinate)] -> Int -> [(Prey, Coordinate)]
move_p a p 0 = [check_move_p a (head (pActions (fst(p!!0)))) (p!!0) ((length a)-1) (length (a!!0)-1)]
move_p a p index = [check_move_p a (head (pActions (fst(p!!index)))) (p!!index) ((length a)-1) (length (a!!0)-1)] ++ move_p a p (index-1)

move :: [[Cell]] -> [(Hunter, Coordinate)] -> [(Prey, Coordinate)] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
move a h p = ((move_h a h ((length h)-1)), (move_p a p ((length p)-1)))

simulate :: [[Cell]] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
simulate a = simulate1 a (sort_combine_lists (find_hunter a ((length a) -1) (length (a!!0)-1)) (find_prey a ((length a)-1) (length (a!!0)-1)))

simulate1 :: [[Cell]] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)]) -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
simulate1 a (h, p) = if hActions (fst(h!!0)) == [] || p == [] then sort_finish (h, p)
															else simulate2 a (h, p)
simulate2 :: [[Cell]] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)]) -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
simulate2 a (h, p) = simulate1 a ( (check_catching (sort_lists(move a h p)) (length h-1) (length p-1) 0))

