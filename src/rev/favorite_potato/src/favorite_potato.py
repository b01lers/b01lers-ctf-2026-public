import os
from helpers import run_c64

ROUNDS = 20

menu = """
Choose:
T)est challenge
R)eal challenge"""

def randomAXY():  return os.urandom(3)

def singleEval(binary, msg):
  A0,X0,Y0 = randomAXY()
  A,X,Y = run_c64(binary, A0, X0, Y0)
  assert 0 <= A <= 255 and 0 <= X <= 255 and 0 <= Y <= 255
  print(f"{msg}: A={A} X={X} Y={Y}")
  inputs = [str(v)  for v in (A0,X0,Y0)]
  return inputs


while True:

  print(menu)
  choice = input("> ").strip().upper()

  if choice == "T":
    inputs = singleEval("test.bin", "Final output")
    guesses = input("Tell me the input A,X,Y: ").strip().split(",")
    if guesses == inputs:
      print("Correct!")
    else:
      print("Wrong!")

  elif choice == "R":
    print(f"Here are the results of {ROUNDS} evaluations:")
    allInputs = []
    for i in range(ROUNDS):
      inputs = singleEval("code.bin", f"Final output #{i+1}")
      allInputs.append(inputs)
    print(f"\nNow tell me all {ROUNDS} inputs:")
    good = True
    for i in range(ROUNDS):
      guesses = input(f"Input #{i+1} - A,X,Y: ").strip().split(",")
      good = good and guesses == allInputs[i]
    if good:
      print("Correct!")
      flag = open("flag.txt", "r").read().strip()
      print(f"Here is your flag: {flag}")
      break
    else:
      print("Incorrect, try again")
      break

  else:
    print("goodbye!")
    break


