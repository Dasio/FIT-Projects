module Type.Config where


data Action
    = Dump
    | Minimize
    | MinimizeN
  deriving (Show)

data Config = Config
    { action :: Action
    , path :: FilePath
    }
  deriving (Show)
