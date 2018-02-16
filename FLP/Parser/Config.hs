-- Project: dka-2-mka (FLP 2016/2017)
-- Author: Dávid Mikuš (xmikus15)

module Parser.Config where

import Type.Config

parseArguments :: [String] -> Either String Config
parseArguments [] = Left $ "Missing arguments"
parseArguments [_] = Left $ "Missing arguments"
parseArguments [x,y]
    | x == "-t" = Right $ Config Minimize y
    | x == "-i" = Right $ Config Dump y
    | x == "-f" = Right $ Config MinimizeN y
    | otherwise = Left $ "Unknown argument"
parseArguments _ = Left $ "Too many arguments"
