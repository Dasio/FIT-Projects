-- Project: dka-2-mka (FLP 2016/2017)
-- Author: Dávid Mikuš (xmikus1

module Parser.FSM 
  where

import Data.List
import Type.FSM
import Text.ParserCombinators.ReadP

parseFSM :: String -> Either String FSM
parseFSM s = case readP_to_S fsmParser s of
   [(fsm, _)] -> Right fsm
   x -> Left "Parser error"

fsmParser :: ReadP FSM
fsmParser = do
    states <- parseStates 
    newLine
    initialState <- parseState
    newLine
    finalStates <- parseStates
    newLine
    trans <- parseTransitions 
    eof
    return $ FSM (getAlphabet trans) states initialState finalStates trans
  where
    getAlphabet trans = sort $ nub $ foldr (\x acc -> symbol x : acc) [] trans

newLine = char '\n'
comma = char ','

parseState = many1 $ satisfy (\ch -> ch /= ',' && ch /= '\n')

parseStates = sepBy1 parseState comma

parseTransitions = many1 $ do
    t <- parseTransition
    newLine
    return t
  where
    parseTransition = do
        from <- parseState
        comma
        symbol <- get
        comma
        to <- parseState
        return $ Transition from symbol to
