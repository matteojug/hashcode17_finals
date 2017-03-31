#pip install pulp
from pulp.solvers import LpMaximize, LpMinimize
from pulp import LpVariable, LpProblem, LpStatus, value

#create variables with ranges
x = LpVariable("x", 0, 3)
y = LpVariable("y", 0, 1)
#create problem
prob = LpProblem("myProblem", LpMinimize)

#create exprations and constraintd
prob += x + y <= 2
prob += -4*x + y

status = prob.solve()

print(LpStatus[status])

print(value(x))
