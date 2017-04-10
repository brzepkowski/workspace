import System.IO
import Control.Monad

intList :: Int -> IO [Int]
intList i = replicateM i (fmap read getLine)

getMaxIndex :: [Int] -> Int
getMaxIndex list =
    let innerGetMaxIndex :: [Int] -> Int -> Int
        innerGetMaxIndex [] _ = error "Empty list"
        innerGetMaxIndex [x] xi = xi
        innerGetMaxIndex (x:xs) xi
            | x > (list !! ti) = xi
            | otherwise      = ti
            where ti = innerGetMaxIndex xs (xi + 1)
    in innerGetMaxIndex list 0


main :: IO ()
main = do
    hSetBuffering stdout NoBuffering -- DO NOT REMOVE
    
    -- Auto-generated code below aims at helping you parse
    -- the standard input according to the problem statement.
    
    loop

loop :: IO ()
loop = do
    
    list <- intList 8
    --putStrLn (show array)
    let index = getMaxIndex list
    
    print index
    
    -- hPutStrLn stderr "Debug messages..."
    
    -- The number of the mountain to fire on.
    
    loop
