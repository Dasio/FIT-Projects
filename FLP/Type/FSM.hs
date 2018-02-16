module Type.FSM
  where

import Data.List
import Data.Ord
import Language.Haskell.TH.Syntax

type State = String
type Symbol = Char

data Transition = Transition
  {
    from :: State,
    symbol :: Symbol,
    to :: State
  } deriving (Eq, Ord)

instance Show Transition where
    show (Transition from symbol to) = filter (\ch -> ch /='"' && ch /='\'') $
        show from ++ "," ++ show symbol ++ "," ++ show to

data FSM = FSM
  {
    alphabet :: [Symbol],
    states :: [State],
    initialState :: State,
    finalStates :: [State],
    transitions :: [Transition]
  } -- deriving (Show)


instance Show FSM where
   show (FSM alphabet states initialState finalStates transitions) =
       -- show alphabet ++ "\n" ++
        intercalate "," states ++ "\n" ++ 
        initialState ++ "\n" ++
        intercalate "," finalStates ++
        foldl (\acc x -> acc ++ "\n" ++ show x) "" transitions

data NewState = NewState
  {
    oldStates :: [State],
    newState :: State
  } deriving (Show)
