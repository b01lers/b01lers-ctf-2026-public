# __badyuri__ by __draconixia__         

## clues
1. The description hints to take the difference between the two files.
2. There are single characters scattered across yuri_1 that are not
   alphabetical characters. This is the only difference between yuri_1 and
   yuri.
3. The characters are often outside of the usual 32-126 symbols and letters
   represented in unicode and ASCII

## solve
1. Take the unicode values of each character in yuri_1 and yuri.txt and
   subtract them. The remainder is the flag.
