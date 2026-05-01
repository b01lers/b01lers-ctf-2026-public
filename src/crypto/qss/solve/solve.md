# QSS

This challenge was based on the kki snooping attack presented in ["SECURITY OF QUANTUM SECRET SHARING WITH TWO-PARTICLE ENTANGLEMENT AGAINST INDIVIDUAL ATTACKS"](https://www.rintonpress.com/xxqic9/qic-9-910/0765-0772.pdf) which is included in this directory.

The idea is to optmize your Information:QBER ratio as to derive enough information from the secret without causing enough error for a round to be invalidated.

## attack
You are given an ancilla qubit which you are allowed to set the original state. I start with the state $|0\rangle$. Then you are allowed to apply any entangling unitary between the target qubit and your controlled ancilla. In my attack, I apply a cnot(a,c) as the entangling gate.

You then must wait for all parties to publish their information to measure your ancilla, based on the published information you will decide to measuer in (z,z) or (x,z). Then you can infer the secret based on the parity of you b and c measurements (if S0: guess $b \oplus c$, else: guess $\neg (b\oplus c)$)

### notes
This is a more general version of the attack presented in the paper and not part of the optimal attack family that they describe. Originally, I was going to use this attack family but the challenge become too resource intensive.  