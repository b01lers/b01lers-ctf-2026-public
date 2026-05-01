powmod :: Int -> Int -> Int -> Int
powmod a 1 c = mod a c
powmod a b c = let r = powmod a (b - 1) c in mod (a * r) c

main :: IO ()
main =
  (readLn :: IO (Int)) >>= \a ->
  (readLn :: IO (Int)) >>= \b ->
  (readLn :: IO (Int)) >>= \c ->
  print $ powmod a b c