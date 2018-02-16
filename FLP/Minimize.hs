-- Project: dka-2-mka (FLP 2016/2017)
-- Author: Dávid Mikuš (xmikus15)

{-# LANGUAGE RecordWildCards #-}

module Minimize
    where

import Type.FSM
import Data.List
import qualified Data.Map as Map
import Data.Maybe
import Data.Ord
import Data.Function
import qualified Data.Set as Set


-- | EXTRA remove states which are non reachable
removeNonReachableStates :: FSM -> FSM
removeNonReachableStates fsm@FSM{..} = fsm
  {
    states = Set.toList newStates ,
    finalStates = Set.toList (Set.fromList finalStates `Set.intersection` newStates) ,
    transitions = filter (\t -> (from t) `elem` newStates && (to t) `elem` newStates) transitions
  }
  where
    step prev = prev `Set.union` Set.fromList
                [fromJust (makeTransition fsm p a) |
                  p <- Set.toList prev, a <- alphabet,
                  isJust $ makeTransition fsm p a]
    alg prev = if (step prev) == prev
                then prev
                else alg (step prev)
    newStates = alg $ Set.singleton initialState

-- | Makes from deterministic finite machine -> well defined
wellFSM :: FSM -> FSM
wellFSM fsm = fsm
    {
        states =  if (null wellTransitions)
                then states fsm
                else states fsm ++ ["SINK"] ,
        transitions = sort $ transitions fsm ++ wellTransitions ++ sinkTransitions
    }
    where
         sinkTransitions = [Transition { from = "SINK", symbol = a, to = "SINK"} | a <- alphabet fsm ]
         wellTransitions = missingTransitions (transitions fsm) (alphabet fsm) (states fsm)
         allTransitions alphabet states = [(state,symbol) | state <- states, symbol <- alphabet]
         missingTransitions transitions alphabet states = [Transition { from = fst tuple, symbol = snd tuple, to = "SINK"} | tuple <- missing ]
         tupleTrans = [(from trans, symbol trans) | trans <- transitions fsm]
         missing = allTransitions (alphabet fsm) (states fsm) \\ tupleTrans

-- | Init algorithm for minimalization
zeroEq :: FSM -> [(State, State)]
zeroEq FSM{..} = [ (p,q) | p <- states, q <- states, (p `elem` finalStates) == (q `elem` finalStates)]

-- | Next step
nextEq :: FSM -> [(State, State)] -> [(State, State)]
nextEq fsm@FSM{..} previous = [ (p,q) | p <- states, q <- states, (p,q) `elem` previous
                        && ekvTrans fsm previous p q]

-- | Returns next state from current state and symbol
makeTransition :: FSM -> State -> Symbol -> Maybe State
makeTransition FSM{..} src sym = case trans of
                                  Just t -> Just (to t)
                                  Nothing -> Nothing
    where
        trans = find (\t -> from t == src && symbol t == sym) transitions

-- | Check if states are in same equivalent class
inSameClass :: FSM -> [(State, State)] -> State -> State -> Symbol -> Bool
inSameClass fsm fclass p q symbol = if (isJust pTrans && isJust qTrans) then
                                     (fromJust pTrans, fromJust qTrans) `elem` fclass
                                     else False
    where
        pTrans = makeTransition fsm p symbol
        qTrans = makeTransition fsm q symbol

-- | Check if every transition of states are in same equivalent class
ekvTrans :: FSM -> [(State, State)] -> State -> State -> Bool
ekvTrans fsm@FSM{..} fclass p q = all (\sym -> inSameClass fsm fclass p q sym) alphabet

-- | Run algorithm for finding equivalent classes
lastEqClass :: FSM -> [(State, State)] -> [(State, State)]
lastEqClass fsm fclass = if nclass == fclass
                            then fclass
                            else lastEqClass fsm nclass
        where
            nclass = nextEq fsm fclass

-- | From equivalent classes returns list of new states with old states
minClasses :: Ord a => [(a, a)] -> [[a]]
minClasses classes = groupBy (\x y -> (x, y) `elem` classes) $ sortBy (compare `on` minRep) $ states
    where
        minRep x = let smallerRep = findSmallerRep x classes in (if smallerRep == x then x else minRep smallerRep)
        findSmallerRep x classes = minimum $ [y | (y, z) <- classes, z == x] ++ [x | (y, z) <- classes, y == x]
        (unzipped1, unzipped2) = unzip classes
        states = nub $ unzipped1 ++ unzipped2

-- | Generate new states from equivalent classes
newStates :: [[State]] -> Map.Map State State
newStates classes =  Map.fromList (foldr (\x acc -> split x ++ acc) [] zipped)
  where
    split x = [(a, show (fst x)) | a <- snd x]
    zipped = zip [1..] classes

-- | Returns new equivalent state for old state
ekvState :: Map.Map State State -> State -> Maybe State
ekvState table state = Map.lookup state table

isSinkState :: FSM -> State -> Bool
isSinkState FSM{..} state = state `notElem` finalStates && all (\t -> (to t) == (from t)) trans
  where
    trans = filter (\t -> (from t) == state) transitions

-- | Main logic of minimalization
reduceFSM' :: FSM -> FSM
reduceFSM' fsm = wdfsm
    {
      states = sort $ nub $ Map.elems table ,
      initialState = case ekvState table (initialState wdfsm) of
                    Just s -> s
                    Nothing -> "EMPTY" ,
      transitions = sort $ nub trans ,
      finalStates = nub [fromJust $ ekvState table x | x <- (finalStates wdfsm), isJust (ekvState table x)]
    }
    where
      wdfsm = wellFSM fsm
      trans = [Transition (fromJust $ ekvState table p) a (fromJust $ ekvState table q) |
              p <- (states wdfsm), q <- (states wdfsm), a <- (alphabet wdfsm),
                isJust (ekvState table p), isJust (ekvState table q),
                (isJust (makeTransition wdfsm p a) && (fromJust $ makeTransition wdfsm p a) == q)]
      table = newStates (minClasses cl)
      cl = lastEqClass wdfsm (zeroEq wdfsm)

-- | Performs minimalization and if needed sink states and transitions connected with it are deleted
reduceFSM :: FSM -> FSM
reduceFSM fsm = case sinkState of
                  Nothing -> reduced
                  Just sink -> reduced
                                {
                                    states = filter (\s -> s /= sink) (states reduced) ,
                                    transitions = filter (\t -> (to t) /= sink && (from t) /= sink) (transitions reduced)
                                }
    where
        reduced = reduceFSM' fsm
        sinkState = find (\s -> isSinkState reduced s) (states reduced)
