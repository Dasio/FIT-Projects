-- Project: dka-2-mka (FLP 2016/2017)
-- Author: Dávid Mikuš (xmikus15)

{-# LANGUAGE RecordWildCards #-}

module Main
    (main)
  where

import Control.Applicative
import System.Environment
import System.IO

import Type.FSM
import Parser.FSM
import Type.Config
import Parser.Config
import Minimize


main :: IO ()
main = do
    c <- parseArguments <$> getArgs
    input <- hGetContents stdin
    either print (handleFSM input) c

handleFSM input Config{..} = do
    handleFSM <$> readFile path >>= either putStrLn (mapM_ putStrLn)
  where
    handleFSM fsm = do
        fsm' <- parseFSM fsm
        case action of
            Dump -> Right [show fsm']
            Minimize -> Right [show (reduceFSM fsm')]
            MinimizeN -> Right [show (removeNonReachableStates $ reduceFSM fsm')]
